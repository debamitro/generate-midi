

all: generate_midi

clean:
	$(RM) generate_midi *.o

generate_midi: generate_midi.cpp
	$(CXX) -Wall -m32 -std=c++11 $< -o $@
