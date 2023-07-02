#include <iostream>
#include <string>
#include <vector>

#include "puzzle.h"

int main(int argc, char** argv) {
    for (std::string line; std::getline(std::cin, line);) {
        Puzzle p;
        Status did = p.parse(line);
        if (!did.ok()) {
            std::cout << did.message() << std::endl;
        }

        std::cout << line << std::endl
                  << p.debug() << std::endl
                  << std::endl;
    }

    return 0;
}