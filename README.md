# Acoustic-Software-Defined-Radios
We present an Acoustic Software Defined Radios platform. 
ASDR is a versatile communication and sensing platform that offers extreme ﬂexibility for researchers fast prototyping and deploying acoustic sensing applications. Like SDR in RF domain, ASDR can help researchers to inspect physical layer details in an acoustic sensing system and even provide plug-and-play hardware ﬂexibility to incorporate new modules. One can enjoy abundant APIs provided by ASDR to develop new and interesting applications, reducing the efforts on platform details, or exploit the ﬂexible conﬁgurations of platform-speciﬁc features to give guidance for future acoustic management design both in hardware and software architecture. Also, we have demonstrated various interesting applications on our ASDR platform.

ASDR aims to tackle the following issues that were not addressed by previous work:

- **Lacking full exploitation of acoustic modules**. Existing work only provides mono channel measurements, which is mostly sampled from the bottom microphone on smartphones. This microphone is purposefully optimized for the audible frequency bandwidth below $8$ kHz[^BatMapper] and suffers from severe distortions on higher frequency band. The microphone on the rear or back of smartphones are not utilized. This microphone actually performs better than the bottom one in the preferable inaudible frequency range. However, no APIs are provided to extract audio samples from the rear microphone. Meanwhile, the ability to simultaneously record stereo samples is also under utilization.
- **Difficult to fine-tune hardware configurations** Existing platforms are data centric and do not focus on hardware. Thus they do not provide APIs to flexibly configure the hardware for specific tasks. For instance, It is not feasible to disable the Automatic Gain Control (AGC) on the dual channels. And it is labor intensive to disable the AGC on either channel since one has to compile platform-dependent kernel drivers for the acoustic modules. In addition, when prototyping special applications that need kernel hacking, one has to resolve lots of OS-associated issues (e.g., root the device) before reaching the kernel, incurring extra burden in development.
- **Compatible issues** When the host operation systems (OS) or software development tookit (SDK) undergo dramatic changes, compatible issues will arise. That is, existing platform will encounter failures when there are system upgrades. Meanwhile, existing utilities~\cite{CAreDroid,CodeInTheAir} that provide configurable abilities to low-lay settings could easily fail to work across different platforms since the acoustic modules on commodity devices have sharp differences on hardware design especially those on the Android platforms, creating a significant burden on building a versatile utility.
- **Fixed physical layout of acoustic sensors** The acoustic sensors on commodity devices are mostly covered by a shell, leaving only a single hole for the signals to travel through. This fixed physical layout for multiple sensors hampers much flexibility in deploying systems that needs stereo recording. In addition, shielding the sensors raise the risk of introducing non-line-of-sight reflections.

The above challenges make it difficult to build cross-platform support on existing hardware.
To this end, we present our Acoustic Software Defined Radio (ASDR) --- a versatile communication and sensing platform. Our ASDR is built on a plug-and-play acoustic module which costs less than $10$ dollars for the Raspberry Pi card PC. It supports stereo recording with up to $96$ kHz sampling rate and three channels for playing the audio signals, each channel of which is boosted by a $3$W power amplifier. In the acoustic module, we support hot plug for acoustic sensors and have designed plug-and-play sensor modules with or without AGC. The sensor modules are purposefully chosen to optimize the performance in the preferable inaudible bandwidth. We build our hardware compatible with lots of existing drivers or application utilities, making it plausible for developers who have less programming experience. As a matter of fact, for offline processing, one only needs to make a few clicks in order to get the raw audio samples. To configure the hardware settings, one can use existing software packets with graphic settings, which significantly reduces the efforts for hardware-dependent tuning. Also, we provide abundant APIs and build sufficient examples on our ASDR platforms, demonstrating the efficiency in developing acoustic sensing applications.



[^BatMapper:]. Bing Zhou, Mohammed Elbadry, Ruipeng Gao, and Fan Ye. 2017. BatMapper: Acoustic Sensing Based Indoor Floor Plan Construction Using Smartphones. In Proceedings of the 15th Annual International Conference on Mobile Systems, Applications, and Services (MobiSys '17). ACM, New York, NY, USA, 42-55. DOI: https://doi.org/10.1145/3081333.3081363