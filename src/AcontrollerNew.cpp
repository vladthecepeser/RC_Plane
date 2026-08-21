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
//     #define MY_MISO 19
//     #define MY_MOSI 23
//     #define MY_SCK 18
//     #define MY_CSN   5  // pass MY_CSN as the csn_pin parameter to the RF24 constructor
//     uint32_t NSS = 5;
//     uint32_t DIO1 = 22;
//     uint32_t BUSYSX1280 = 21;
//     uint32_t NRST = 17;
    

//     //LCD
//     int SDA1 = 27;
//     int SCL1 = 14;
//     int address = 0x27;

//     // Left switch
//     int LL = 32;
//     int LR = 33;

//     // Right switch
//     int RL = 25;
//     int RR = 26;

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

// SPIClass spi(VSPI); 
// SX1280 radio = new Module(NSS, DIO1, NRST, BUSYSX1280, spi, spiSettings);
// SPISettings spiSettings(2000000, MSBFIRST, SPI_MODE0);
// bool role = true;  // true = TX role, false = RX role

// #if defined(ESP8266) || defined(ESP32)
//   ICACHE_RAM_ATTR
// #endif


// //LCD Setup and Thread
// LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)

// atomic<bool> isTransmitted(false);
// atomic<int> batteryFlag(0);
// SemaphoreHandle_t lcdMutex;
// TaskHandle_t lcdTaskHandle = NULL;

// volatile bool interruptCounter;
// hw_timer_t *timer = NULL;

// void IRAM_ATTR onTimer() {
//   timerMotors = true;
// }

// void vLCD(void *pvParameters){
//     for(;;){
//         xSemaphoreTake(lcdMutex, portMAX_DELAY);
//         lcd.clear();
//         lcd.setCursor(3, 0);

//         if(current){
//             lcd.print("TRANSMIT SUCCESSFUL");
//         }
//         else{
//             lcd.print("TRANSMISSION FAILED");
//         }
//         xSemaphoreGive(lcdMutex);

//         vTaskDelay(pdMS_TO_TICKS(10)); // Blocks this task, letting Main run
//     }
// }

// void setup() {
//     pinMode(MY_CSN, OUTPUT);

//       // 1. Set 80 prescaler for 1 MHz (1 tick = 1 microsecond)
//     timer = timerBegin(0, 80, true);
    
//     // 2. Attach interrupt callback
//     timerAttachInterrupt(timer, &onTimer, true);
    
//     // 3. Set alarm to trigger every 1/120 of a second (8333 microseconds)
//     timerAlarmWrite(timer, 8333, true);
    
//     // 4. Enable alarm
//     timerAlarmEnable(timer);

//     portMUX_TYPE timerMutex = portMUX_INITIALIZER_UNLOCKED; 

//     Serial.begin(115200);
//     Serial.println("ESP32 started");

//     //LCD Setup
//     Wire.begin(SDA1, SCL1);
//     Serial.println("I2C bus started");

//     //Setup LCD I2C Thread on Core 1
//     lcdMutex = xSemaphoreCreateMutex();
//     xTaskCreatePinnedToCore(vLCD, "LCD_Display", 4096, NULL, 1, &lcdTaskHandle, 0);
    
//     xSemaphoreTake(lcdMutex, portMAX_DELAY);
//     lcd.init();
//     lcd.backlight();
//     xSemaphoreGive(lcdMutex);

//     //New Radio Setup
//     Serial.print(F("[SX1280] Initializing ... "));
//     ConfigLoRa_t config;
//     config.frequency = 2400;
//     spi.begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
//     int state = radio.begin(config);
//     if (state != RADIOLIB_ERR_NONE) {
//         xSemaphoreTake(lcdMutex, portMAX_DELAY);
//         lcd.setCursor(0, 0);
//         lcd.print("RADIO INIT FAILED");
//         while (1) {}  // hold in infinite loop
//         xSemaphoreGive(lcdMutex);
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

// //Warning: use only in the main loop thread, not in the LCD thread
// bool lastState = false;

// void loop() {
//     if(timerMotors){
//         portENTER_CRITICAL(&timerMutex); 

//         Packet payload;
//         payload.rightHorizontal = analogRead(RH);
//         payload.rightVertical = analogRead(RV);
//         payload.leftHorizontal = analogRead(LH);
//         payload.leftVertical = analogRead(LV);

//         payload.rightR = digitalRead(RR);
//         payload.rightL = digitalRead(RL);
//         payload.leftR = digitalRead(LR);
//         payload.leftL = digitalRead(LL);
//         Serial.print("Values read!  ");

//         Serial.print("RH: "); Serial.print(payload.rightHorizontal);
//         Serial.print(", RV: "); Serial.print(payload.rightVertical);
//         Serial.print(", LH: "); Serial.print(payload.leftHorizontal);
//         Serial.print(", LV: "); Serial.print(payload.leftVertical);
//         Serial.print(", RR: "); Serial.print(payload.rightR);
//         Serial.print(", RL: "); Serial.print(payload.rightL);
//         Serial.print(", LR: "); Serial.print(payload.leftR);
//         Serial.print(", LL: "); Serial.print(payload.leftL);
//         Serial.println();
        
//         int state = radio.transmit((uint8_t*)&payload, sizeof(payload));

//         isTransmitted.store(state == RADIOLIB_ERR_NONE);

//         bool currentState = (state == RADIOLIB_ERR_NONE);

//         if (currentState != lastState) {

//             isTransmitted.store(currentState);

//             xTaskNotifyGive(lcdTaskHandle);

//             lastState = currentState;
//         }

//     // if (!radio.isChipConnected()) {
//     //     resetChip();
//     // }

//     //delay(10); // Delay for readability
//         timerMotors = false;
//         portEXIT_CRITICAL(&timerMutex); 
//     } 

// }

// void resetSequence(){

// }

// void resetChip(){

// }