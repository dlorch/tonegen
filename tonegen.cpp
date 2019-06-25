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

double PureToneGenerator::generate(int toneFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    double tonePeriodSeconds = 1.0 / toneFrequencyHz;
    double radians = timeIndexSeconds / tonePeriodSeconds * (2 * M_PI);
    double result = sin(radians);

    return result;
}

// Square Wave is generated by adding odd-numbered harmonics with decreasing amplitude https://youtu.be/YsZKvLnf7wU?t=363
double SquareWaveGenerator::generate(int fundamentalFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    double firstHarmonicPeriodSeconds = 1.0 / fundamentalFrequencyHz;
    double firstHarmonicRadians = timeIndexSeconds / firstHarmonicPeriodSeconds * (2 * M_PI);

    double thirdHarmonicPeriodSeconds = 1.0 / (fundamentalFrequencyHz * 3);
    double thirdHarmonicRadians = timeIndexSeconds / thirdHarmonicPeriodSeconds * (2 * M_PI);
    double thirdHarmonicAmplitude = 1.0 / 3.0;

    double fifthHarmonicPeriodSeconds = 1.0 / (fundamentalFrequencyHz * 5);
    double fifthHarmonicRadians = timeIndexSeconds / fifthHarmonicPeriodSeconds * (2 * M_PI);
    double fifthHarmonicAmplitude = 1.0 / 5.0;

    double seventhHarmonicPeriodSeconds = 1.0 / (fundamentalFrequencyHz * 7);
    double seventhHarmonicRadians = timeIndexSeconds / seventhHarmonicPeriodSeconds * (2 * M_PI);
    double seventhHarmonicAmplitude = 1.0 / 7.0;

    double ninthHarmonicPeriodSeconds = 1.0 / (fundamentalFrequencyHz * 9);
    double ninthHarmonicRadians = timeIndexSeconds / ninthHarmonicPeriodSeconds * (2 * M_PI);
    double ninthHarmonicAmplitude = 1.0 / 9.0;

    // ... continue to infinite

    double result = sin(firstHarmonicRadians) \
                  + thirdHarmonicAmplitude   * sin(thirdHarmonicRadians) \
                  + fifthHarmonicAmplitude   * sin(fifthHarmonicRadians) \
                  + seventhHarmonicAmplitude * sin(seventhHarmonicRadians) \
                  + ninthHarmonicAmplitude   * sin(ninthHarmonicRadians);

    return result;
}

// Violin sound https://meettechniek.info/additional/additive-synthesis.html
double ViolinGenerator::generate(int fundamentalFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    double amplitude = 0.49;

    double harmonic1PeriodSeconds = 1.0 / fundamentalFrequencyHz;
    double harmonic1Radians = timeIndexSeconds / harmonic1PeriodSeconds * (2 * M_PI);
    double harmonic1Amplitude = 0.995;

    double harmonic2PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 2);
    double harmonic2Radians = timeIndexSeconds / harmonic2PeriodSeconds * (2 * M_PI);
    double harmonic2Amplitude = 0.940;

    double harmonic3PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 3);
    double harmonic3Radians = timeIndexSeconds / harmonic3PeriodSeconds * (2 * M_PI);
    double harmonic3Amplitude = 0.425;

    double harmonic4PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 4);
    double harmonic4Radians = timeIndexSeconds / harmonic4PeriodSeconds * (2 * M_PI);
    double harmonic4Amplitude = 0.480;

    double harmonic6PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 6);
    double harmonic6Radians = timeIndexSeconds / harmonic4PeriodSeconds * (2 * M_PI);
    double harmonic6Amplitude = 0.365;

    double harmonic7PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 7);
    double harmonic7Radians = timeIndexSeconds / harmonic7PeriodSeconds * (2 * M_PI);
    double harmonic7Amplitude = 0.040;

    double harmonic8PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 8);
    double harmonic8Radians = timeIndexSeconds / harmonic8PeriodSeconds * (2 * M_PI);
    double harmonic8Amplitude = 0.085;

    double harmonic10PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 10);
    double harmonic10Radians = timeIndexSeconds / harmonic10PeriodSeconds * (2 * M_PI);
    double harmonic10Amplitude = 0.090;

    double result = amplitude *
                  ( harmonic1Amplitude  * sin(harmonic1Radians)
                  + harmonic2Amplitude  * cos(harmonic2Radians)
                  + harmonic3Amplitude  * sin(harmonic3Radians)
                  + harmonic4Amplitude  * cos(harmonic4Radians)
                  + harmonic6Amplitude  * cos(harmonic6Radians)
                  + harmonic7Amplitude  * sin(harmonic6Radians)
                  + harmonic8Amplitude  * cos(harmonic8Radians)
                  + harmonic10Amplitude * cos(harmonic10Radians) );

    return result;
}

double ChirpGenerator::generate(int initialFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    int finalFrequencyHz = initialFrequencyHz * 10;

    // modulate the frequency with time, linearly decreasing from initialFrequencyHz to finalFrequencyHz
    int momentaryFrequencyHz = initialFrequencyHz + (double)finalFrequencyHz / durationSeconds * timeIndexSeconds;

    double tonePeriodSeconds = 1.0 / momentaryFrequencyHz;
    double radians = timeIndexSeconds / tonePeriodSeconds * (2 * M_PI);
    double result = sin(radians);

    return result;
}

double NoEnvelope::getAmplitude(double timeIndexSeconds)
{
    return 1.0;
}

