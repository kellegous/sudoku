import Foundation

private func quadOf(_ ix: Int) -> Int {
    (ix % 9) / 3 + 3 * (ix / 27)
}

private func colOf(_ ix: Int) -> Int {
    ix % 9
}

private func rowOf(_ ix: Int) -> Int {
    ix / 9
}

private func getPeers() -> [Int] {
    (0..<81).flatMap({(i: Int) in
        let row = rowOf(i)
        let col = colOf(i)
        let quad = quadOf(i)
        return (0..<81).filter({ $0 != i && (row == rowOf($0) || col == colOf($0) || quad == quadOf($0)) })
    })
}

enum PuzzleError: Error {
    case invalidPuzzle(msg: String)
}

struct CellIter : IteratorProtocol {
    typealias Element = Int
    
    private var values: UInt16
    
    init(_ values: UInt16) {
        self.values = values
    }
    
    mutating func next() -> Int? {
        guard values != 0 else { return nil }
        let v = values
        values &= values-1
        return v.trailingZeroBitCount + 1
    }
}

struct Cell: Sequence, Equatable, CustomStringConvertible {
    private let values: UInt16
    
    private init(_ values: UInt16) {
        self.values = values
    }

    func add(_ value: Int) -> Cell {
        Cell(values | (1 << (value - 1)))
    }

    func assign(_ value: Int) -> Cell {
        Cell(1 << (value - 1))
    }

    func remove(_ value: Int) -> Cell {
        Cell(values & ~(1 << (value - 1)))
    }

    var count: Int {
        values.nonzeroBitCount // dang, this uses popcnt instruction!
    }
    
    var isEmpty: Bool {
        count == 0
    }
    
    var solution: Int? {
        (count == 1) ? values.trailingZeroBitCount + 1 : nil
    }

    func makeIterator() -> CellIter {
        CellIter(values)
    }

    var description: String {
        self.map({ $0.description }).joined()
    }
    
    static func withAll() -> Cell {
        Cell(0x1ff)
    }
    
    static func empty() -> Cell {
        Cell(0)
    }
}

class Puzzle: CustomStringConvertible {
    private var cells: Array<Cell>
    
    private static let peers = getPeers()
   
    private init() {
        cells = Array(repeating: Cell.withAll(), count: 81)
    }
    
    init(_ puzzle: Puzzle) {
        cells = puzzle.cells
    }
    
    private func assign(_ value: Int, atIndex index: Int) -> Bool {
        cells[index] = cells[index].assign(value)
        let offset = index * 20
        for i in 0..<20 {
            let peer = Puzzle.peers[offset + i]
            let old = cells[peer]
            let new = old.remove(value)
            if (old == new) {
                continue
            }
            cells[peer] = new
            if new.isEmpty {
                return false
            }
            if let soln = new.solution {
                if !assign(soln, atIndex: peer) {
                    return false
                }
            }
        }
        return true
    }
    
    var isSolved: Bool {
        cells.allSatisfy({ $0.count == 1 })
    }
    
    func solve() -> Puzzle? {
        if isSolved {
            return self
        }
      
        // find the index with the smallest count that is > 1
        let ix = cells.indices.reduce(0, { (a, b) in
            let ac = cells[a].count
            let bc = cells[b].count
            return (ac == 1 || (bc > 1 && bc < ac)) ? b : a
        })
        
        for v in cells[ix] {
            let puzzle = Puzzle(self)
            if puzzle.assign(v, atIndex: ix) {
                if let soln = puzzle.solve() {
                    return soln
                }
            }
        }
        
        return nil
    }
   
    var description: String {
        cells.map({ $0.solution?.description ?? "." }).joined()
    }
   
    func debug() -> String {
        var res = ""
        let w = cells.reduce(0, { max($0, $1.count) })
        let q = String(repeating: "-", count: 3 * (w + 2))
        let d = "\n\(q)+\(q)+\(q)\n"
        for i in 0..<81 {
            let v = cells[i].description
            let a = String(repeating: " ", count: (w - v.count)/2)
            let vals = String(repeating: " ", count: 1 + (w - v.count)/2) + v + String(repeating: " ", count: w - v.count - a.count + 1)
            if i == 0 {
            } else if (i % 27 == 0) {
                res += d
            } else if (i % 9 == 0) {
                res += "\n"
            } else if (i % 3 == 0) {
                res += "|"
            }
            res += vals
        }
        return res
    }
    
    convenience init(_ s: String) throws {
        self.init()
        if s.count != 81 {
            throw PuzzleError.invalidPuzzle(msg: "puzzle must be 81 characters long")
        }
        
        for (i, c) in s.enumerated() {
            switch c {
            case "1"..."9":
                let v = c.wholeNumberValue!
                if !assign(v, atIndex: i) {
                    throw PuzzleError.invalidPuzzle(msg: "conflicting assignment at \(i)")
                }
            case "0",".":
                continue
            default:
                throw PuzzleError.invalidPuzzle(msg: "invalid character at \(i): \(c)")
            }
        }
    }
}

while let line = readLine(strippingNewline: true) {
    print(line)
    let puzzle = try Puzzle(line).solve()
    print(puzzle ?? "no solution")
    print()
}
