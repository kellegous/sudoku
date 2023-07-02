#include <iostream>
#include <string>
#include <vector>

#include "puzzle.h"
#include "result.h"
#include "util.h"

int main(int argc, char** argv) {
    for (std::string line; std::getline(std::cin, line);) {
        auto r = Puzzle::parse(line);
        if (r.is_err()) {
            std::cerr << r.take_err_value() << std::endl;
            std::terminate();
        }
        Puzzle p = r.take_ok_value();
        std::cout << line << std::endl
                  << p.to_string() << std::endl
                  << std::endl;
    }

    return 0;
}