#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include <vector>

#include "cell.h"
#include "status.h"

class Puzzle {
   public:
    Puzzle() : m_cells(81, Cell::with_all_values()) {}

    Status parse(const std::string& s);

   private:
    std::vector<Cell> m_cells;
};

#endif