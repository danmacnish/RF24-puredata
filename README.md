# RF24-puredata
an external for puredata, allowing you to stream data wirelessly to a raspberry pi with the NRF24L01+

detailed instructions can be found @ hacakday.io:embedded puredata + wireless

This external requires the RF24 library @ https://github.com/stanleyseow/RF24.git to be installed.
You can copy the precompiled external (RF24Listener.pd_linux) to ../puredata/extra/
or you can compile the external from source. Note that you will have to install the flext environment before compiling: http://grrrr.org/research/software/flext/

