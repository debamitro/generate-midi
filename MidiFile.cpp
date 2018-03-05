#include "MidiFile.hpp"
#include <cstdint>
#include <fstream>
#include <vector>

MidiFile::MidiFile(const char* fileName, const int numTracks)
    : numTracks_(numTracks),
      file(fileName)
    , tracks(numTracks)
{
}

void MidiFile::generate()
{
    write_header();
    write_tracks();
}

void MidiFile::close()
{
    file.close();
}

void MidiFile::add_note(const Note key,
                        const int octave,
                        const Duration duration,
                        const int trackNumber
    )
{
    const Note adjustedKey = key + (Note)(octave * 12);
    // note on event
    tracks[trackNumber-1].events.emplace_back(
        Event(0x00, 0x90, adjustedKey, 0x60));
    // note off event
    tracks[trackNumber-1].events.emplace_back(
        Event(duration & 0x7F, 0x80, adjustedKey, 0x20));
}

static void write_int_as_big_endian(unsigned int n, std::ofstream& file);

void MidiFile::write_header()
{
    file.write("MThd", 4);

    // length
    write_int_as_big_endian(6, file);

    // data
    // format - 2 bytes
    file.put(0);
    file.put(0);
    // tracks - 2 bytes
    file.put(0);
    file.put(numTracks_);
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

static void write_int_as_big_endian(const unsigned int n, std::ofstream& file)
{
    file.put((n >> 24) & 0xFF);
    file.put((n >> 16) & 0xFF);
    file.put((n >> 8) & 0xFF);
    file.put(n & 0xFF);
}
