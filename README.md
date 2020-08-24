# ASDR
We present an Acoustic Software Defined Radios platform. 
ASDR is a versatile communication and sensing platform that offers extreme ﬂexibility for researchers fast prototyping and deploying acoustic sensing applications. Like SDR in RF domain, ASDR can help researchers to inspect physical layer details in an acoustic sensing system and even provide plug-and-play hardware ﬂexibility to incorporate new modules. One can enjoy abundant APIs provided by ASDR to develop new and interesting applications, reducing the efforts on platform details, or exploit the ﬂexible conﬁgurations of platform-speciﬁc features to give guidance for future acoustic management design both in hardware and software architecture. Also, we have demonstrated various interesting applications on our ASDR platform.

ASDR aims to tackle the following issues that were not addressed by previous work:

- **Lacking full exploitation of acoustic modules**. Existing work only provides mono channel measurements, which is mostly sampled from the bottom microphone on smartphones. This microphone is purposefully optimized for the audible frequency bandwidth below 8 kHz [1] and suffers from severe distortions on higher frequency band. The microphone on the rear or back of smartphones are not utilized. This microphone actually performs better than the bottom one in the preferable inaudible frequency range. However, no APIs are provided to extract audio samples from the rear microphone. Meanwhile, the ability to simultaneously record stereo samples is also under utilization.
- **Difficult to fine-tune hardware configurations** Existing platforms are data centric and do not focus on hardware. Thus they do not provide APIs to flexibly configure the hardware for specific tasks. For instance, It is not feasible to disable the Automatic Gain Control (AGC) on the dual channels. And it is labor intensive to disable the AGC on either channel since one has to compile platform-dependent kernel drivers for the acoustic modules. In addition, when prototyping special applications that need kernel hacking, one has to resolve lots of OS-associated issues (e.g., root the device) before reaching the kernel, incurring extra burden in development.
- **Compatible issues** When the host operation systems (OS) or software development tookit (SDK) undergo dramatic changes, compatible issues will arise. That is, existing platform will encounter failures when there are system upgrades. Meanwhile, existing utilities [2][3] that provide configurable abilities to low-lay settings could easily fail to work across different platforms since the acoustic modules on commodity devices have sharp differences on hardware design especially those on the Android platforms, creating a significant burden on building a versatile utility.
- **Fixed physical layout of acoustic sensors** The acoustic sensors on commodity devices are mostly covered by a shell, leaving only a single hole for the signals to travel through. This fixed physical layout for multiple sensors hampers much flexibility in deploying systems that needs stereo recording. In addition, shielding the sensors raise the risk of introducing non-line-of-sight reflections.

The above challenges make it difficult to build cross-platform support on existing hardware.
To this end, we present our Acoustic Software Defined Radio (ASDR) --- a versatile communication and sensing platform. Our ASDR is built on a plug-and-play acoustic module which costs less than 10 dollars for the Raspberry Pi card PC. It supports stereo recording with up to 96 kHz sampling rate and three channels for playing the audio signals, each channel of which is boosted by a 3W power amplifier. In the acoustic module, we support hot plug for acoustic sensors and have designed plug-and-play sensor modules with or without AGC. The sensor modules are purposefully chosen to optimize the performance in the preferable inaudible bandwidth. We build our hardware compatible with lots of existing drivers or application utilities, making it plausible for developers who have less programming experience. As a matter of fact, for offline processing, one only needs to make a few clicks in order to get the raw audio samples. To configure the hardware settings, one can use existing software packets with graphic settings, which significantly reduces the efforts for hardware-dependent tuning. Also, we provide abundant APIs and build sufficient examples on our ASDR platforms, demonstrating the efficiency in developing acoustic sensing applications.

# Acoustic Software Defined Radio Hardware Architecture

![ASDR hardware architecture](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/hardware3.png)
Architecture (V1.3)

Major components: Raspberry Pi, INMP411, TPS54332, WM8731

Raspberry Pi has been a versatile education platform for ubiquitous computing worldwide. It has compact size and lots of community supports. There are enormous tutorials and projects samples which are valuable resources for beginners. Specifically, the core processor has the NEON technology~\cite{Neon} which is a packed Single Instruction Multiple Data (SIMD) hardware structure and can be used for computing acceleration. On-chip GPU could also be utilized for acceleration. The Raspbian operation system it runs is a kind of Linux OS and supports almost all the available programming languages. The above good features and its powerful processing capability make the Raspberry Pi the best choice for our ASDR platform.

