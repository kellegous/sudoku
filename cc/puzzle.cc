#include "puzzle.h"

#include <iostream>

Status Puzzle::parse(const std::string& s) {
    if (s.size() != 81) {
        return Status::Error("Puzzle must be 81 characters long");
    }

    for (auto i = 0; i < 81; ++i) {
        auto c = s[i];
        if (c == '.' || c == '0') {
            continue;
        } else if (c > '0' && c <= '9') {
            m_cells[i] = m_cells[i].assign(c - '0');
        } else {
            return Status::Errorf("Invalid character '%c' at position %d", c, i);
        }
    }

    return Status::Ok();
}