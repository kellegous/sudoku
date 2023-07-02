#include "status.h"

#include "util.h"

Status Status::Ok() {
    return Status();
}

Status Status::Error(const char* message) {
    return Status(message);
}

Status Status::Errorf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::string msg = Format(format, args);
    va_end(args);
    return Status(msg);
}

bool Status::is_ok() const {
    return m_message == "";
}

const char* Status::message() const {
    return m_message.c_str();
}