ADSREnvelope::ADSREnvelope(double durationSeconds)
{
    if(durationSeconds <= 0.0)
        throw std::logic_error("Invalid value for durationSeconds: must be positive non-zero value");

    this->attackAmplitude       = 1.0;
    this->attackDurationSeconds = durationSeconds * 0.1;

    // decay amplitude range is given by attack and sustain
    this->decayDurationSeconds = durationSeconds * 0.1;

    this->sustainAmplitude       = 0.7;
    this->sustainDurationSeconds = durationSeconds * 0.7;

    this->releaseAmplitude       = 0.0;
    this->releaseDurationSeconds = durationSeconds * 0.1;
}

double ADSREnvelope::getAmplitude(double timeIndexSeconds)
{
    double result;

    if(timeIndexSeconds < this->attackDurationSeconds)
    {
        // linearly increasing
        result = this->attackAmplitude / this->attackDurationSeconds * timeIndexSeconds;
    }
    else if(timeIndexSeconds < this->attackDurationSeconds + this->decayDurationSeconds)
    {
        double relativeTimeIndexSeconds = timeIndexSeconds - this->attackDurationSeconds;
        double relativeAmplitude = this->attackAmplitude - this->sustainAmplitude;

        // linearly decreasing
        result = this->attackAmplitude - (relativeAmplitude / this->decayDurationSeconds * relativeTimeIndexSeconds);
    }
    else if(timeIndexSeconds < this->attackDurationSeconds + this->decayDurationSeconds + this->sustainDurationSeconds)
    {
        double relativeTimeIndexSeconds = timeIndexSeconds - this->attackDurationSeconds - this->decayDurationSeconds;

        // keep at same level
        result = this->sustainAmplitude;
    }
    else
    {
        double relativeTimeIndexSeconds = timeIndexSeconds - this->attackDurationSeconds - this->decayDurationSeconds - this->sustainDurationSeconds;

        // linearly decreasing
        result = this->sustainAmplitude - (this->sustainAmplitude / this->releaseDurationSeconds * relativeTimeIndexSeconds);
    }

    return result;
}

Sampler::Sampler(int sampleRateHz, int bitsPerSample, int numChannels): sampleRateHz(sampleRateHz), bitsPerSample(bitsPerSample), numChannels(numChannels)
{
    if(numChannels != 1)
        throw std::logic_error("Unsupported value for numChannels: only 1 channel (mono) supported");

    if(bitsPerSample != 8)
        throw std::logic_error("Unsupported value for bitsPerSample: only 8 bits supported");
}

void Sampler::sample(ToneGenerator* generator, int toneFrequencyHz, double durationSeconds, Envelope* envelope, double volume)
{
    if(volume == 11) // loudest
        volume = 1.0;
    else if(volume < 0 || volume > 1)
        throw std::logic_error("Invalid volume: must be within range 0.0 .. 1.0");

    const double sampleValueRange = pow(2, this->bitsPerSample);

    for(int i=0; i < this->sampleRateHz * durationSeconds; i++) {
        double timeIndexSeconds = (double)i / this->sampleRateHz;
        double sample = generator->generate(toneFrequencyHz, timeIndexSeconds, durationSeconds);
        
        // apply envelope
        sample = sample * envelope->getAmplitude(timeIndexSeconds);

        // apply volume
        sample = sample * volume;

        // map continous result from tone generator [-1.0, 1.0] to discrete sample value range [0 .. 255]
        char sampleValue = (sample + 1.0) / 2.0 * sampleValueRange;
        this->sampleData.push_back(sampleValue);
    }
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
    const int sampleRateHz    = 22050;    // number of samples per second
    const int numChannels     = 1;        // Mono
    const int bitsPerSample   = CHAR_BIT; // 8 bits
    const double volume       = 0.75;     // 0.0 .. 1.0
    const double noteDuration = 0.25;     // seconds

    PureToneGenerator pureTone     = PureToneGenerator();
    SquareWaveGenerator squareWave = SquareWaveGenerator();
    ViolinGenerator violin         = ViolinGenerator();
    ChirpGenerator chirp           = ChirpGenerator();

    NoEnvelope noEnvelope     = NoEnvelope();
    ADSREnvelope adsrEnvelope = ADSREnvelope(noteDuration);

    Sampler sampler = Sampler(sampleRateHz, bitsPerSample, numChannels);

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

    // pure, sinusoidal tone; no envelope
    for(int i=0; i<maryLength; i++)
    {
        sampler.sample(&pureTone, marySong[i], noteDuration, &noEnvelope, volume);
    }

    // square waves; no envelope
    for(int i=0; i<maryLength; i++)
    {
        sampler.sample(&squareWave, marySong[i], noteDuration, &noEnvelope, volume);
    }

    // square waves; ADSR envelope
    for(int i=0; i<maryLength; i++)
    {
        sampler.sample(&squareWave, marySong[i], noteDuration, &adsrEnvelope, volume);
    }

    // violin; ADSR envelope
    for(int i=0; i<maryLength; i++)
    {
        sampler.sample(&violin, marySong[i], noteDuration, &adsrEnvelope, volume);
    }

    sampler.sample(&chirp, C4, noteDuration, &adsrEnvelope, volume);
    sampler.sample(&chirp, C4, noteDuration, &adsrEnvelope, volume);
    sampler.sample(&chirp, C4, noteDuration, &adsrEnvelope, volume);

    std::ofstream maryFile("output/mary.wav", std::ios::out | std::ios::binary);
    WAVWriter::writeSamplesToBinaryStream(&sampler, &maryFile);
    maryFile.close();
    std::cout << "Wrote output/mary.wav" << std::endl;

    return 0;
}