The codec mounted on the acoustic module is WM8731 which features low power stereo audio codec with an integrated headphone driver and programmable sampling rate. It support 16-32 bits resolution and the programmable sampling rate ranges from 8 kHz to 96 kHz. Adopting WM8731 codec in ASDR wins an edge over those audio hardware in the Android platform since it provides better hardware performance. Another underlying facts for the adoption of WM8731 is that the community offer Linux-compatible drive, which eases the pains for device driver development. The WM8731 is not the only option for our ASDR. We find this chip achieves a good balance between the performance and price, making it more suitable for our ASDR platform. It should be noted that the maximum sampling rate for the codec is 96 kHz and one can easily replace the common microphone modules in ASDR for ultrasonic sensing.

The acoustic module utilizes TDA2822M (replaced with PAM8406 in the second version) for its low supply power voltages, which could be 1.8 V. Also, it is a class-AB audio power amplifier which introduce less signal distortions to the inputs. The microphone for the acoustic module is carefully chosen. Apart from high sensitivity and good signal-to-noise-ratio (SNR), we also make the microphone module to have two special properties: flat frequency response in most frequency band and better sensitivity in the preferable inaudible bandwidth (above 18 kHz). Such good properties could help address the frequency selected problem [4] and guarantee better performance for acoustic sensing applications. These considerations lead us to a choice of INMP411. It can be observed that the frequency response is quite flat between 100 Hz to 10 kHz and after 10 kHz which is the preferable bandwidth for most acoustic sensing applications, the frequency response is significantly improved. Such a nice frequency response makes INMP411 the best choice for our microphone modules. It should be noted that our microphone modules have two different types: one is transistor-based which has fixed 140 X gain and one uses MAX9814 AGC which provides programmable gain. Such a design makes it easier to disable AGC, which has become a prerequisite for profiling based acoustic sensing applications.

The schematic is shown in the following figure:
![Hardware Schematic](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/schematic-pcb/v1.1.png)

The pictures of the prototype are shown in the following figures:
![1](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/photo1.jpg)
![2](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/photo2.jpg)
![3](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/photo3.jpg)
![4](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/photo4.jpg)


# Acoustic Software Defined Radio Software Architecture
![Hardware Schematic](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/software.png)

The software framework consists of four layers: driver, hardware abstraction, middleware, and user space.

Driver Layer: This layer involves a specific device driver to fire up the codec chip WM8731. To ease efforts, We build a script for easy installation. Since this driver is available in kernel sources, we simply build a script to configure it. The driver interfaces with the wrapper module in the hardware abstraction layer. Also, it communicates with shell scripts in the middleware via existing utilities including aplay and arecord.

Hardware Abstract Layer: This layer has a wrapper module to interface with upper layer. It loads a readable JSON-format configuration script from user space to configure the audio hardware. This configuration script can specify the properties of audio hardware including sampling rate, number of channels, bit resolutions, etc.

Middleware: This layer contains three major components, namely shell scripts, communication modules, and a numerical library. The shell scripts serve as communication channels to dispatch commands from the GUI to the hardware. It allows for playing and recording audio profiles by invoking existing utilities such as aplay and arecord through system calls. In this way, the shell scripts help to enable acoustic sensing in an offline manner, as well as to debug the hardware.

The communication modules in the middleware offer three interfaces to bridge the gaps between different programming languages or to delegate the processing tasks to other hosts. First, one can utilize the socket to interface with other target hosts or programming languages. In particular, a client-side MATLAB script, either running locally or remotely, together with a local server-side socket script are provided, assisting the visualization of the processing results in real-time. Since the above socket based scripts accomplish the most cumbersome task of extracting raw audio samples, one can focus on algorithm designs and save efforts on platform details. Second, we build a hybrid of C and Java programming environment via Java Native Interface (establishing another channel for the GUI to communication with hardware, emulating Android and assisting to debug Native Development Kit APIs (infeasible to achieve on Android hosts). 

The numerical library contains utility functions for fast prototyping. It has basic signal processing methods such as i) waveform reshaping to mitigate audible noise when transmitting signals in inaudible frequency range, ii) preamble detection algorithms crucial for most acoustic sensing applications, and iii) robust decoding strategies.
Specifically, we propose a new preamble detection algorithm to effectively handle the near-far effect, hardware heterogeneity, and multipath problem (we refer to Section~\ref{sec:preamble detection} for further details).
In addition, the library also includes efficient numerical utilities built upon two special hardware architectures, namely Neon and GPU, to speed up the program. Both the communication modules and numerical library serve as major components to support user application developments.

