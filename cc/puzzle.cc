#include "puzzle.h"

#include <array>
#include <iostream>
#include <unordered_set>

#include "util.h"

namespace {

constexpr size_t quad_of(size_t ix) {
    return (ix % 9) / 3 + 3 * (ix / 27);
}

constexpr size_t col_of(size_t ix) {
    return ix % 9;
}

constexpr size_t row_of(size_t ix) {
    return ix / 9;
}

consteval auto get_peers() {
    std::array<size_t, 81 * 20> peers = {};
    auto iter = peers.begin();
    for (auto i = 0; i < 81; i++) {
        auto row = row_of(i);
        auto col = col_of(i);
        auto quad = quad_of(i);
        for (auto j = 0; j < 81; j++) {
            if (i == j) {
                continue;
            }

            if (row == row_of(j) || col == col_of(j) || quad == quad_of(j)) {
                *iter++ = j;
            }
        }
    }
    return peers;
};

size_t least_ambiguious_cell_index(const std::vector<Cell>& cells) {
    size_t ix = 0;
    int min = 10;
    for (size_t i = 0, n = cells.size(); i < n; ++i) {
        auto c = cells[i].count();
        if (c > 1 && c < min) {
            min = c;
            ix = i;
        }
    }
    return ix;
}

}  // namespace

bool Puzzle::assign(int ix, int value) {
    // peers computed during compile time.
    static constexpr std::array<size_t, 81 * 20> peers = get_peers();

    m_cells[ix] = m_cells[ix].assign(value);
    size_t off = ix * 20;
    for (size_t i = 0; i < 20; ++i) {
        auto peer = peers[off + i];
        auto o = m_cells[peer];
        auto n = o.remove(value);
        if (o == n) {
            continue;
        }
        m_cells[peer] = n;
        if (n.is_empty()) {
            return false;
        }
        auto soln = n.solution();
        if (soln) {
            if (!assign(peer, *soln)) {
                return false;
            }
        }
    }

    return true;
}

Result<Puzzle, std::string> Puzzle::parse(const std::string& s) {
    if (s.size() != 81) {
        return Err(std::string("Puzzle must be 81 characters long"));
    }

    Puzzle p;
    for (auto i = 0; i < 81; ++i) {
        auto c = s[i];
        if (c == '.' || c == '0') {
            continue;
        } else if (c > '0' && c <= '9') {
            if (!p.assign(i, c - '0')) {
                return Err(std::string("conflicting assignment"));
            }
        } else {
            return Err(Format("Invalid character '%c' at position %d", c, i));
        }
    }

    return Ok(p);
}

std::string Puzzle::to_string() const {
    std::string s;
    s.reserve(81);
    for (auto i = 0; i < 81; ++i) {
        auto c = m_cells[i].solution();
        s.push_back(c.has_value() ? c.value() + '0' : '.');
    }
    return s;
}

std::string Puzzle::debug() const {
    std::string res;
    int w = 0;
    for (auto c : m_cells) {
        w = std::max(w, c.count());
    }
    std::string q(3 * (w + 2), '-');
    std::string d = Format("\n%s-%s-%s\n", q.c_str(), q.c_str(), q.c_str());
    std::string f = Format(" %%%ds ", w);

    for (auto i = 0; i < 81; i++) {
        std::string vals = m_cells[i].to_string();
        if (i == 0) {
        } else if (i % 27 == 0) {
            res += d;
        } else if (i % 9 == 0) {
            res += '\n';
        } else if (i % 3 == 0) {
            res += "|";
        }
        res += Format(f.c_str(), vals.c_str());
    }

    return res;
}

bool Puzzle::is_solved() const {
    for (auto c : m_cells) {
        if (c.count() != 1) {
            return false;
        }
    }
    return true;
}

std::optional<Puzzle> Puzzle::solve() const {
    if (is_solved()) {
        return *this;
    }

    auto ix = least_ambiguious_cell_index(m_cells);
    for (auto v : m_cells[ix]) {
        auto p = *this;
        if (p.assign(ix, v)) {
            auto soln = p.solve();
            if (soln) {
                return soln;
            }
        }
    }

    return std::nullopt;
}