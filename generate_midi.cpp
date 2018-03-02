#include <fstream>

class MidiFile
{
public:
    MidiFile (const char * fileName);
    ~MidiFile ();

    void generate();
private:
    void write_header();
    void write_tracks();

    std::ofstream file;
};

int main () {
    MidiFile midiFile("out.mid");
    midiFile.generate();

    return 0;
}

MidiFile::MidiFile (const char * fileName) : file(fileName) {
}

MidiFile::~MidiFile () {
    file.close();
}

void MidiFile::generate () {
    write_header ();
    write_tracks ();
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
    file.write("MTrk",4);

    // length
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(8);

    file.put(0x00);

    file.put(0x90);
    file.put(0x3C);
    file.put(0x40);

    file.put(0x7F);

    file.put(0x80);
    file.put(0x3C);
    file.put(0x40);
}
