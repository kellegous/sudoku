#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

std::string Format(const char* format, ...);
std::string Format(const char* format, va_list args);
#endif