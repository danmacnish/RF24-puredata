/*
RF24Listener
This external for puredata is written using the flext environment.
The code reads data from an NRF24L01+ module connected to a raspberry pi and outputs it to pureData.

Dependencies: RF24 library. If you compile this code from source, you will also need to install the flext environment.

required hardware:

Reciever:
NRF24L01+ connected to a raspberry pi

transmitter:
NRF24L01+ connected to an arduino pro mini

Autoacknowledgement is not used. pipeline 1 is enabled.
*/

#include <cstdlib>
// include flext header
#include <flext.h>
// include RF24 header (absolute path specified)
#include <RF24.h>

using namespace std;

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

//define the class that stands for the pureData object:

class RF24Listener:
	// inherit from basic flext class
	public flext_base
{
	// obligatory flext header (class name,base class name)
	FLEXT_HEADER(RF24Listener,flext_base)

public:

	// constructor
	RF24Listener();

protected:
	
	//method that is called when a bang message is recieved at the input:
	void m_bang();
	
private: 

	FLEXT_CALLBACK(m_bang);  // callback for method "m_bang" (with no input arguments)
};

// instantiate the pureData object class
FLEXT_NEW("RF24Listener", RF24Listener)

//instantiate the RF24 class
RF24 radio(25, 8, BCM2835_SPI_SPEED_8MHZ);

//constructor
RF24Listener::RF24Listener()
{
	//define bang inlet. When a bang is sent to this inlet, the m_bang method will be called.
	AddInAnything();
	
	//define outlets
	AddOutFloat(); //first outlet, raw received value is output here
	AddOutInt(); //second outlet, pipeline number that data was recieved on is output here
	AddOutInt(); //3rd outlet = first 2 bytes of recieved data
	AddOutInt(); //4th outlet = second 2 bytes of recieved data
	AddOutInt(); //5th outlet = 3rd 2 bytes of recieved data
	AddOutInt(); //6th outlet = 4th 2 bytes of recieved data
	AddOutInt(); //7th outlet = 5th 2 bytes of recieved data
	AddOutInt(); //8th outlet = 6th 2 bytes of recieved data
	AddOutInt(); //9th outlet = 7th 2 bytes of recieved data
	AddOutInt(); //10th outlet = 8th 2 bytes of recieved data
	//more outputs could be added for more data. I only sent data from 8 sensors to the raspberry pi, hence 8 lots of 2 bytes.
	
	//register method for bang event on input 0
	FLEXT_ADDBANG(0, m_bang);
	
	//initialise radio for reading
    	radio.begin();
    	radio.setAutoAck(0); //disable auto acknowledgement
    	radio.setRetries(0,0); //disable resending data
    	radio.enableDynamicPayloads(); //for some reason static payloads do not work.... radio.available always returns false??
    	radio.setDataRate(RF24_1MBPS); //set data rate to 1mbps
    	radio.setPALevel(RF24_PA_MAX); //set transmit power to max (not really relevant since we are only receiving…)
    	radio.setChannel(90); //set channel
   	radio.setCRCLength(RF24_CRC_16); //set CRC to 16 bits. could also be set to 8 bits.
   	radio.openReadingPipe(1,0xF0F0F0F0E1LL); //open pipe 1, and give it the address of 0xF0F0F0F0E1LL
    	radio.startListening(); //set the RF24 module to start listening for data

}

//function called when bang message is recieved at the input:
void RF24Listener::m_bang()
{
	//declare local variable to store pipeline number that data was received on
	static uint8_t * pipe_number;

	if(radio.available(pipe_number)) //if there is data available on pipeline
	{	
		//array to store the raw value read from the NRF24L01+:
		static int raw[16];

		//variable to output raw value to puredata
		static float * raw_out; 

        	//read payload size
        	static uint8_t len = radio.getDynamicPayloadSize();
		//read data from radio		
		radio.read(raw, len);

		//typecast raw value to a float pointer, send value to output 1 (raw value output)
		raw_out = (float *)raw; ToOutFloat(1, *raw_out); 

		//send elements of array to outputs. once again, more outputs could be added for more data.
		ToOutInt(2, raw[0]);
		ToOutInt(3, raw[1]);
		ToOutInt(4, raw[2]);
		ToOutInt(5, raw[3]);
		ToOutInt(6, raw[4]);
		ToOutInt(7, raw[5]);
		ToOutInt(8, raw[6]);
		ToOutInt(9, raw[7]);
	}
	else {};
}
