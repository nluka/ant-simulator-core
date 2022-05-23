#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include "ant-simulator-core.hpp"
#include "cpp-lib/includes/arr2d.hpp"
#include "cpp-lib/includes/pgm8.hpp"

int main(int const argc, char *const *const argv) {
  if (argc < 9) {
    std::cout << "usage: "
      "<pgm_pathname> "
      "<max_iters> "
      "<grid_width> "
      "<grid_height> "
      "<grid_color> "
      "<ant_col> "
      "<ant_row> "
      "<ant_orient> "
      "[rule_color,rule_replacementcolor,rule_turndir]+\n";
    return 1;
  }

  uint_fast64_t maxIterations;
  uint_fast16_t gridWidth, gridHeight, antCol, antRow;
  uint8_t gridColor;
  int_fast8_t antOrientation;
  std::array<asc::Rule, 256> rules{};

  #define PARSE_ARG(argName, var, parser, cliVal)     \
  try {                                               \
    var = static_cast<decltype(var)>(parser(cliVal)); \
  } catch (...) {                                     \
    std::cerr << "ERROR: failed to parse <"           \
      << argName << "> value\n";                      \
    return 2;                                         \
  }

  PARSE_ARG("max_iters",   maxIterations,  std::stoull, argv[2]);
  PARSE_ARG("grid_width",  gridWidth,      std::stoul,  argv[3]);
  PARSE_ARG("grid_height", gridHeight,     std::stoul,  argv[4]);
  PARSE_ARG("grid_color",  gridColor,      std::stoul,  argv[5]);
  PARSE_ARG("ant_col",     antCol,         std::stoul,  argv[6]);
  PARSE_ARG("ant_row",     antRow,         std::stoul,  argv[7]);
  PARSE_ARG("ant_orient",  antOrientation, std::stoi,   argv[8]);

  #undef PARSE_ARG

  // parse rules
  for (int i = 9, ruleNum = 1; i < argc; ++i, ++ruleNum) {
    char const
      *const delim = ",",
      *const color = strtok(argv[i], delim),
      *const replacementColor = strtok(NULL, delim),
      *const turnDir = strtok(NULL, delim);

    uint8_t parsedColor, parsedReplacementColor;

    #define PARSE_RULE_PART(partName, var, parser, cliVal) \
    try {                                                  \
      var = static_cast<decltype(var)>(parser(cliVal));    \
    } catch (...) {                                        \
      std::cerr << "ERROR: failed to parse <"              \
        << partName << "> for rule " << ruleNum << '\n';   \
      exit(3);                                             \
    }

    PARSE_RULE_PART("rule_color",            parsedColor,            std::stoul, color)
    PARSE_RULE_PART("rule_replacementcolor", parsedReplacementColor, std::stoul, replacementColor)

    #undef PARSE_RULE_PART

    // parse rule_turndir
    int_fast8_t const parsedTurnDir = ([turnDir, &parsedTurnDir, ruleNum](){
      switch (turnDir[0]) {
        case 'l':
        case 'L':
          return TD_LEFT;
        case 'n':
        case 'N':
          return TD_NONE;
        case 'r':
        case 'R':
          return TD_RIGHT;
        default:
          std::cerr << "ERROR: failed to parse <rule_turndir> for rule "
            << ruleNum << '\n';
          exit(3);
      }
    })();

    rules[parsedColor] = asc::Rule(parsedReplacementColor, parsedTurnDir);
  }

  asc::Simulation sim;
  try {
    sim = asc::Simulation(
      gridWidth, gridHeight, gridColor,
      antCol, antRow, antOrientation,
      rules
    );
  } catch (std::string const &err) {
    std::cerr << "ERROR: " << err << '\n';
    return 4;
  }

  std::cout << "running simulation... ";
  try {
    for (size_t i = 0; i < maxIterations && !sim.is_finished(); ++i) {
      sim.step_once();
    }
  } catch (...) {
    std::cerr << "failed\n";
    return 5;
  }
  std::cout << "done\n";

  asc::StepResult lastStepRes = sim.last_step_result();
  std::cout << "result: "
    << (lastStepRes == asc::StepResult::FAILED_AT_BOUNDARY
      ? "hit boundary"
      : "reached max iterations")
    << '\n';

  std::cout << "writing `" << argv[1] << "`... ";
  {
    std::ofstream pgm(argv[1]);
    if (!pgm.is_open()) {
      std::cerr << "failed\n";
      return 6;
    }
    try {
      pgm8::write_ascii(
        &pgm,
        static_cast<uint16_t>(gridWidth),
        static_cast<uint16_t>(gridHeight),
        arr2d::max<uint8_t>(sim.grid(), gridWidth, gridHeight),
        sim.grid()
      );
    } catch (...) {
      std::cerr << "failed\n";
      return 6;
    }
  }
  std::cout << "done\n";

  return 0;
}

// g++ src/*.cpp src/cpp-lib/*.cpp -Wall -std=c++2a -o bin/tester.exe
