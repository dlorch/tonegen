#ifndef TONEGEN_H
#define TONEGEN_H

#include <climits>

// TODO should consider using static methods only; need to think about what it means for subclassing
class ToneGenerator
{
    public:
        double generate(int toneFrequencyHz, double timeIndex);
};

#include <vector>

class Sampler
{
    private:
        int sampleRateHz;
        int bitsPerSample;
        int numChannels;
        std::vector<char> sampleData;
        Sampler();
    public:
        Sampler(int sampleRateHz, int bitsPerSample, int numChannels);
        void sample(ToneGenerator* generator, int toneFrequencyHz, double durationSecs, double volume);
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
        std::vector<char>& getSampleData(); // TODO should consider returning "const" value
};

// Note names, MIDI numbers and frequencies: https://pages.mtu.edu/~suits/notefreqs.html
// Scientific pitch notation: https://en.wikipedia.org/wiki/Scientific_pitch_notation
enum NoteFrequencies
{
    C0 = 16,
    C0_Sharp, D0_Flat = 17,
    D0 = 18,
    D0_Sharp, E0_Flat = 19,
    E0 = 21,
    F0 = 22,
    F0_Sharp, G0_Flat = 23,
    G0 = 25,
    G0_Sharp, A0_Flat = 26,
    A0 = 28,
    A0_Sharp, B0_Flat = 29,
    B0 = 31,
    C1 = 33,
    C1_Sharp, D1_Flat = 35,
    D1 = 37,
    D1_Sharp, E1_Flat = 39,
    E1 = 41,
    F1 = 44,
    F1_Sharp, G1_Flat = 46,
    G1 = 49,
    G1_Sharp, A1_Flat = 52,
    A1 = 55,
    A1_Sharp, B1_Flat = 58,
    B1 = 62,
    C2 = 65,
    C2_Sharp, D2_Flat = 69,
    D2 = 73,
    D2_Sharp, E2_Flat = 78,
    E2 = 82,
    F2 = 87,
    F2_Sharp, G2_Flat = 93,
    G2 = 98,
    G2_Sharp, A2_Flat = 104,
    A2 = 110,
    A2_Sharp, B2_Flat = 117,
    B2 = 123,
    C3 = 131,
    C3_Sharp, D3_Flat = 139,
    D3 = 147,
    D3_Sharp, E3_Flat = 156,
    E3 = 165,
    F3 = 175,
    F3_Sharp, G3_Flat = 185,
    G3 = 196,
    G3_Sharp, A3_Flat = 208,
    A3 = 220,
    A3_Sharp, B3_Flat = 233,
    B3 = 247,
    C4 = 262,
    C4_Sharp, D4_Flat = 277,
    D4 = 294,
    D4_Sharp, E4_Flat = 311,
    E4 = 330,
    F4 = 349,
    F4_Sharp, G4_Flat = 370,
    G4 = 392,
    G4_Sharp, A4_Flat = 415,
    A4 = 440,
    A4_Sharp, B4_Flat = 466,
    B4 = 494,
    C5 = 523,
    C5_Sharp, D5_Flat = 554,
    D5 = 587,
    D5_Sharp, E5_Flat = 622,
    E5 = 659,
    F5 = 698,
    F5_Sharp, G5_Flat = 740,
    G5 = 784,
    G5_Sharp, A5_Flat = 831,
    A5 = 880,
    A5_Sharp, B5_Flat = 932,
    B5 = 988,
    C6 = 1047,
    C6_Sharp, D6_Flat = 1109,
    D6 = 1175,
    D6_Sharp, E6_Flat = 1245,
    E6 = 1319,
    F6 = 1397,
    F6_Sharp, G6_Flat = 1480,
    G6 = 1568,
    G6_Sharp, A6_Flat = 1661,
    A6 = 1760,
    A6_Sharp, B6_Flat = 1865,
    B6 = 1976,
    C7 = 2093,
    C7_Sharp, D7_Flat = 2217,
    D7 = 2349,
    D7_Sharp, E7_Flat = 2489,
    E7 = 2637,
    F7 = 2794,
    F7_Sharp, G7_Flat = 2960,
    G7 = 3136,
    G7_Sharp, A7_Flat = 3322,
    A7 = 3520,
    A7_Sharp, B7_Flat = 3729,
    B7 = 3951,
    C8 = 4186,
    C8_Sharp, D8_Flat = 4435,
    D8 = 4699,
    D8_Sharp, E8_Flat = 4978,
    E8 = 5274,
    F8 = 5588,
    F8_Sharp, G8_Flat = 5920,
    G8 = 6272,
    G8_Sharp, A8_Flat = 6645,
    A8 = 7040,
    A8_Sharp, B8_Flat = 7459,
    B8 = 7902
};

typedef struct // Resource Interchange File Format (RIFF)
{
    uint32_t ChunkID;
    uint32_t ChunkSize;
    uint32_t Format;
} RIFFHeader;

typedef struct // Format of the sound information in the data sub-chunk
{
    uint32_t Subchunk1ID;
    uint32_t Subchunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
} FmtSubChunk;

typedef struct // Sound data
{
    uint32_t Subchunk2ID;
    uint32_t Subchunk2Size;
} DataSubChunk;

class WAVWriter
{
    public:
        void writeSamplesToBinaryStream(Sampler* sampler, std::ofstream* wavStream);
};

#endif
