#include "util.h"

#include <cstdio>

std::string Format(const char* format, ...) {
    std::string result(256, 0);
    va_list args;

    while (true) {
        va_start(args, format);
        int n = vsnprintf((char*)result.data(), result.size(), format, args);
        va_end(args);
        if (n <= result.size()) {
            return result;
        }
        result.resize(result.size() * 2);
    }
}

std::string Format(const char* format, va_list args) {
    std::string result(256, 0);

    while (true) {
        int n = vsnprintf((char*)result.data(), result.size(), format, args);
        if (n <= result.size()) {
            return result;
        }
        result.resize(result.size() * 2);
    }
}