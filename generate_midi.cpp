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
        unsigned char delta_;
        unsigned char status_;
        unsigned char data1_;
        unsigned char data2_;

        Event(unsigned char delta,
            unsigned char status,
            unsigned char data1,
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
