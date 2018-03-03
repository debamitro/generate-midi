#include "MidiFile.hpp"

int main()
{
    MidiFile midiFile("out.mid");
    midiFile.add_note(MidiFile::NI, -1);
    midiFile.add_note(MidiFile::RE);
    midiFile.add_note(MidiFile::GA);
    midiFile.add_note(MidiFile::SA, 0, MidiFile::QUARTER);
    midiFile.add_note(MidiFile::NI, -1, MidiFile::QUARTER);
    midiFile.add_note(MidiFile::RE, 0, MidiFile::QUARTER);
    midiFile.add_note(MidiFile::SA, 0, MidiFile::ONE * (uint8_t)2);
    midiFile.generate();

    return 0;
}
