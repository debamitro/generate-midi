#include <fstream>
#include <vector>

class MidiFile
{
public:
    MidiFile (const char * fileName);
    ~MidiFile ();

    void add_note(unsigned char key, unsigned char duration = 0x08);
    void generate();
private:
    void write_header();
    void write_tracks();

    struct Event {
        unsigned char delta_;
        unsigned char status_;
        unsigned char data1_;
        unsigned char data2_;

        Event (unsigned char delta,
               unsigned char status,
               unsigned char data1,
               unsigned char data2) :
            delta_(delta),
            status_(status),
            data1_(data1),
            data2_(data2) { }
        void write_to (std::ofstream & file) const;
    };

    struct Track {
        int length;
        std::vector<Event> events;

        void write_to (std::ofstream & file) const;
    };

    std::ofstream file;
    std::vector<Track> tracks;
};

int main () {
    MidiFile midiFile("out.mid");
    midiFile.add_note(0x3B);
    midiFile.add_note(0x3E);
    midiFile.add_note(0x40);
    midiFile.add_note(0x3C,0x02);
    midiFile.add_note(0x3B,0x02);
    midiFile.add_note(0x3E,0x02);
    midiFile.add_note(0x3C,0x10);
    midiFile.generate();

    return 0;
}

MidiFile::MidiFile (const char * fileName) : file(fileName), tracks(1) {
}

MidiFile::~MidiFile () {
    file.close();
}

void MidiFile::generate () {
    write_header ();
    write_tracks ();
}

void MidiFile::add_note(const unsigned char key, const unsigned char duration) {
    tracks[0].events.emplace_back(Event(0x00,0x90,key,0x40));
    tracks[0].events.emplace_back(Event(duration,0x80,key,0x40));
}

void MidiFile::write_header() {
    file.write("MThd",4);

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

void MidiFile::write_tracks() {
    for (const auto & track : tracks) {
        track.write_to(file);
    }
}

void MidiFile::Track::write_to(std::ofstream & file) const {
    file.write("MTrk",4);

    // length
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(events.size());

    for (const auto & event : events) {
        event.write_to(file);
    }
}

void MidiFile::Event::write_to(std::ofstream & file) const {
    file.put(delta_);
    file.put(status_);
    file.put(data1_);
    file.put(data2_);
}
