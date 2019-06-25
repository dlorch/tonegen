Convert from .wav to .mp3

``
sox bells.wav bells.mp3
``

Create visualization

``
ffmpeg -i bells.mp3 -filter_complex showwaves=s=320x240,mode=p2p -y -acodec copy bells.mp4
``
