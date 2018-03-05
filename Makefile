

all: generate_midi

clean:
	$(RM) generate_midi *.o out.mid

%.o: %.cpp
	$(CXX) -c -Wall -Wextra -m32 -std=c++11 $< -o $@

generate_midi: generate_midi.o MidiFile.o
	$(CXX) -Wall -Wextra -m32 $^ -o $@
