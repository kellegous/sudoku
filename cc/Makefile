OBJS := main.o cell.o puzzle.o util.o
TPLS := result.h

sudoku: $(OBJS)
	g++ -O3 -Wall -std=c++20 -o $@ $^

%.o: %.cc %.h $(TPLS)
	g++ -O3 -Wall -std=c++20 -c $< -o $@

clean:
	rm -f $(OBJS) sudoku