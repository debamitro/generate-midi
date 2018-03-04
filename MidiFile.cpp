#include "MidiFile.hpp"
#include <cstdint>
#include <fstream>
#include <vector>

MidiFile::MidiFile(const char* fileName)
    : file(fileName)
    , tracks(1)
{
}

MidiFile::~MidiFile()
{
    file.close();
}

void MidiFile::generate()
{
    write_header();
    write_tracks();
}

void MidiFile::add_note(const Note key, const int octave, const Duration duration)
{
    tracks[0].events.emplace_back(Event(0x00, 0x90, key + (uint8_t)(octave * 12), 0x60));
    tracks[0].events.emplace_back(Event(duration, 0x80, key + (uint8_t)(octave * 12), 0x20));
}

void MidiFile::write_header()
{
    file.write("MThd", 4);

    // length
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(6);

    // data
    // format - 2 bytes
    file.put(0);
    file.put(0);
    // tracks - 2 bytes
    file.put(0);
    file.put(1);
    // division - 2 bytes
    file.put(0);
    file.put(10);
}

void MidiFile::write_tracks()
{
    for (const auto& track : tracks) {
        track.write_to(file);
    }
}

void MidiFile::Track::write_to(std::ofstream& file) const
{
    file.write("MTrk", 4);

    // length
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(3+events.size()*4+4);

    file.put(0x00);
    file.put(0xC0);
    file.put(0x00);

    for (const auto& event : events) {
        event.write_to(file);
    }

    // end of track
    file.put(0x00);
    file.put(0xFF);
    file.put(0x2F);
    file.put(0x00);
}

void MidiFile::Event::write_to(std::ofstream& file) const
{
    file.put(delta_);
    file.put(status_);
    file.put(data1_);
    file.put(data2_);
}
