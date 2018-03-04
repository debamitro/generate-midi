#ifndef MIDIFILE_HPP
#define MIDIFILE_HPP

#include <cstdint>
#include <fstream>
#include <vector>

class MidiFile {
public:
    MidiFile(const char* fileName);
    ~MidiFile();

    using Note = uint8_t;
    static const Note SA = 0x3D; // Middle C-sharp
    static const Note _RE = SA + 1;
    static const Note RE = SA + 2;
    static const Note _GA = SA + 3;
    static const Note GA = SA + 4;
    static const Note MA = SA + 5;
    static const Note MA_ = SA + 6;
    static const Note PA = SA + 7;
    static const Note _DHA = SA + 8;
    static const Note DHA = SA + 9;
    static const Note _NI = SA + 10;
    static const Note NI = SA + 11;

    using Duration = uint8_t;
    static const Duration QUARTER = 0x02;
    static const Duration HALF = 0x04;
    static const Duration ONE = 0x08;

    void add_note(Note key, int octave = 0, Duration duration = ONE);
    void generate();

private:
    void write_header();
    void write_tracks();

    struct Event {
        Duration delta_;
        unsigned char status_;
        Note data1_;
        unsigned char data2_;

        Event(Duration delta,
            unsigned char status,
            Note data1,
            unsigned char data2)
            : delta_(delta)
            , status_(status)
            , data1_(data1)
            , data2_(data2)
        {
        }
        void write_to(std::ofstream& file) const;
    };

    struct Track {
        int length;
        std::vector<Event> events;

        unsigned int size() const;
        void write_to(std::ofstream& file) const;
    };

    std::ofstream file;
    std::vector<Track> tracks;
};

#endif
