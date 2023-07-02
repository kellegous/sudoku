OBJS := main.o cell.o puzzle.o status.o util.o

sudoku: $(OBJS)
	g++ -std=c++20 -o $@ $^

%.o: %.cc %.h
	g++ -std=c++20 -c $< -o $@

clean:
	rm -f $(OBJS) sudoku