#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>

using namespace std;


// Pin definitions
    //NRF24L01+
    #define MY_MISO 22
    #define MY_MOSI 21
    #define MY_SCLK 19
    #define MY_SS   18  // pass MY_SS as the csn_pin parameter to the RF24 constructor
    #define MY_CE   4

//Radio Setup Stuff
RF24 radio(MY_CE, MY_SS); // the (ce_pin, csn_pin) connected to the radio

 
SPIClass* hspi = nullptr;

struct Packet {
    uint16_t rightHorizontal;
    uint16_t rightVertical;
    uint16_t leftHorizontal;
    uint16_t leftVertical;
    uint8_t rightR;
    uint8_t rightL;
    uint8_t leftR;
    uint8_t leftL;
};
const uint64_t pipeAddress = 0xF0F0F0F0D2LL;
const uint8_t payloadSize = sizeof(Packet);

void setup() {

    Serial.begin(115200);
    
    //Custom SPI pins
    hspi = new SPIClass(HSPI);
    hspi->begin(MY_SCLK, MY_MISO, MY_MOSI, MY_SS);
 
    if (!radio.begin(hspi)) {
        Serial.println(F("radio hardware not responding!!"));
        while (1) {} // hold program in infinite loop to prevent subsequent errors
    }

    radio.setPALevel(RF24_PA_LOW);       // Optional, useful at close range
    radio.setPayloadSize(payloadSize);
    radio.openReadingPipe(1, pipeAddress);
    radio.startListening();

    // pinMode(CSN, OUTPUT);
    // pinMode(CE, OUTPUT);
    // pinMode(MOSI, OUTPUT);
    // pinMode(SCK, OUTPUT);
    // pinMode(IRQ, INPUT);
    // pinMode(MISO, INPUT);
}


void loop() {
    if (radio.available()) {
        Packet payload;
        radio.read(&payload, sizeof(payload));

        Serial.print("Data received");
        Serial.print(" ");
        Serial.print("RX: ");
        Serial.print(payload.rightHorizontal);
        Serial.print(" ");
        Serial.print(payload.rightVertical);
        Serial.print(" ");
        Serial.print(payload.leftHorizontal);
        Serial.print(" ");
        Serial.println(payload.leftVertical);
    }
    else{
        Serial.println("No data available");
    }
    delay(100);
}