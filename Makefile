CXX=g++
CXXFLAGS=-g -Wall -std=c++11

all: counting 

counting: counting.cpp hashtable.cpp
	$(CXX) $(CXXFLAGS) counting.cpp hashtable.cpp -o counting


clean:
	rm -f *.o all