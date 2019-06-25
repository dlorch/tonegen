Tone generator
==============

About
-----

This program generates sound.

What is sound? Sound is essentially [air molecules](https://animagraffs.com/loudspeaker/)
travelling away from its source in pockets of high and low pressure. Humans can "hear"
sound, because the ear drums are able to detect the sound waves.

How is sound generated? A [loud speaker](https://animagraffs.com/loudspeaker/) can generate
sound by moving a diaphragm back and forth. The diagram in the loudspeaker is connected to
an electromagnet, which can be moved forward and backwards to create the sound waves in the
air.

How does a computer generate sound? A [digital-to-analog converter (DAC)](https://en.wikipedia.org/wiki/Digital-to-analog_converter),
typically "the sound card", takes sequence of numbers and turn them into eletric current,
which in turn moves the speaker's electromagnet forward or backwards. Suppose I use an
8-bit value for a single a sound sample, then the values 0 and 255 respectively move
[the magnet](https://animagraffs.com/loudspeaker/) from one to the other extremity.

A common format to represent sound waves in digital form is given by
[Pulse Code Modulation (PCM)](https://www.tutorialspoint.com/digital_communication/digital_communication_pulse_code_modulation.htm)
where analog sound waves are simply represented by sequences of numbers.

How to use
----------

Compile and run:

```
$ make
$ ./tonegen
Wrote output/mary.wav
```

Now play back [mary.wav](https://raw.githubusercontent.com/dlorch/tonegen/master/output/mary.wav).

This is how [Audacity](https://www.audacityteam.org/) represents the file.

Section with "pure tone" (sine waves):

![sine waves](https://raw.githubusercontent.com/dlorch/tonegen/master/images/sine.png)

Section with square waves:

![square waves](https://raw.githubusercontent.com/dlorch/tonegen/master/images/square.png)

Section with Attack, Decay, Sustain, Release (ADSR) envelope:

![adsr envelope](https://raw.githubusercontent.com/dlorch/tonegen/master/images/adsr_envelope.png)

Links / Concepts
----------------

* Fundamentals of Sound
    * [Pulse Code Modulation (PCM)](https://www.tutorialspoint.com/digital_communication/digital_communication_pulse_code_modulation.htm)
    * [How speakers make sound - Animagraffs](https://animagraffs.com/loudspeaker/)
    * [Digital to Analog Converter (DAC)](https://en.wikipedia.org/wiki/Digital-to-analog_converter)
    * [Frequencies of Musical Notes](https://pages.mtu.edu/~suits/notefreqs.html)
    * [Scientific pitch notation](https://en.wikipedia.org/wiki/Scientific_pitch_notation)
    * [Audacity](https://www.audacityteam.org/)
* Synthesis of Complex Sounds
    * [Project 1 - Synthesis of Musical Notes and Instrument Sounds with Sinusoids (Qi, 2015)](https://web.eecs.utk.edu/~qi/ece505/project/proj1.pdf)
    * [Overtones, harmonics and Additive synthesis](https://www.youtube.com/watch?v=YsZKvLnf7wU)
    * [Additive synthesis](https://en.wikipedia.org/wiki/Additive_synthesis)
    * [Harmonic](https://en.wikipedia.org/wiki/Harmonic)
    * [Music Meets Science. 1 Pitch & Frequency, 2.Timbre & Complex Wave, 3. Frequency & Harmonics](https://www.youtube.com/watch?v=sWVvm8JPGcU)
    * [FM SYNTHS in under 4 minutes | Andrew Huang](https://www.youtube.com/watch?v=vvBl3YUBUyY)
    * [Frequency modulation synthesis](https://en.wikipedia.org/wiki/Frequency_modulation_synthesis)
    * [Synthesis of Complex Audio Spectra by Means of Frequency Modulation (Chowning, 1973)](https://web.eecs.umich.edu/~fessler/course/100/misc/chowning-73-tso.pdf)
* Envelope
    * [Attack, Decay, Sustain, Release (ADRS) Envelope](https://en.wikipedia.org/wiki/Envelope_(music))
* WAVE File Format
    * [WAVE PCM soundfile format](http://soundfile.sapp.org/doc/WaveFormat/)
    * [portable_endian.h](https://gist.github.com/panzi/6856583)
