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

// int payloadSize = 13;

// //Radio Setup Stuff
// RF24 radio(MY_CE, MY_SS); // the (ce_pin, csn_pin) connected to the radio
// const uint64_t pipeAddress = 0xF0F0F0F0D2LL;
// const uint8_t maxRetries = 5; // maximum number of retries for sending data
 
// SPIClass* hspi = nullptr;

// //LCD Setup Stuff
// LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)


// void setup() {

//     Serial.begin(115200);

//     //Initialize the LCD
//     lcd.init();
//     lcd.backlight();
    
//     //Custom SPI pins
//     hspi = new SPIClass(HSPI);
//     hspi->begin(MY_SCLK, MY_MISO, MY_MOSI, MY_SS);
 
//     if (!radio.begin(hspi)) {
//         Serial.println(F("radio hardware not responding!!"));
//         while (1) {} // hold program in infinite loop to prevent subsequent errors
//     }

//     radio.setPALevel(RF24_PA_LOW);       // Optional, useful at close range
//     radio.setPayloadSize(payloadSize);
//     radio.stopListening();

//     Wire.begin(SDA1, SCL1);
//     pinMode(RH, INPUT);
//     pinMode(RV, INPUT);
//     pinMode(LH, INPUT);
//     pinMode(LV, INPUT);
//     pinMode(LL, INPUT_PULLUP);
//     pinMode(LR, INPUT_PULLUP);
//     pinMode(RL, INPUT_PULLUP);
//     pinMode(RR, INPUT_PULLUP);

//     // pinMode(CSN, OUTPUT);
//     // pinMode(CE, OUTPUT);
//     // pinMode(MOSI, OUTPUT);
//     // pinMode(SCK, OUTPUT);
//     // pinMode(IRQ, INPUT);
//     // pinMode(MISO, INPUT);

//   // Initialize the LCD
//   LiquidCrystal_I2C lcd(address, 20, 4);
//   lcd.init();
//   lcd.backlight();
// }


// void loop() {
//     int rightHorizontal = analogRead(RH);
//     int rightVertical = analogRead(RV);
//     int leftHorizontal = analogRead(LH);
//     int leftVertical = analogRead(LV);

//     int rightR = digitalRead(RR);
//     int rightL = digitalRead(RL);
//     int leftR = digitalRead(LR);
//     int leftL = digitalRead(LL);


//     struct Packet {
//         uint16_t rightHorizontal;
//         uint16_t rightVertical;
//         uint16_t leftHorizontal;
//         uint16_t leftVertical;
//         uint8_t rightR;
//         uint8_t rightL;
//         uint8_t leftR;
//         uint8_t leftL;
//     };

//     Packet payload;

//     radio.openWritingPipe(pipeAddress);
//     radio.write(&payload, sizeof(payload));

//     // Attempt to send the payload with retries if needed
//     bool sent = false;
//     for (uint8_t attempt = 0; attempt < maxRetries; ++attempt) {
//         if (radio.write(&payload, sizeof(payload))) {
//             sent = true;
//             break;
//         }
//         lcd.clear();
//     }

//     if (!sent){
//         Serial.print("Transmission Failed");
//         lcd.setCursor(3,0);
//         lcd.print("TRANSMISSION FAILED");
//     }

//     delay(100); // Delay for readability
// }