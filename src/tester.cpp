#include <iostream>
#include "ant-simulator-core.hpp"
#include "cpp-lib/pgm8.hpp"

int main(int const argc, char const *const *const argv) {
  if (argc != 2) {
    std::cout << "usage: <pgm_pathname>\n";
    return 1;
  }

  std::array<asc::Rule, 256> rules{};
  rules[0] = asc::Rule(true, 1, TD_LEFT);
  rules[1] = asc::Rule(true, 0, TD_RIGHT);
  uint16_t const gridWidth = 75, gridHeight = 75;

  asc::Simulation sim(
    gridWidth, gridHeight, 1,
    gridWidth / 2, gridHeight / 2, AO_WEST,
    rules
  );

  std::cout << "running simulation... ";
  try {
    for (size_t i = 0; i < 1000000 && !sim.is_finished(); ++i) {
      sim.step_once();
    }
  } catch (...) {
    std::cerr << "failed\n";
    return 2;
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
      return 3;
    }
    try {
      pgm8::write_bin(
        &pgm,
        gridWidth, gridHeight, 1,
        reinterpret_cast<unsigned char const *>(sim.grid().data())
      );
    } catch (...) {
      std::cerr << "failed\n";
      return 4;
    }
  }
  std::cout << "done\n";

  return 0;
}

// g++ src/*.cpp src/cpp-lib/*.cpp -Wall -std=c++2a -o bin/tester.exe
