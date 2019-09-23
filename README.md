# Acoustic-Software-Defined-Radios
We present an Acoustic Software Defined Radios platform. 
ASDR is a versatile communication and sensing platform that offers extreme ﬂexibility for researchers fast prototyping and deploying acoustic sensing applications. Like SDR in RF domain, ASDR can help researchers to inspect physical layer details in an acoustic sensing system and even provide plug-and-play hardware ﬂexibility to incorporate new modules. One can enjoy abundant APIs provided by ASDR to develop new and interesting applications, reducing the efforts on platform details, or exploit the ﬂexible conﬁgurations of platform-speciﬁc features to give guidance for future acoustic management design both in hardware and software architecture. Also, we have demonstrated various interesting applications on our ASDR platform.

ASDR aims to tackle the following issues that were not addressed by previous work:

--- **Lacking full exploitation of acoustic modules**. Existing work only provides mono channel measurements, which is mostly sampled from the bottom microphone on smartphones. This microphone is purposefully optimized for the audible frequency bandwidth below $8$ kHz~\cite{BatMapper} and suffers from severe distortions on higher frequency band. The microphone on the rear or back of smartphones are not utilized. This microphone actually performs better than the bottom one in the preferable inaudible frequency range. However, no APIs are provided to extract audio samples from the rear microphone. Meanwhile, the ability to simultaneously record stereo samples is also under utilization.


