#ifndef __RESULT_H__
#define __RESULT_H__

// h/t https://yegor.pomortsev.com/post/result-type/

#include <variant>

template <typename T>
class Ok {
   public:
    explicit constexpr Ok(T value) : m_value(std::move(value)) {}

    constexpr T&& take_value() {
        return std::move(m_value);
    }

   private:
    T m_value;
};

template <typename T>
class Err {
   public:
    explicit constexpr Err(T value) : m_value(std::move(value)) {}

    constexpr T&& take_value() {
        return std::move(m_value);
    }

   private:
    T m_value;
};

template <typename V, typename E>
class Result {
   public:
    constexpr Result(Ok<V> value) : m_variant(std::move(value)) {}
    constexpr Result(Err<E> value) : m_variant(std::move(value)) {}

    constexpr bool is_ok() const {
        return std::holds_alternative<Ok<V>>(m_variant);
    }

    constexpr bool is_err() const {
        return std::holds_alternative<Err<E>>(m_variant);
    }

    constexpr V ok() const {
        return std::get<Ok<V>>(m_variant).value;
    }

    constexpr E err() const {
        return std::get<Err<E>>(m_variant).value;
    }

    constexpr V&& take_ok() {
        return std::get<Ok<V>>(m_variant).take_value();
    }

    constexpr E&& take_err() {
        return std::get<Err<E>>(m_variant).take_value();
    }

   private:
    std::variant<Ok<V>, Err<E>> m_variant;
};

#endif  // __RESULT_H__