#include "puzzle.h"

#include <iostream>
#include <unordered_set>

#include "util.h"

namespace {

size_t quad_of(size_t ix) {
    return (ix % 9) / 3 + 3 * (ix / 27);
}

size_t col_of(size_t ix) {
    return ix % 9;
}

size_t row_of(size_t ix) {
    return ix / 9;
}

std::vector<std::unordered_set<size_t>> get_peers() {
    std::vector<std::unordered_set<size_t>> peers(81);
    for (auto i = 0; i < 81; ++i) {
        auto row = row_of(i);
        auto col = col_of(i);
        auto quad = quad_of(i);
        for (auto j = 0; j < 81; ++j) {
            if (i != j && (row == row_of(j) || col == col_of(j) || quad == quad_of(j))) {
                peers[i].insert(j);
            }
        }
    }
    return peers;
}

}  // namespace

Result<bool, std::string> Puzzle::assign(int ix, int value) {
    static auto peers = get_peers();

    m_cells[ix] = m_cells[ix].assign(value);
    for (auto peer : peers[ix]) {
        auto o = m_cells[peer];
        auto n = o.remove(value);
        if (o == n) {
            continue;
        }
        m_cells[peer] = n;
        if (n.is_empty()) {
            return Err(std::string("conflicting assignment"));
        }
        auto soln = n.solution();
        if (soln) {
            auto res = assign(peer, *soln);
            if (!res.is_ok()) {
                return res;
            }
        }
    }

    return Ok(true);
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
            auto res = p.assign(i, c - '0');
            if (!res.is_ok()) {
                return Err(res.take_err_value());
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