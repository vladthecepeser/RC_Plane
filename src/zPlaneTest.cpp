// #include <Arduino.h>
// #include <SPI.h>
// #include <RadioLib.h>
// #include <ESP32Servo.h>
// #include <thread>
// #include <cmath>

// // Servo objects
// Servo motor;
// Servo leftWing;
// Servo rightWing;

// // NRF24L01+ pins
// #define MY_MISO 19
// #define MY_MOSI 23
// #define MY_SCK 18
// #define MY_CSN   5  // pass MY_CSN as the csn_pin parameter to the RF24 constructor
// uint32_t NSS = 5;
// uint32_t DIO1 = 22;
// uint32_t BUSYSX1280 = 21;
// uint32_t NRST = 17;

// //Servo Pins
// const int motorPin = 25;
// const int rightWingPin = 26;
// const int leftWingPin = 27;

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

// struct ElevonMix {
//     int leftElevon;
//     int rightElevon;
// };

// void resetSequence();
// void resetChip();
// void setFlag(void);
// struct ElevonMix findElevon(Packet);

// SX1280 radio = new Module(NSS, DIO1, NRST, BUSYSX1280);
// SPIClass *vspi = new SPIClass(VSPI);
// bool role = true;  // true = TX role, false = RX role

// #if defined(ESP8266) || defined(ESP32)
// ICACHE_RAM_ATTR
// #endif

// volatile bool receivedFlag = false;

// void setup() {

//     // Allow allocation of all timers
// 	ESP32PWM::allocateTimer(0);
// 	ESP32PWM::allocateTimer(1);
// 	ESP32PWM::allocateTimer(2);
// 	ESP32PWM::allocateTimer(3);

//     Serial.begin(115200);

//     vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
//     pinMode(MY_CSN, OUTPUT);

//     //SX1280 Initialize
//     ConfigLoRa_t config;
//     config.frequency = 2400;
//     int state = radio.begin(config);

//     if(state != RADIOLIB_ERR_NONE) {
//         Serial.print("Init failed, code ");
//         Serial.println(state);
//         while(true);
//     }

//     radio.setDio1Action(setFlag);

//     state = radio.startReceive();

//     if(state != RADIOLIB_ERR_NONE) {
//         Serial.print("startReceive failed, code ");
//         Serial.println(state);
//     }

//     motor.setPeriodHertz(50);      // Standard 50hz servo
// 	leftWing.setPeriodHertz(50);      // Standard 50hz servo
// 	rightWing.setPeriodHertz(50);      // Standard 50hz servo   

//     Serial.println("Receiver ready");
// }


// void loop() {
//     if (!motor.attached() || !leftWing.attached() || !rightWing.attached()) {
// 		motor.attach(motorPin, 1000, 2000);
//         leftWing.attach(leftWingPin, 1000, 2000);
//         rightWing.attach(rightWingPin, 1000, 2000);
// 	}
//     Packet packet;

//     int state = radio.readData((uint8_t*)&packet, sizeof(Packet));

//     if (state == RADIOLIB_ERR_NONE) {
//         (0< packet.rightHorizontal && packet.rightHorizontal < 190) ? packet.rightHorizontal = 0 : packet.rightHorizontal = packet.rightHorizontal;             //Most Left
//         (3845 < packet.rightHorizontal && packet.rightHorizontal < 4095) ? packet.rightHorizontal = 3900 : packet.rightHorizontal = packet.rightHorizontal;     //Most Right
//         (1900 < packet.rightHorizontal && packet.rightHorizontal < 2000) ? packet.rightHorizontal = 1950 : packet.rightHorizontal = packet.rightHorizontal;     //Center

//         (3845 < packet.rightVertical && packet.rightVertical < 4095) ? packet.rightVertical = 3900 : packet.rightVertical = packet.rightVertical;               //Most Up
//         (0< packet.rightVertical && packet.rightVertical < 100) ? packet.rightVertical = 0 : packet.rightVertical = packet.rightVertical;                       //MostDown
//         (1900 < packet.rightVertical && packet.rightVertical < 2000) ? packet.rightVertical = 1950 : packet.rightVertical = packet.rightVertical;               //Center

