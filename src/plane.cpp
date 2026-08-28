// #include <Arduino.h>
// #include <SPI.h>
// #include <RF24.h>
// #include <ESP32Servo.h>
// #include <thread>
// #include <cmath>
// #include <atomic>

// using namespace std;

// // Servo objects
// Servo motor;
// Servo leftWing;
// Servo rightWing;

// // NRF24L01+ pins
// #define MY_MISO 22
// #define MY_MOSI 21
// #define MY_SCK 19
// #define MY_CSN 4
// #define MY_CE 18
// #define MY_IRQ 5

// //Servo Pins
// const int motorPin = 25;
// const int rightWingPin = 27;
// const int leftWingPin = 14;

// const int lights = 23;

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
// struct ElevonMix findElevon(Packet);

// RF24 radio(MY_CE, MY_CSN, 1000000);
// SPIClass *vspi = new SPIClass(VSPI);
// const uint64_t pipeAddress = 0xF0F0F0F0D2LL;
// const uint8_t payloadSize = sizeof(Packet);

// volatile bool interruptCounter = false;
// hw_timer_t *timer = NULL;

// void IRAM_ATTR onTimer() {
//   interruptCounter = true;
// }

// void vLED(void *pvParameters)
// {
//     TickType_t lastWake = xTaskGetTickCount();

//     for (;;)
//     {
//         vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(1450));

//         digitalWrite(lights, HIGH);
//         vTaskDelay(pdMS_TO_TICKS(50));
//         digitalWrite(lights, LOW);
//     }
// }

// void setup() {
//     timer = timerBegin(0, 80, true);                //Set 80 prescaler for 1 MHz (1 tick = 1 microsecond)
//     timerAttachInterrupt(timer, &onTimer, true);    //Attach interrupt callback
//     timerAlarmWrite(timer, 8333, true);             //Set alarm to trigger every 1/120 of a second (8333 microseconds)
//     timerAlarmEnable(timer);                        //Enable Alarm

//     portMUX_TYPE timerMutex = portMUX_INITIALIZER_UNLOCKED; 

//     // Allow allocation of all timers
// 	ESP32PWM::allocateTimer(0);
// 	ESP32PWM::allocateTimer(1);
// 	ESP32PWM::allocateTimer(2);
// 	ESP32PWM::allocateTimer(3);

//     xTaskCreatePinnedToCore(vLED, "LED_Lights", 4096, NULL, 1, NULL, 0); //Setup LCD Thread on Core 0

//     Serial.begin(115200);

//     vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
//     pinMode(MY_CE, OUTPUT);
//     pinMode(MY_CSN, OUTPUT);

//     pinMode(lights, OUTPUT);

//     if (!radio.begin(vspi, MY_CE, MY_CSN)) {
//         Serial.println("RADIO INIT FAILED");
//         while (1) {
//             delay(10);
//         }
//     }

//     resetSequence();

//     motor.setPeriodHertz(50);      // Standard 50hz servo
// 	leftWing.setPeriodHertz(50);      // Standard 50hz servo
// 	rightWing.setPeriodHertz(50);      // Standard 50hz servo   

//     Serial.println("Receiver ready");
// }

// unsigned long sendTime = millis();

// void loop() {
//     if (interruptCounter) {
//         //int currentTime = millis();

//         if (!motor.attached() || !leftWing.attached() || !rightWing.attached()) {
//             motor.attach(motorPin, 1000, 2000);
//             leftWing.attach(leftWingPin, 1000, 2000);
//             rightWing.attach(rightWingPin, 1000, 2000);
//         }

//         uint8_t pipe = 255;
//         if (radio.available(&pipe)) {
//             Packet packet;
//             uint8_t bytes = radio.getPayloadSize();

//             if (bytes == sizeof(packet)) {
//                 radio.read(&packet, bytes);

//                 (0< packet.rightHorizontal && packet.rightHorizontal < 190) ? packet.rightHorizontal = 0 : packet.rightHorizontal = packet.rightHorizontal;             //Most Left
//                 (3845 < packet.rightHorizontal && packet.rightHorizontal < 4095) ? packet.rightHorizontal = 3900 : packet.rightHorizontal = packet.rightHorizontal;     //Most Right
//                 (1900 < packet.rightHorizontal && packet.rightHorizontal < 2000) ? packet.rightHorizontal = 1950 : packet.rightHorizontal = packet.rightHorizontal;     //Center

