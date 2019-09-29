# Acoustic-Software-Defined-Radios
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

![ASDR hardware architecture](https://github.com/caichao/Acoustic-Software-Defined-Radios/blob/master/pics/hardware.png)
Architecture (V1.0)

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

# Demonstrative Applications:
- Realtime gesture recognition
- Aerial acoustic communication
- Acoustic based indoor localization
- Airborne temperature acquisition

# Installation Guides

**Method one**: use our image provides ![here](http:www.baidu.com) and you can run all the code in our ASDR repository

**Method two**: you need to install drivers for GPU/neon/WM8731 

steps to install the drivers for WM8731:

1. go to here to fetch bcm2835 drivers ![bcm2835 drivers](https://www.airspayce.com/mikem/bcm2835/) and install using the following steps:
upload bcm2835-1.52.tar.gz to PI
tar zxvf bcm2835-1.52.tar.gz
cd bcm2835-1.52
./configure
sudo make check
sudo make install
2. 

3. 

steps to install the drivers for GPU:

1. 

2.

3. 

steps to install the drivers for WM8731:


 
[1]. Bing Zhou, Mohammed Elbadry, Ruipeng Gao, and Fan Ye. 2017. BatMapper: Acoustic Sensing Based Indoor Floor Plan Construction Using Smartphones. In Proceedings of the 15th Annual International Conference on Mobile Systems, Applications, and Services (MobiSys '17). ACM, New York, NY, USA, 42-55. DOI: https://doi.org/10.1145/3081333.3081363

[2]. Salma Elmalaki, Lucas Wanner, and Mani Srivastava. 2015. CAreDroid: Adaptation Framework for Android Context-Aware Applications. In Proceedings of the 21st Annual International Conference on Mobile Computing and Networking (MobiCom '15). ACM, New York, NY, USA, 386-399. DOI: http://dx.doi.org/10.1145/2789168.2790108

[3]. Tim Kaler, John Patrick Lynch, Timothy Peng, Lenin Ravindranath, Arvind Thiagarajan, Hari Balakrishnan, and Sam Madden. 2010. Code in the air: simplifying sensing on smartphones. In Proceedings of the 8th ACM Conference on Embedded Networked Sensor Systems (SenSys '10). ACM, New York, NY, USA, 407-408. DOI=http://dx.doi.org/10.1145/1869983.1870046

[4]. Wenguang Mao, Zaiwei Zhang, Lili Qiu, Jian He, Yuchen Cui, and Sangki Yun. 2017. Indoor Follow Me Drone. In Proceedings of the 15th Annual International Conference on Mobile Systems, Applications, and Services (MobiSys '17). ACM, New York, NY, USA, 345-358. DOI: https://doi.org/10.1145/3081333.3081362