//         (4000 < packet.leftHorizontal && packet.leftHorizontal < 4050) ? packet.leftHorizontal = 4095 : packet.leftHorizontal = packet.leftHorizontal;          //Most left
//         (0< packet.leftHorizontal && packet.leftHorizontal < 150) ? packet.leftHorizontal = 0 : packet.leftHorizontal = packet.leftHorizontal;                  //Most Right
//         (2000< packet.leftHorizontal && packet.leftHorizontal < 2100) ? packet.leftHorizontal = 2048 : packet.leftHorizontal = packet.leftHorizontal;           //Center

//         (0< packet.leftVertical && packet.leftVertical < 190) ? packet.leftVertical = 0 : packet.leftVertical = packet.leftVertical;                            //Most Up
//         (3850 < packet.leftVertical &&  packet.leftVertical < 4095) ? packet.leftVertical = 3900 : packet.leftVertical = packet.leftVertical;                   //Most Down
//         (1900< packet.leftHorizontal && packet.leftHorizontal < 2000) ? packet.leftHorizontal = 1950 : packet.leftHorizontal = packet.leftHorizontal;           //Center
            
            
//         packet.rightHorizontal = map(packet.rightHorizontal, 0, 3900, 0, 4095);
//         packet.rightVertical = map(packet.rightVertical, 0, 3900, 0, 4095); 
//         packet.leftHorizontal = map(packet.leftHorizontal, 0, 4095, 4095, 0);
//         packet.leftVertical = map(packet.leftVertical, 0, 3900, 4095, 0);

//         ElevonMix elevonMix = findElevon({packet.rightHorizontal, packet.rightVertical});

//         leftWing.write(elevonMix.leftElevon);
//         rightWing.write(elevonMix.rightElevon);
//         motor.write(packet.leftVertical);

//             // Serial.print(" | Received ");
//             // Serial.print(bytes);
//             // Serial.print(" bytes on pipe ");
//             // Serial.print(pipe);
//             Serial.print(" | leftWing=");
//             Serial.print(elevonMix.leftElevon);
//             Serial.print(" | rightWing=");
//             Serial.println(elevonMix.rightElevon);
//             // Serial.print(" | RH=");
//             // Serial.print(packet.rightHorizontal);
//             // Serial.print(" | RV=");
//             // Serial.println(packet.rightVertical);
//             // Serial.print(" LH=");
//             // Serial.print(packet.leftHorizontal);
//             // Serial.print(" LV=");
//             // Serial.print(packet.leftVertical);
//             // Serial.print(" RR=");
//             // Serial.print(packet.rightR);
//             // Serial.print(" RL=");
//             // Serial.print(packet.rightL);
//             // Serial.print(" LR=");
//             // Serial.print(packet.leftR);
//             // Serial.print(" LL=");
//             // Serial.println(packet.leftL);
//     }
//     else {
//         Serial.print("Receive error: ");
//         Serial.println(state);
//         delay(10);
//     }

//     // if (!radio.isChipConnected()) {
//     //     resetChip();
//     //     motor.write(1000);

//     // }
// }

// ElevonMix findElevon(Packet packet) {
//     ElevonMix result;
//     double roll = map(packet.rightHorizontal, 0, 4095, -1000, 1000)/1000.0;
//     double pitch = map(packet.rightVertical, 0, 4095, -1000, 1000)/1000.0;  // Normalize to -1 to 1

//     result.leftElevon = map(1000*((3.0/2.0)*(roll + pitch)- 0.5*pow(roll + pitch, 3)), -1000, 1000, 1000, 2000);
//     result.rightElevon = map(1000*((3.0/2.0)*(pitch - roll)- 0.5*pow(pitch - roll, 3)), -1000, 1000, 2000, 1000);
//     return result;
// }

// void resetSequence(){
// }

// void resetChip(){
// }

// void setFlag(void) {
//     receivedFlag = true;
// }