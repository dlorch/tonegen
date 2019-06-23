/*
    Tone generator

    BSD 2-Clause License

    Copyright (c) 2019, Daniel Lorch
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <climits>
#include "tonegen.h"
#include "portable_endian.h"

// the tone generator returns a continous result between [-1.0, 1.0]
double PureToneGenerator::generate(int toneFrequencyHz, double timeIndex)
{
    double tonePeriodSeconds = 1.0 / toneFrequencyHz;
    double radians = timeIndex / tonePeriodSeconds * (2 * M_PI);
    double result = sin(radians);

    return result;
}

Sampler::Sampler(int sampleRateHz, int bitsPerSample, int numChannels): sampleRateHz(sampleRateHz), bitsPerSample(bitsPerSample), numChannels(numChannels)
{
    if(numChannels != 1)
        throw std::logic_error("Unsupported value for numChannels: only 1 channel (mono) supported");

    if(bitsPerSample != 8)
        throw std::logic_error("Unsupported value for bitsPerSample: only 8 bits supported");
}

void Sampler::sample(ToneGenerator* generator, int toneFrequencyHz, double durationSecs, double volume)
{
    if(volume == 11) // loudest
        volume = 1.0;
    else if(volume < 0 || volume > 1)
        throw std::logic_error("Invalid volume: must be within range 0.0 .. 1.0");

    const double sampleValueRange = pow(2, this->bitsPerSample);

    for(int i=0; i < this->sampleRateHz * durationSecs; i++) {
        double timeIndex = (double)i / this->sampleRateHz;
        
        // map continous result from tone generator [-1.0, 1.0] to discrete
        // sample value range [0 .. 255] and also adjust the volume
        char sampleResult = (generator->generate(toneFrequencyHz, timeIndex) * volume + 1.0) / 2.0 * sampleValueRange;
     
        this->sampleData.push_back(sampleResult);
    }

    // TODO: should consider to "smoothen" the transition between samples to avoid the clicking noises
}

int Sampler::getSampleRateHz()
{
    return this->sampleRateHz;
}

int Sampler::getBitsPerSample()
{
    return this->bitsPerSample;
}

int Sampler::getNumChannels()
{
    return this->numChannels;
}

std::vector<char>& Sampler::getSampleData()
{
    return this->sampleData;
}

// WAVE Format: http://soundfile.sapp.org/doc/WaveFormat/
void WAVWriter::writeSamplesToBinaryStream(Sampler *sampler, std::ofstream *wavStream)
{
    DataSubChunk dataSubChunk;
    dataSubChunk.Subchunk2ID   = htobe32(0x64617461); // "data"
    dataSubChunk.Subchunk2Size = htole32(sampler->getSampleData().size() * sampler->getNumChannels() * sampler->getBitsPerSample()/8);

    // WTF MSFT: mixed big- and little endian in the *same* header structs? You've got to be kidding me...

    FmtSubChunk fmtSubChunk;
    fmtSubChunk.Subchunk1ID   = htobe32(0x666d7420); // "fmt "
    fmtSubChunk.Subchunk1Size = htole32(16);         // size of the rest of this subchunk
    fmtSubChunk.AudioFormat   = htole32(1);          // PCM (i.e. linear quantization)
    fmtSubChunk.NumChannels   = htole32(sampler->getNumChannels());
    fmtSubChunk.SampleRate    = htole32(sampler->getSampleRateHz());
    fmtSubChunk.ByteRate      = htole32(sampler->getSampleRateHz() * sampler->getNumChannels() * sampler->getBitsPerSample()/8);
    fmtSubChunk.BlockAlign    = htole32(sampler->getNumChannels() * sampler->getBitsPerSample()/8);
    fmtSubChunk.BitsPerSample = htole32(sampler->getBitsPerSample());

    RIFFHeader riffHeader;
    riffHeader.ChunkID   = htobe32(0x52494646); // "RIFF"
    riffHeader.ChunkSize = htole32(4 + (8 + fmtSubChunk.Subchunk1Size) + (8 + dataSubChunk.Subchunk2Size));
    riffHeader.Format    = htobe32(0x57415645); // "WAVE"

    wavStream->write((char *)&riffHeader, sizeof(riffHeader));
    wavStream->write((char *)&fmtSubChunk, sizeof(fmtSubChunk));
    wavStream->write((char *)&dataSubChunk, sizeof(dataSubChunk));
    // C++ apparently guarantees, that the first element of a vector points to consecutive memory of the data
    wavStream->write((char *)&sampler->getSampleData()[0], sizeof(char)*sampler->getSampleData().size());
}

int main() {
    const int sampleRateHz  = 22050;    // number of samples per second
    const int numChannels   = 1;        // Mono
    const int bitsPerSample = CHAR_BIT; // 8 bits 
    const double volume     = 0.75;     // 0.0 .. 1.0

    PureToneGenerator tone = PureToneGenerator();
    Sampler sampler        = Sampler(sampleRateHz, bitsPerSample, numChannels);

    // Mary had a Little Lamb: http://www.choose-piano-lessons.com/piano-notes.html
    const int marySong[] =
    {
    //  Ma-----ry    had      a     lit----le    lamb
        E4,    D4,    C4,    D4,    E4,    E4,    E4,
    //  lit----le    lamb,   lit----le    lamb
        D4,    D4,    D4,    E4,    E4,    E4,
    //  Ma-----ry    had      a     lit----le    lamb
        E4,    D4,    C4,    D4,    E4,    E4,    E4,
    //  Its  fleece  was    white   as    snow.
        E4,    D4,    D4,    E4,    D4,    C4
    };

    const int maryLength = sizeof(marySong)/sizeof(marySong[0]);

    for(int i=0; i<maryLength; i++)
    {
        sampler.sample(&tone, marySong[i], 0.25, volume);
    }

    std::ofstream maryFile("mary.wav", std::ios::out | std::ios::binary);
    WAVWriter wavWriter = WAVWriter();
    wavWriter.writeSamplesToBinaryStream(&sampler, &maryFile);
    maryFile.close();
    std::cout << "Wrote mary.wav" << std::endl;

    return 0;
}
