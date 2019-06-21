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
Wrote mary.wav
```

Now play back [mary.wav](https://raw.githubusercontent.com/dlorch/tonegen/master/mary.wav).

This is how [Audacity](https://www.audacityteam.org/) represents the generate file:

![Audacity showing mary.wav](https://raw.githubusercontent.com/dlorch/tonegen/master/mary.png)

Concepts
--------

* Pulse Code Modulation (PCM)
* WAVE file format
* Musical notes

Links
-----

* [Pulse Code Modulation](https://www.tutorialspoint.com/digital_communication/digital_communication_pulse_code_modulation.htm)
* [WAVE PCM soundfile format](http://soundfile.sapp.org/doc/WaveFormat/)
* [How speakers make sound - Animagraffs](https://animagraffs.com/loudspeaker/)
* [Digital to Analog Converter (DAC)](https://en.wikipedia.org/wiki/Digital-to-analog_converter)
* [portable_endian.h](https://gist.github.com/panzi/6856583)
* [Frequencies of Musical Notes](https://pages.mtu.edu/~suits/notefreqs.html)
* [Scientific pitch notation](https://en.wikipedia.org/wiki/Scientific_pitch_notation)
* [Audacity](https://www.audacityteam.org/)
