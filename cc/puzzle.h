#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include <optional>
#include <vector>

#include "cell.h"
#include "result.h"

class Puzzle {
   public:
    Puzzle() : m_cells(81, Cell::with_all_values()) {}

    Puzzle(const Puzzle& p) : m_cells(p.m_cells) {}

    static Result<Puzzle, std::string> parse(const std::string& s);

    std::string to_string() const;

    std::string debug() const;

    bool is_solved() const;

    std::optional<Puzzle> solve() const;

   private:
    Result<bool, std::string> assign(int index, int value);

    std::vector<Cell> m_cells;
};

#endif