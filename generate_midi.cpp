#include <fstream>

static void generate_midi ();

int main () {
    generate_midi ();

    return 0;
}

static void generate_midi ()
{
    std::ofstream midiFile;
    midiFile.open("out.mid");

    midiFile.write("MThd",4);

    // length
    midiFile.put(0);
    midiFile.put(0);
    midiFile.put(0);
    midiFile.put(6);

    // data
    midiFile.put(0);
    midiFile.put(0);
    midiFile.put(0);
    midiFile.put(1);
    midiFile.put(0);
    midiFile.put(10);

    midiFile.write("MTrk",4);

    // length
    midiFile.put(0);
    midiFile.put(0);
    midiFile.put(0);
    midiFile.put(8);

    midiFile.put(0x00);

    midiFile.put(0x90);
    midiFile.put(0x3C);
    midiFile.put(0x40);

    midiFile.put(0x7F);

    midiFile.put(0x80);
    midiFile.put(0x3C);
    midiFile.put(0x40);

    midiFile.close();
}
