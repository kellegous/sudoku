#include "cell.h"

#include <bit>

Cell Cell::with_all_values() {
    return Cell(static_cast<uint16_t>(0x1ff));
}

Cell Cell::add(int value) const {
    return Cell(m_values | (1 << (value - 1)));
}

Cell Cell::remove(int value) const {
    return Cell(m_values & ~(1 << (value - 1)));
}

Cell Cell::assign(int value) const {
    return Cell(1 << (value - 1));
}

Cell::Iterator::value_type Cell::Iterator::operator*() const {
    return std::countr_zero(m_values) + 1;
}

Cell::Iterator Cell::Iterator::operator++() {
    m_values &= m_values - 1;
    return *this;
}