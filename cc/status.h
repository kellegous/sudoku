#ifndef __STATUS_H__
#define __STATUS_H__

#include <string>

class Status {
   public:
    Status() noexcept : m_message("") {}

    Status(const char* message) noexcept : m_message(message) {}

    Status(const std::string& message) noexcept : m_message(message) {}

    Status(const Status& s) noexcept : m_message(s.m_message) {}

    Status(const Status&& s) noexcept : m_message(s.m_message) {}

    static Status Ok();

    static Status Error(const char* message);

    static Status Errorf(const char* format, ...);

    bool is_ok() const;

    const char* message() const;

   private:
    std::string m_message;
};

#endif
