import XCTest

@testable import sudoku

final class CellTests: XCTestCase {
    
    func testAllValues() {
        let c = Cell.withAll()
        XCTAssertEqual(Array(c), [1, 2, 3, 4, 5, 6, 7, 8, 9])
        XCTAssertEqual(9, c.count)
    }
    
    func testEmpty() {
        let c = Cell.empty()
        XCTAssertEqual(Array(c), [])
        XCTAssertEqual(0, c.count)
    }
    
    func testAdd() {
        XCTAssertEqual(Array(Cell.empty().add(1)), [1])
        XCTAssertEqual(Array(Cell.withAll().add(1)), [1, 2, 3, 4, 5, 6, 7, 8, 9])
        XCTAssertEqual(Array(Cell.empty().add(2).add(9)), [2, 9])
    }
    
    func testRemove() {
        XCTAssertEqual(Array(Cell.withAll().remove(3)), [1, 2, 4, 5, 6, 7, 8, 9])
        XCTAssertEqual(Array(Cell.empty().remove(4)), [])
        XCTAssertEqual(Array(Cell.withAll().remove(9).remove(7)), [1, 2, 3, 4, 5, 6, 8])
    }
    
    func testAssign() {
        XCTAssertEqual(Array(Cell.withAll().assign(5)), [5])
        XCTAssertEqual(Array(Cell.empty().assign(6)), [6])
    }
    
    func testEquality() {
        XCTAssertEqual(Cell.empty().add(1), Cell.withAll().assign(1))
        XCTAssertNotEqual(Cell.empty(), Cell.withAll())
    }
}
