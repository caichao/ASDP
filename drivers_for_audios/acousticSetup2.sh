#!/bin/sh

echo "make sure you
  (1) already rebooted after acousticSetup.sh
  (2) connect the acoustic board
  "
read -p "press <enter> to continue..." dummy

##amixer set 'Master' 127 # 6dB, 100
##amixer set 'Master' 125 # 4dB, 93
amixer set 'Master' 116 # -5dB, 66
amixer set 'Master Playback ZC' mute
amixer set 'Sidetone' 0%
amixer set 'Line' 0%,0% mute nocap
amixer set 'Mic' cap
amixer set 'Mic Boost' 0%
amixer set 'Playback Deemphasis' mute
amixer set 'Capture' 100%
amixer set 'ADC High Pass Filter' unmute
amixer set 'Input Mux' 'Mic'
amixer set 'Output Mixer HiFi' unmute
amixer set 'Output Mixer Line Bypass' mute
amixer set 'Output Mixer Mic Sidetone' mute
amixer set 'Store DC Offset' mute

# update /var/lib/alsa/asound.state, or, the file will be updated after rebooting
sudo alsactl store

# to change alsamixer setting according to asound.state, do:
## sudo alsactl restore
