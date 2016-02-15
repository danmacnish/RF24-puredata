/*This code is used to read the analog inputs of an arduino pro mini,
 * and transmit data to a rasperry pi running puredata and the RF24 external. 
 * see www.github.com/whiteboarddan/RF24-puredata
 */

#include <SPI.h>
#include <RF24.h>
 
// ce,csn pins
RF24 radio(9,10);

// array to store values read from analog inputs. array is 32 bytes long, corresponding to the 32 byte NRF24 maximum payload
unsigned int outBuffer[16]= {0};
 
void setup(void)
{
    // init radio for writing on channel 76
    radio.begin();
    radio.setAutoAck(0); //disable auto acknowledgement
    radio.setRetries(0,0); //disable resending of data
    radio.enableDynamicPayloads(); //enable dynamic payloads. Static payloads didn't seem to work (July 2015)
    radio.setPALevel(RF24_PA_MAX); //set transmit power to maximum
    radio.setChannel(0x5a); //set channel to match raspberry pi
    radio.openWritingPipe(0xF0F0F0F0E1LL); //set pipe address to match raspberry pi
    radio.powerUp();
}
 
void loop(void)
{
    //read data from the analog inputs, and write them to an array. up to 16 analog values can be transmitted in one payload
    outBuffer[0] = analogRead(A3);
    outBuffer[2] = analogRead(A2);
    outBuffer[4] = analogRead(A1);
    outBuffer[6] = analogRead(A0);
    outBuffer[8] = analogRead(A4);
    outBuffer[10] = analogRead(A6);
    // transmit data over SPI to the NRF module
    radio.write(outBuffer, 32);
 
    // pause a second, so that ADC can stabilise
    delay(50);
}
