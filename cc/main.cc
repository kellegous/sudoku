#include <iostream>
#include <string>
#include <vector>

#include "puzzle.h"

int main(int argc, char** argv) {
    for (std::string line; std::getline(std::cin, line);) {
        Puzzle p;
        Status s = p.parse(line);
        if (!s.is_ok()) {
            std::cout << s.message() << std::endl;
        }
    }

    return 0;
}