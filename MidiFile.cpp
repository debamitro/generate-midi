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
    tracks[0].events.emplace_back(Event(0x00, 0x90, key + (uint8_t)(octave * 12), 0x40));
    tracks[0].events.emplace_back(Event(duration, 0x80, key + (uint8_t)(octave * 12), 0x40));
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
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(1);
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
    file.put(events.size());

    for (const auto& event : events) {
        event.write_to(file);
    }
}

void MidiFile::Event::write_to(std::ofstream& file) const
{
    file.put(delta_);
    file.put(status_);
    file.put(data1_);
    file.put(data2_);
}
