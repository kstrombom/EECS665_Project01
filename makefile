
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

# clean
clean :
	rm -f $(PROG) $(OBJS)
	rm -f $(OUTPUT)
	clear
