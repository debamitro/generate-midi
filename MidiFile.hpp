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
    static const Note SA = 0x3C;
    static const Note _RE = 0x3D;
    static const Note RE = 0x3E;
    static const Note _GA = 0x3F;
    static const Note GA = 0x40;
    static const Note MA = 0x41;
    static const Note MA_ = 0x42;
    static const Note PA = 0x43;
    static const Note _DHA = 0x44;
    static const Note DHA = 0x45;
    static const Note _NI = 0x46;
    static const Note NI = 0x47;

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

        void write_to(std::ofstream& file) const;
    };

    std::ofstream file;
    std::vector<Track> tracks;
};

#endif
