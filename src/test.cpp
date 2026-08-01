// #include <Arduino.h>
// #include <SPI.h>
// #include <RF24.h>

// // NRF24L01+ pins
// #define MY_MISO 22
// #define MY_MOSI 21
// #define MY_SCK 19
// #define MY_CSN 18
// #define MY_CE 4

// void resetSequence();
// void resetChip();

// struct Packet {
//     uint16_t rightHorizontal;
//     uint16_t rightVertical;
//     uint16_t leftHorizontal;
//     uint16_t leftVertical;
//     uint8_t rightR;
//     uint8_t rightL;
//     uint8_t leftR;
//     uint8_t leftL;
// };

// RF24 radio(MY_CE, MY_CSN, 1000000);
// SPIClass *vspi = new SPIClass(VSPI);
// const uint64_t pipeAddress = 0xF0F0F0F0D2LL;
// const uint8_t payloadSize = sizeof(Packet);

// void setup() {
//     Serial.begin(115200);
//     delay(1000);

//     vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
//     pinMode(MY_CE, OUTPUT);
//     pinMode(MY_CSN, OUTPUT);

//     if (!radio.begin(vspi, MY_CE, MY_CSN)) {
//         Serial.println("RADIO INIT FAILED");
//         while (1) {
//             delay(1000);
//         }
//     }

//     resetSequence();

//     Serial.println("Receiver ready");
// }

// void loop() {
//     uint8_t pipe = 255;

//     if (radio.available(&pipe)) {
//         Packet packet;
//         uint8_t bytes = radio.getPayloadSize();

//         if (bytes == sizeof(packet)) {
//             radio.read(&packet, bytes);
//             Serial.print("Received ");
//             Serial.print(bytes);
//             Serial.print(" bytes on pipe ");
//             Serial.print(pipe);
//             Serial.print(" | RH=");
//             Serial.print(packet.rightHorizontal);
//             Serial.print(" RV=");
//             Serial.print(packet.rightVertical);
//             Serial.print(" LH=");
//             Serial.print(packet.leftHorizontal);
//             Serial.print(" LV=");
//             Serial.print(packet.leftVertical);
//             Serial.print(" RR=");
//             Serial.print(packet.rightR);
//             Serial.print(" RL=");
//             Serial.print(packet.rightL);
//             Serial.print(" LR=");
//             Serial.print(packet.leftR);
//             Serial.print(" LL=");
//             Serial.println(packet.leftL);
//         } else {
//             Serial.print("Invalid payload size: ");
//             Serial.println(bytes);
//             radio.flush_rx();
//         }
//     }

//     if (!radio.isChipConnected()) {
//         resetChip();
//     }
//     delay(100);
// }


// void resetSequence(){
//     radio.setChannel(76);
//     radio.setDataRate(RF24_1MBPS);
//     radio.setCRCLength(RF24_CRC_16);
//     radio.setRetries(5, 15);
//     radio.setAutoAck(true);
//     radio.disableDynamicPayloads();
//     radio.flush_tx();
//     radio.flush_rx();
//     radio.openReadingPipe(0, pipeAddress);
//     radio.setPALevel(RF24_PA_MIN);
//     radio.setPayloadSize(payloadSize);
//     radio.startListening();
// }

// void resetChip(){

//     Serial.println("Resetting radio chip...");
    
//     radio.powerDown();
//     delay(500);     //Important!!!

//     if (!radio.begin(vspi, MY_CE, MY_CSN)) {
//         Serial.println("Radio chip re-initialization failed");
//         return;
//     }

//     //Go thru the flow
//     resetSequence();

//     if (!radio.isChipConnected()) {
//         Serial.println("Failed re-initialization");
//         resetChip();
//     } 
//     else {
//         Serial.println("Successful re-initialization");
//     }

//     delay(100);
// }