User Space: In the user space, ASDR offers a user-friendly Java-based GUI and a readable JSON-format script. The GUI offers various ways for acoustic sensing developments. For instance, it allows recording and playing audio signals via shell scripts by invoking arecord and aplay, respectively, for offline processing. This can also be achieved by running audacity, a compatible professional audio utility. In addition, it provides a function generator for modulation and an oscilloscope for display. Specifically, it can generate popular signals such as pure tone or chirp signals, and it can also visualize the FFT results or spectrogram of received signals in real-time. These functions provided by the GUI can help debug the ASDR hardware and also serve as good utilities for beginners. Moreover, the GUI can read/write the JSON-format script that offers another way for advanced developers to easily access and configure the hardware. 
It should be noted that the offline processing method could not be readily enabled by running available applications on commodity smartphones.
Those applications mostly have "filters" inside, introducing equalization effects that further deteriorate the frequency selectivity problem, rendering them unsuitable for acoustic sensing development.

# Demonstrative Applications:
- Realtime gesture recognition
- Aerial acoustic communication
- Acoustic based indoor localization
- Airborne temperature acquisition

# Installation Guides

**Method one**: use our image provides [here](http:www.baidu.com) and you can run all the code in our ASDR repository

**Method two**: you need to install drivers for GPU/neon/WM8731 

steps to install the drivers for WM8731:

1. go to here to fetch bcm2835 drivers [bcm2835 drivers](https://www.airspayce.com/mikem/bcm2835/) and install using the following steps:

``` 
sudo apt-get update
upload bcm2835-1.52.tar.gz to PI
tar zxvf bcm2835-1.52.tar.gz
cd bcm2835-1.52
./configure
sudo make check
sudo make install
``` 

2. upload the files drivers_for_audios to anywhere on the pi
```
cd drivers_for_audios
chmod +x acousticSetup.sh acousticSetup2.sh
sh acousticSetup.sh
sh acousticSetup2.sh
```
It should be noted that you should enable SPI on the PI. This can be done by running  'sudo raspi-config' and you can graphically enable it. 

steps to install the neon drivers:
1. build the static and dynamic libs:
```
mkdir neon
cd neon
git clone https://github.com/projectNe10/Ne10           # clone the source code from the remote repository
sudo apt-get install cmake
cd $NE10_PATH                                           # change directory to the location of the Ne10 source
mkdir build && cd build                                 # create the build directory and navigate into it
export NE10_LINUX_TARGET_ARCH=armv7                     # set the target architecture (can also be aarch64)
cmake -DGNULINUX_PLATFORM=ON                            # run cmake to generate the build files
make

export NE10_LINUX_TARGET_ARCH=armv7 
cmake -DNE10_BUILD_SHARED=ON
make
```
2. copy the generated libs files to /usr/local/lib
3. You can build your project that use neon API now if you add the absolute build directory '-l /home/xx/pi/neon/Ne10/inc'


 
[1]. Bing Zhou, Mohammed Elbadry, Ruipeng Gao, and Fan Ye. 2017. BatMapper: Acoustic Sensing Based Indoor Floor Plan Construction Using Smartphones. In Proceedings of the 15th Annual International Conference on Mobile Systems, Applications, and Services (MobiSys '17). ACM, New York, NY, USA, 42-55. DOI: https://doi.org/10.1145/3081333.3081363

[2]. Salma Elmalaki, Lucas Wanner, and Mani Srivastava. 2015. CAreDroid: Adaptation Framework for Android Context-Aware Applications. In Proceedings of the 21st Annual International Conference on Mobile Computing and Networking (MobiCom '15). ACM, New York, NY, USA, 386-399. DOI: http://dx.doi.org/10.1145/2789168.2790108

[3]. Tim Kaler, John Patrick Lynch, Timothy Peng, Lenin Ravindranath, Arvind Thiagarajan, Hari Balakrishnan, and Sam Madden. 2010. Code in the air: simplifying sensing on smartphones. In Proceedings of the 8th ACM Conference on Embedded Networked Sensor Systems (SenSys '10). ACM, New York, NY, USA, 407-408. DOI=http://dx.doi.org/10.1145/1869983.1870046

[4]. Wenguang Mao, Zaiwei Zhang, Lili Qiu, Jian He, Yuchen Cui, and Sangki Yun. 2017. Indoor Follow Me Drone. In Proceedings of the 15th Annual International Conference on Mobile Systems, Applications, and Services (MobiSys '17). ACM, New York, NY, USA, 345-358. DOI: https://doi.org/10.1145/3081333.3081362

Maintainer to this repository (No particular order): 

[1]. Pei Rao, Master, Huazhong University of Science and Technology. raopei1994@163.com

[2]. Yan Yan, Master, Huazhong University of Science and Technology. 1063422772@qq.com

[3]. Peng Wang, Master, Huazhong University of Science and Technology. somewap@qq.com

[4]. Liyuan Ye, Master, Huazhong University of Science and Technology. yeliyuan1997@qq.com

[5]. Meichen An, Master, Huazhong University of Science and Technology. m201972133@hust.edu.com

[6]. Chao Cai, Ph.D, Nanyang Technological School. caichao08@gmail.com


