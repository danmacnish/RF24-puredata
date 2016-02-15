# RF24-puredata
an external for puredata, allowing you to stream data wirelessly to a raspberry pi with the NRF24L01+

detailed instructions can be found @ https://hackaday.io/project/9130-embedded-puredata-wireless

This external requires the RF24 library @ https://github.com/stanleyseow/RF24.git to be installed.
After installing the RF24 library, you can copy the precompiled external (RF24Listener.pd_linux) to ../puredata/extra/
OR you can compile the external from source. Note that you will have to install the flext environment before compiling: http://grrrr.org/research/software/flext/
Information on how to compile the source code using flext can be found on the hackaday.io project website.

