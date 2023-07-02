#ifndef __CELL_H__
#define __CELL_H__

#include <cstdint>
#include <iterator>
#include <optional>
#include <string>

class Cell {
   public:
    Cell(uint16_t values) noexcept : m_values(values) {}

    Cell(const Cell& c) noexcept : m_values(c.m_values) {}

    Cell(const Cell&& c) noexcept : m_values(c.m_values) {}

    Cell& operator=(const Cell& c) noexcept {
        m_values = c.m_values;
        return *this;
    }

    friend bool operator==(const Cell& a, const Cell& b) {
        return a.m_values == b.m_values;
    }

    friend bool operator!=(const Cell& a, const Cell& b) {
        return a.m_values != b.m_values;
    }

    static Cell with_all_values();

    Cell add(int value) const;

    Cell remove(int value) const;

    Cell assign(int value) const;

    int count() const;

    std::optional<int> solution() const;

    bool is_empty() const {
        return m_values == 0;
    }

    std::string to_string() const;

    struct Iterator {
        using iterator_category = std::input_iterator_tag;
        using value_type = int;

        Iterator(uint16_t values) : m_values(values) {}

        value_type operator*() const;

        Iterator operator++();

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.m_values != b.m_values;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.m_values == b.m_values;
        }

       private:
        uint16_t m_values;
    };

    Iterator begin() const {
        return Iterator(m_values);
    }

    Iterator end() const {
        return Iterator(0);
    }

   private:
    uint16_t m_values;
};

#endif