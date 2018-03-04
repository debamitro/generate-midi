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

static void write_int_as_big_endian (unsigned int n, std::ofstream& file);

void MidiFile::write_header()
{
    file.write("MThd", 4);

    // length
    write_int_as_big_endian (6, file);

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

unsigned int MidiFile::Track::size() const
{
    // 3 bytes for instrument setting event
    // 4 bytes each for every note on or note off event
    // 4 bytes for end of track event
    return 3 + events.size() * 4 + 4;
}


void MidiFile::Track::write_to(std::ofstream& file) const
{
    file.write("MTrk", 4);

    // length
    write_int_as_big_endian(size(), file);

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

static void write_int_as_big_endian (const unsigned int n, std::ofstream& file)
{
    file.put((n >> 24) & 0xFF);
    file.put((n >> 16) & 0xFF);
    file.put((n >> 8) & 0xFF);
    file.put(n & 0xFF);
}
