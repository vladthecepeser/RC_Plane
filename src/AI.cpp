// #include <Arduino.h>
// #include <SPI.h>
// #include "printf.h"
// #include "RF24.h"

// using namespace std;

// // ESP32 DevKit-compatible pins for the nRF24L01+
// #define MY_MISO 22
// #define MY_MOSI 21
// #define MY_SCK  19
// #define MY_CSN  18
// #define MY_CE   4

// // Uncomment one of these for a fixed role per board.
// // Build the TX board with RADIO_ROLE_TX enabled and the RX board with RADIO_ROLE_RX enabled.
// // #define RADIO_ROLE_TX
// // #define RADIO_ROLE_RX
// RF24 radio(MY_CE, MY_CSN, 1000000);
// SPIClass *vspi = new SPIClass(VSPI);

// const uint8_t pipeAddress[6] = "2Node";
// bool role = false;    // true = TX, false = RX
// float payload = 0.0f;

// void setup() {
//   vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
//   Serial.begin(115200);
//   delay(500);

//   pinMode(MY_CE, OUTPUT);
//   pinMode(MY_CSN, OUTPUT);

//   if (!radio.begin(vspi, MY_CE, MY_CSN)) {
//     Serial.println(F("radio hardware is not responding!!"));
//     while (1) {
//       delay(1000);
//     }
//   }

//   radio.setChannel(125);
//   radio.setDataRate(RF24_250KBPS);
//   radio.setCRCLength(RF24_CRC_16);
//   radio.setPALevel(RF24_PA_MIN);
//   radio.enableDynamicPayloads();
//   radio.setAutoAck(true);
//   radio.setRetries(5, 15);
//   radio.setPayloadSize(sizeof(payload));


//   if (role) {
//     // TX sends to the shared pipe address and does not listen for incoming traffic.
//     radio.openWritingPipe(pipeAddress);
//     radio.stopListening();
//     Serial.println(F("TX ready"));
//   } else {
//     // RX listens on the same shared pipe address.
//     radio.openReadingPipe(0, pipeAddress);
//     radio.startListening();
//     Serial.println(F("RX ready"));
//   }

//   radio.flush_tx();
//   radio.flush_rx();

//   printf_begin();
//   radio.printPrettyDetails();
// }

// void loop() {
//   if (role) {
//     //Serial.print(F("[TX] Trying to send: "));
//     //Serial.println(payload);

//     bool report = radio.write(&payload, sizeof(payload));
//     if (report) {
//       Serial.print(F("[TX] Success: "));
//       Serial.print(payload);
//       payload += 0.01f;
//     } else {
//       //Serial.println(F("[TX] Failed or timed out"));
//       delay(.1);
//     }
//     delay(20);
//   } else {
//     uint8_t pipe = 255;
//     if (radio.available(&pipe)) {
//       uint8_t bytes = radio.getPayloadSize();
//       if (pipe > 5 || bytes != sizeof(payload)) {
//         Serial.print(F("[RX] Ignoring invalid packet: pipe="));
//         Serial.print(pipe);
//         Serial.print(F(", bytes="));
//         Serial.println(bytes);
//         radio.flush_rx();
//       } else {
//         radio.read(&payload, bytes);
//         Serial.print(F("[RX] Received "));
//         Serial.print(bytes);
//         Serial.print(F(" bytes on pipe "));
//         Serial.print(pipe);
//         Serial.print(F(": "));
//         Serial.println(payload);
//         delay(1000);
//       }
//     }
//   }
// }
