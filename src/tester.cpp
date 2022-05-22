#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <filesystem>
#include <sstream>
#include "ant-simulator-core.hpp"
#include "cpp-lib/includes/arr2d.hpp"
#include "cpp-lib/includes/pgm8.hpp"

int main(int const, char *const *const argv) {
  {
    namespace fs = std::filesystem;

    std::ifstream f(argv[1]);
    if (!f.is_open() || !f.good()) {
      return 1;
    }

    std::ostringstream oss;
    oss << f.rdbuf();
    std::string str = oss.str();

    asc::Simulation::parse(str);
  }

  return 0;
}

// g++ src/*.cpp src/cpp-lib/*.cpp -Wall -std=c++2a -o bin/tester.exe
