// #include <Arduino.h>
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <SPI.h>
// #include <RF24.h>
 

// using namespace std;

// // Pin definitions
//     //Joysticks
//     int RH = 35;
//     int RV = 34;
//     int LH = 39;
//     int LV = 36;

//     //NRF24L01+
//     #define MY_MISO 22
//     #define MY_MOSI 21
//     #define MY_SCLK 19
//     #define MY_SS   18  // pass MY_SS as the csn_pin parameter to the RF24 constructor
//     #define MY_CE   4
//     #define MY_IRQ  23

//     //LCD
//     int SDA1 = 32;
//     int SCL1 = 33;
//     int address = 0x27;

//     // Left switch
//     int LL = 25;
//     int LR = 26;

//     // Right switch
//     int RL = 27;
//     int RR = 14;

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

// //Radio Setup Stuff
// RF24 radio(MY_CE, MY_SS); // the (ce_pin, csn_pin) connected to the radio
// const uint64_t pipeAddress = 0xF0F0F0F0D2LL;
// const uint8_t maxRetries = 5; // maximum number of retries for sending data
// const uint8_t payloadSize = sizeof(Packet);
 
// SPIClass* hspi = nullptr;

// //LCD Setup Stuff
// LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)


// void setup() {
//     Serial.begin(115200);
//     delay(1000);
//     Serial.println("ESP32 started");

//     Wire.begin(SDA1, SCL1);
//     Serial.println("I2C bus started");
//     delay(1000);

//     hspi = new SPIClass(HSPI);
//     hspi->begin(MY_SCLK, MY_MISO, MY_MOSI, MY_SS);
//     Serial.println("SPI initialized");
//     delay(1000);

//     if (!radio.begin(hspi)) {
//         Serial.println(F("radio hardware not responding!!"));
//         Serial.println("Continuing without radio");
//     } else {
//         radio.setPALevel(RF24_PA_LOW);
//         radio.setPayloadSize(payloadSize);
//         radio.stopListening();
//         Serial.println("Radio initialized");
//     }
//     delay(1000);

//     pinMode(RH, INPUT);
//     pinMode(RV, INPUT);
//     pinMode(LH, INPUT);
//     pinMode(LV, INPUT);
//     pinMode(LL, INPUT_PULLUP);
//     pinMode(LR, INPUT_PULLUP);
//     pinMode(RL, INPUT_PULLUP);
//     pinMode(RR, INPUT_PULLUP);

//         //Initialize the LCD
//     lcd.init();
//     lcd.backlight();

//     Serial.println("Setup complete");
//     lcd.print("Controller Ready");
//     delay(10000);
// }

// bool lcdSentSuccess = false;
// bool lcdSentFailure = false;

// void loop() {
//     int rightHorizontal = analogRead(RH);
//     int rightVertical = analogRead(RV);
//     int leftHorizontal = analogRead(LH);
//     int leftVertical = analogRead(LV);

//     int rightR = digitalRead(RR);
//     int rightL = digitalRead(RL);
//     int leftR = digitalRead(LR);
//     int leftL = digitalRead(LL);
//     Serial.print("Values read!  ");
//     // Serial.print("RH: "); Serial.print(rightHorizontal);
//     // Serial.print(", RV: "); Serial.print(rightVertical);
//     // Serial.print(", LH: "); Serial.print(leftHorizontal);
//     // Serial.print(", LV: "); Serial.print(leftVertical);
//     // Serial.print(", RR: "); Serial.print(rightR);
//     // Serial.print(", RL: "); Serial.print(rightL);
//     // Serial.print(", LR: "); Serial.print(leftR);
//     // Serial.print(", LL: "); Serial.print(leftL);
//     // Serial.println();

//     Packet payload;

//     radio.openWritingPipe(pipeAddress);
//     radio.write(&payload, sizeof(payload));

//     // Attempt to send the payload with retries if needed
//     bool sent = false;
//     for (uint8_t attempt = 0; attempt < maxRetries; ++attempt) {
//         if (radio.write(&payload, sizeof(payload))) {
//             Serial.println("Transmitted Successfully!");
//             sent = true;
//             if (!lcdSentSuccess) {
//                 lcd.clear();
//                 lcd.setCursor(3, 0);
//                 lcd.print("TRANSMITTED SUCCESSFULLY");
//                 lcdSentSuccess = true;
//                 lcdSentFailure = false; // Reset failure flag
//             }
//             break;
//         }
//     }

//     if (!sent){
//         Serial.println("Transmission Failed");
//         if(!lcdSentFailure) {
//             lcd.setCursor(0, 0);
//             lcd.clear();
//             lcd.print("TRANSMISSION FAILED");
//             lcdSentFailure = true;
//             lcdSentSuccess = false; // Reset success flag
//         }
//     }

//     delay(10); // Delay for readability
// }