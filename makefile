
# --------------------------------------------------- #
# Kate Strombom
# 2558809
# EECS 665
# --------------------------------------------------- #

# variables

PROG = main
OBJS = main.o nfa.o

INPUT = input.txt
INPUT2 = input_2.txt
INPUT3 = input_3.txt
INPUT4 = input_4.txt
OUTPUT = output.txt

CXX = g++
VERS = -std=c++11
DEBUG = -g
WARN = -Wall
LDFLAGS = $(WARN) $(DEBUG)
CXXFLAGS = -c $(VERS) $(WARN) $(DEBUG)

# --------------------------------------------------- #

# executable
$(PROG) : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(PROG)

# object files
main.o : main.cpp
	$(CXX) $(CXXFLAGS) main.cpp
nfa.o : nfa.cpp
	$(CXX) $(CXXFLAGS) nfa.cpp

# --------------------------------------------------- #

# test
test: $(PROG)
	./$(PROG) < $(INPUT)

# test
test2: $(PROG)
	./$(PROG) < $(INPUT2)
	# test
test3: $(PROG)
	./$(PROG) < $(INPUT3)
# test
test4: $(PROG)
	./$(PROG) < $(INPUT4)

# clean
clean :
	rm -f $(PROG) $(OBJS)
	rm -f $(OUTPUT)
	clear
