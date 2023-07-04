use std::{
    error::Error,
    io::{self, BufRead},
};

const fn quad_of(ix: usize) -> usize {
    (ix % 9) / 3 + 3 * (ix / 27)
}

const fn col_of(ix: usize) -> usize {
    ix % 9
}

const fn row_of(ix: usize) -> usize {
    ix / 9
}

const fn get_peers() -> [usize; 81 * 20] {
    // for loops are not allowed in const fns
    let mut peers = [0; 81 * 20];
    let mut i = 0;
    while i < 81 {
        let row = row_of(i);
        let col = col_of(i);
        let quad = quad_of(i);
        let mut offset = i * 20;
        let mut j = 0;
        while j < 81 {
            if i != j && (row_of(j) == row || col_of(j) == col || quad_of(j) == quad) {
                peers[offset] = j;
                offset += 1;
            }
            j += 1;
        }
        i += 1;
    }
    peers
}

// peers are computed at compile time
const PEERS: [usize; 81 * 20] = get_peers();

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
struct Cell {
    values: u16,
}

impl Cell {
    fn with_all() -> Self {
        Self { values: 0x1ff }
    }

    #[allow(dead_code)]
    fn empty() -> Self {
        Self { values: 0 }
    }

    #[allow(dead_code)]
    fn with_values(values: &[i32]) -> Self {
        values.iter().fold(Self::empty(), |cell, &v| cell.add(v))
    }

    #[allow(dead_code)]
    fn add(self, v: i32) -> Self {
        Self {
            values: self.values | (1 << (v - 1)),
        }
    }

    fn remove(self, v: i32) -> Self {
        Self {
            values: self.values & !(1 << (v - 1)),
        }
    }

    fn solution(self) -> Option<i32> {
        if self.count() == 1 {
            Some(self.values().next().unwrap())
        } else {
            None
        }
    }

    fn contains(self, v: i32) -> bool {
        self.values & (1 << (v - 1)) != 0
    }

    fn is_empty(self) -> bool {
        self.values == 0
    }

    fn count(self) -> usize {
        self.values.count_ones() as usize
    }

    fn values(self) -> impl Iterator<Item = i32> {
        (1..=9).filter(move |&v| self.contains(v))
    }

    fn assign(self, v: i32) -> Self {
        Self {
            values: 1 << (v - 1),
        }
    }
}

#[derive(Clone)]
struct Puzzle {
    cells: [Cell; 9 * 9],
}

impl Puzzle {
    fn assign(&mut self, ix: usize, v: i32) -> Result<(), Box<dyn Error>> {
        self.cells[ix] = self.cells[ix].assign(v);
        let offset = ix * 20;
        for i in 0..20 {
            let peer = PEERS[offset + i];
            let old = self.cells[peer];
            let new = old.remove(v);
            if old == new {
                continue;
            }
            self.cells[peer] = new;
            if new.is_empty() {
                return Err("conflicting assignment".into());
            } else if let Some(v) = new.solution() {
                self.assign(peer, v)?;
            }
        }
        Ok(())
    }

    fn is_solved(&self) -> bool {
        self.cells.iter().all(|c| c.count() == 1)
    }

    fn solve(&self) -> Option<Self> {
        if self.is_solved() {
            return Some(self.clone());
        }

        let (ix, cell) = self
            .cells
            .iter()
            .enumerate()
            .filter(|(_, c)| c.count() > 1)
            .min_by(|(_, a), (_, b)| a.count().cmp(&b.count()))?;
        for v in cell.values() {
            let mut puzzle = self.clone();
            if puzzle.assign(ix, v).is_err() {
                continue;
            }
            if let Some(solution) = puzzle.solve() {
                return Some(solution);
            }
        }

        None
    }
}

impl std::str::FromStr for Puzzle {
    type Err = Box<dyn Error>;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut puzzle = Self {
            cells: [Cell::with_all(); 9 * 9],
        };

        for (ix, c) in s.chars().enumerate() {
            match c {
                '1'..='9' => puzzle.assign(ix, c.to_digit(10).unwrap() as i32)?,
                '.' | '0' => continue,
                _ => return Err(format!("invalid character in puzzle at {}: {}", ix, c).into()),
            }
        }

        Ok(puzzle)
    }
}

impl std::fmt::Display for Puzzle {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let s = self
            .cells
            .iter()
            .map(|c| match c.solution() {
                Some(v) => v.to_string().chars().next().unwrap(),
                None => '.',
            })
            .collect::<String>();
        write!(f, "{}", s)
    }
}

impl std::fmt::Debug for Puzzle {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut res = String::new();
        let w = self.cells.iter().map(|c| c.count()).max().unwrap();
        let q = "-".repeat(3 * (w + 2));
        let d = format!("\n{}+{}+{}\n", q, q, q);
        for (i, cell) in self.cells.iter().enumerate() {
            let vals = cell
                .values()
                .map(|v| v.to_string())
                .collect::<Vec<_>>()
                .join("");
            let s = format!("{:^width$}", vals, width = w);
            if i == 0 {
            } else if i % 27 == 0 {
                res.push_str(&d);
            } else if i % 9 == 0 {
                res.push('\n');
            } else if i % 3 == 0 {
                res.push('|');
            }
            res.push_str(&format!(" {} ", s));
        }

        write!(f, "{}", res)
    }
}

fn main() -> Result<(), Box<dyn Error>> {
    for line in io::stdin().lock().lines() {
        let line = line?;
        println!("{}", line);
        if let Some(puzzle) = line.parse::<Puzzle>()?.solve() {
            println!("{}", puzzle);
        } else {
            println!("no solution");
        }
        println!();
    }

    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_cell() {
        let c = Cell::with_all();
        assert_eq!(
            vec![1, 2, 3, 4, 5, 6, 7, 8, 9],
            c.values().collect::<Vec<_>>()
        );
        assert_eq!(9, c.count());
        assert_eq!(None, c.solution());
        assert!(!c.is_empty());

        let c = Cell::empty();
        assert_eq!(Vec::<i32>::new(), c.values().collect::<Vec<_>>());
        assert_eq!(0, c.count());
        assert_eq!(None, c.solution());
        assert!(c.is_empty());

        let c = Cell::with_values(&[1, 5, 7]);
        assert_eq!(vec![1, 5, 7], c.values().collect::<Vec<_>>());
        assert_eq!(3, c.count());
        assert_eq!(None, c.solution());
        assert!(!c.is_empty());

        let c = Cell::with_values(&[1, 5, 7]).remove(1).remove(7);
        assert_eq!(vec![5], c.values().collect::<Vec<_>>());
        assert_eq!(1, c.count());
        assert_eq!(Some(5), c.solution());
        assert!(!c.is_empty());

        let c = Cell::with_all().assign(9);
        assert_eq!(vec![9], c.values().collect::<Vec<_>>());
        assert_eq!(1, c.count());
        assert_eq!(Some(9), c.solution());
        assert!(!c.is_empty());
    }
}