//                 (3845 < packet.rightVertical && packet.rightVertical < 4095) ? packet.rightVertical = 3900 : packet.rightVertical = packet.rightVertical;               //Most Up
//                 (0< packet.rightVertical && packet.rightVertical < 100) ? packet.rightVertical = 0 : packet.rightVertical = packet.rightVertical;                       //MostDown
//                 (1900 < packet.rightVertical && packet.rightVertical < 2000) ? packet.rightVertical = 1950 : packet.rightVertical = packet.rightVertical;               //Center

//                 (4000 < packet.leftHorizontal && packet.leftHorizontal < 4050) ? packet.leftHorizontal = 4095 : packet.leftHorizontal = packet.leftHorizontal;          //Most left
//                 (0< packet.leftHorizontal && packet.leftHorizontal < 150) ? packet.leftHorizontal = 0 : packet.leftHorizontal = packet.leftHorizontal;                  //Most Right
//                 (2000< packet.leftHorizontal && packet.leftHorizontal < 2100) ? packet.leftHorizontal = 2048 : packet.leftHorizontal = packet.leftHorizontal;           //Center

//                 (0< packet.leftVertical && packet.leftVertical < 190) ? packet.leftVertical = 0 : packet.leftVertical = packet.leftVertical;                            //Most Up
//                 (3850 < packet.leftVertical &&  packet.leftVertical < 4095) ? packet.leftVertical = 3900 : packet.leftVertical = packet.leftVertical;                   //Most Down
//                 (1900< packet.leftHorizontal && packet.leftHorizontal < 2000) ? packet.leftHorizontal = 1950 : packet.leftHorizontal = packet.leftHorizontal;           //Center
                
                
//                 packet.rightHorizontal = map(packet.rightHorizontal, 0, 3900, 0, 4095);
//                 packet.rightVertical = map(packet.rightVertical, 0, 3900, 0, 4095); 
//                 packet.leftHorizontal = map(packet.leftHorizontal, 0, 4095, 4095, 0);
//                 packet.leftVertical = map(packet.leftVertical, 0, 3900, 1500, 1000);

//                 ElevonMix elevonMix = findElevon({packet.rightHorizontal, packet.rightVertical});

//                 leftWing.write(elevonMix.leftElevon);
//                 rightWing.write(elevonMix.rightElevon);
//                 motor.write(packet.leftVertical);

//                 // if (currentTime - sendTime >= 100) {
//                 //     sendTime = currentTime;

//                 //     // Serial.print(" | Received ");
//                 //     // Serial.print(bytes);
//                 //     // Serial.print(" bytes on pipe ");
//                 //     // Serial.print(pipe);
//                 //     Serial.print(" | leftWing=");
//                 //     Serial.print(elevonMix.leftElevon);
//                 //     Serial.print(" | rightWing=");
//                 //     Serial.println(elevonMix.rightElevon);
//                 //     // Serial.print(" | RH=");
//                 //     // Serial.print(packet.rightHorizontal);
//                 //     // Serial.print(" | RV=");
//                 //     // Serial.println(packet.rightVertical);
//                 //     // Serial.print(" LH=");
//                 //     // Serial.print(packet.leftHorizontal);
//                 //     // Serial.print(" LV=");
//                 //     // Serial.print(packet.leftVertical);
//                 //     // Serial.print(" RR=");
//                 //     // Serial.print(packet.rightR);
//                 //     // Serial.print(" RL=");
//                 //     // Serial.print(packet.rightL);
//                 //     // Serial.print(" LR=");
//                 //     // Serial.print(packet.leftR);
//                 //     // Serial.print(" LL=");
//                 //     // Serial.println(packet.leftL);
//                 // }
                
//             } else {
//                 Serial.print("Invalid payload size: ");
//                 Serial.println(bytes);
//                 radio.flush_rx();
//                 delay(10);
//             }
//         }
//         interruptCounter = false;
//     }
//         if (!radio.isChipConnected()) {
//             resetChip();
//             motor.write(1500);
//         }
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

//     delay(10);
// }