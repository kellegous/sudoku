#include <iostream>
#include <string>
#include <vector>

#include "puzzle.h"

int main(int argc, char** argv) {
    for (std::string line; std::getline(std::cin, line);) {
        auto r = Puzzle::parse(line);
        if (r.is_err()) {
            std::cerr << r.take_err() << std::endl;
            std::terminate();
        }
        std::cout << line << std::endl;
        auto p = r.take_ok().solve();
        if (p.has_value()) {
            std::cout << p.value().to_string() << std::endl
                      << std::endl;
        } else {
            std::cout << "no solution" << std::endl
                      << std::endl;
        }
    }

    return 0;
}