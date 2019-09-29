#!/bin/sh

#======================================================================
# packages
#======================================================================
##sudo apt-get install vim mplayer



#======================================================================
# unnecessary for either:
#   "2016-09-23-raspbian-jessie.img" / "NOOBS_v1_9_3.zip" / (4.4.21-v7+)
# or
#   "2016-05-27-raspbian-jessie.img" / "NOOBS_v1_9_2.zip" / (4.4.11-v7+)
#======================================================================
### sudo apt-get update
### sudo apt-get upgrade
### sudo rpi-update



#======================================================================
# /boot/config.txt
#======================================================================
sudo sh -c "cat >> /boot/config.txt <<-____________________

#======================================================================
# added by users $(date +%Y%m%d_%H%M)
#======================================================================
# memory split:
gpu_mem=16
# enable i2c:
dtparam=i2c_arm=on
# enable spi:
dtparam=spi=on
# enable i2s:
dtparam=i2s=on
# i2s / DAC driver:
dtoverlay=i2s-mmap
dtoverlay=rpi-proto
### #the following 7 lines is unnecessary
### hdmi_force_hotplug=1
### config_hdmi_boost=4
### overscan_left=24
### overscan_right=24
### overscan_top=16
### overscan_bottom=16
### disable_overscan=0
____________________"

:<<BLOCK_COMMENT
sudo sh -c "cat >> /boot/config.txt <<-____________________
____________________"
BLOCK_COMMENT



#======================================================================
# /etc/modules
#======================================================================
sudo sh -c "cat >> /etc/modules <<-____________________

#======================================================================
# added by users $(date +%Y%m%d_%H%M)
#======================================================================
snd_soc_bcm2708
snd_soc_bcm2708_i2s
bcm2708_dmaengine
snd_soc_wm8731
snd_soc_rpi_proto
SND_BCM2708_SOC_I2S
SND_SOC_GENERIC_DMAENGINE_PCM
SND_SOC_WM8731
SND_BCM2708_SOC_RPI_PROTO
____________________"

:<<BLOCK_COMMENT
sudo sh -c "cat >> /etc/modules <<-____________________
____________________"
BLOCK_COMMENT



#======================================================================
# /etc/asound.conf
# not sure whether this is necessary
#======================================================================
sudo sh -c "cat >> /etc/asound.conf <<-____________________

#======================================================================
# added by users $(date +%Y%m%d_%H%M)
#======================================================================
pcm.!default  {
type hw card 0
}
ctl.!default {
type hw card 0
}
____________________"



#======================================================================
# /etc/modprobe.d/alsa-blacklist.conf
#======================================================================
sudo sh -c "cat >> /etc/modprobe.d/alsa-blacklist.conf <<-____________________

#======================================================================
# added by users $(date +%Y%m%d_%H%M)
#======================================================================
blacklist snd_bcm2835
____________________"
