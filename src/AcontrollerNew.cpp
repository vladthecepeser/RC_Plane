// #include <Arduino.h>
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <SPI.h>
// #include <RadioLib.h>
// #include <atomic>
 

// using namespace std;

// // Pin definitions
//     //Joysticks
//     int RH = 35;
//     int RV = 34;
//     int LH = 39;
//     int LV = 36;

//     //SX1280
//     #define MY_MISO 22
//     #define MY_MOSI 21
//     #define MY_SCK 19
//     #define MY_CSN   18  // pass MY_CSN as the csn_pin parameter to the RF24 constructor
//     uint32_t NSS = 23;
//     uint32_t DIO1 = 5;
//     uint32_t NRST = 4;
//     uint32_t BUSYSX1280 = 2;

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

// void resetSequence();
// void resetChip();

// SX1280 radio = new Module(NSS, DIO1, NRST, BUSYSX1280);
// SPIClass *vspi = new SPIClass(VSPI);
// bool role = true;  // true = TX role, false = RX role

// #if defined(ESP8266) || defined(ESP32)
//   ICACHE_RAM_ATTR
// #endif


// //LCD Setup and Thread
// LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)

// atomic<bool> isTransmitted(false);
// SemaphoreHandle_t lcdMutex;

// //Warning: use only in the LCD thread, not in the main loop
// bool lastState = false;

// void vLCD(void *pvParameters){
//     for(;;){
//         bool current = isTransmitted.load();

//         if(current != lastState){
//             xSemaphoreTake(lcdMutex, portMAX_DELAY);
//             lcd.clear();
//             lcd.setCursor(3, 0);

//             if(current){
//                 lcd.print("TRANSMIT SUCCESSFUL");
//             }
//             else{
//                 lcd.print("TRANSMISSION FAILED");
//             }

//             xSemaphoreGive(lcdMutex);
//         }
//         lastState = current;

//         vTaskDelay(pdMS_TO_TICKS(10)); // Blocks this task, letting Main run
//     }
// }

// void setup() {
//     vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
//     pinMode(MY_CSN, OUTPUT);

//     Serial.begin(115200);
//     Serial.println("ESP32 started");

//     //LCD Setup
//     Wire.begin(SDA1, SCL1);
//     Serial.println("I2C bus started");

//     //Setup LCD I2C Thread on Core 1
//     lcdMutex = xSemaphoreCreateMutex();
//     xTaskCreatePinnedToCore(vLCD, "LCD_Display", 4096, NULL, 1, NULL, 0);
    
//     xSemaphoreTake(lcdMutex, portMAX_DELAY);
//     lcd.init();
//     lcd.backlight();
//     xSemaphoreGive(lcdMutex);

//     //New Radio Setup
//     Serial.print(F("[SX1280] Initializing ... "));
//     ConfigLoRa_t config;
//     config.frequency = 2400;
//     int state = radio.begin(config);
//     if (state != RADIOLIB_ERR_NONE) {
//         Serial.print("Init failed, code ");
//         Serial.println(state);
//         while (true);
//     }

//     Serial.println("Transmitter ready");


//     pinMode(RH, INPUT);
//     pinMode(RV, INPUT);
//     pinMode(LH, INPUT);
//     pinMode(LV, INPUT);
//     pinMode(LL, INPUT_PULLUP);
//     pinMode(LR, INPUT_PULLUP);
//     pinMode(RL, INPUT_PULLUP);
//     pinMode(RR, INPUT_PULLUP);

//     Serial.println("Setup complete");

//     xSemaphoreTake(lcdMutex, portMAX_DELAY);
//     lcd.setCursor(0, 0);
//     lcd.print("Controller Ready");
//     xSemaphoreGive(lcdMutex);
    
//     delay(100);
// }

// void loop() {
//     Packet payload;
//     payload.rightHorizontal = analogRead(RH);
//     payload.rightVertical = analogRead(RV);
//     payload.leftHorizontal = analogRead(LH);
//     payload.leftVertical = analogRead(LV);

//     payload.rightR = digitalRead(RR);
//     payload.rightL = digitalRead(RL);
//     payload.leftR = digitalRead(LR);
//     payload.leftL = digitalRead(LL);
//     Serial.print("Values read!  ");

//     Serial.print("RH: "); Serial.print(payload.rightHorizontal);
//     Serial.print(", RV: "); Serial.print(payload.rightVertical);
//     Serial.print(", LH: "); Serial.print(payload.leftHorizontal);
//     Serial.print(", LV: "); Serial.print(payload.leftVertical);
//     Serial.print(", RR: "); Serial.print(payload.rightR);
//     Serial.print(", RL: "); Serial.print(payload.rightL);
//     Serial.print(", LR: "); Serial.print(payload.leftR);
//     Serial.print(", LL: "); Serial.print(payload.leftL);
//     Serial.println();
    
//     int state = radio.transmit((uint8_t*)&payload, sizeof(payload));

//     isTransmitted.store(state == RADIOLIB_ERR_NONE);

//     // if (!radio.isChipConnected()) {
//     //     resetChip();
//     // }

//     delay(10); // Delay for readability
// }

// void resetSequence(){

// }

// void resetChip(){

// }