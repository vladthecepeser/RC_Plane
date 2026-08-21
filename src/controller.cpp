#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RF24.h>
#include <atomic>
 

using namespace std;

// Pin definitions
    //Joysticks
    int RH = 35;
    int RV = 34;
    int LH = 39;
    int LV = 36;

    //NRF24L01+
    #define MY_IRQ  23
    #define MY_MISO 22
    #define MY_MOSI 21
    #define MY_SCK 19
    #define MY_CSN   18  // pass MY_CSN as the csn_pin parameter to the RF24 constructor
    #define MY_CE   4
    
    //LCD
    int SDA1 = 32;
    int SCL1 = 33;
    int address = 0x27;

    // Left switch
    int LL = 25;
    int LR = 26;

    // Right switch
    int RL = 27;
    int RR = 14;

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

void resetSequence();
void resetChip();

//Radio Setup Stuff
RF24 radio(MY_CE, MY_CSN, 1000000); // the (ce_pin, csn_pin) connected to the radio
const uint64_t pipeAddress = 0xF0F0F0F0D2LL;
const uint8_t payloadSize = sizeof(Packet);
SPIClass *vspi = new SPIClass(VSPI);
bool role = true;  // true = TX role, false = RX role
SemaphoreHandle_t radioMutex;


//LCD Setup Stuff
LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)
atomic<bool> isTransmitted(false);
SemaphoreHandle_t lcdMutex;
TaskHandle_t lcdTaskHandle = NULL;

volatile bool interruptCounter = false;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  interruptCounter = true;
}

void vLCD(void *pvParameters){
    for(;;){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 

        bool current = isTransmitted.load();

        xSemaphoreTake(lcdMutex, portMAX_DELAY);
        lcd.setCursor(0, 0);

        if(current){
            lcd.print("TRANSMIT SUCCESSFUL");
        }
        else{
            lcd.print("TRANSMISSION FAILED");
        }

        xSemaphoreGive(lcdMutex);

        vTaskDelay(pdMS_TO_TICKS(30)); // Blocks this task, letting Main run
    }
}

void setup() {
    timer = timerBegin(0, 80, true);                //Set 80 prescaler for 1 MHz (1 tick = 1 microsecond)
    timerAttachInterrupt(timer, &onTimer, true);    //Attach interrupt callback
    timerAlarmWrite(timer, 8333, true);             //Set alarm to trigger every 1/120 of a second (8333 microseconds)
    timerAlarmEnable(timer);                        //Enable Alarm

    portMUX_TYPE timerMutex = portMUX_INITIALIZER_UNLOCKED; 

    Serial.begin(115200);
    delay(100);
    Serial.println("ESP32 started");

    //LCD Setup
    Wire.begin(SDA1, SCL1);
    Serial.println("I2C bus started");
    delay(100);

    lcdMutex = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(vLCD, "LCD_Display", 4096, NULL, 1, &lcdTaskHandle, 0); //Setup LCD Thread on Core 0

    xSemaphoreTake(lcdMutex, portMAX_DELAY);
    lcd.init();
    lcd.backlight();
    xSemaphoreGive(lcdMutex);


    //Radio Setup
    radioMutex = xSemaphoreCreateMutex();
    vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
    pinMode(MY_CE, OUTPUT);
    pinMode(MY_CSN, OUTPUT);

    if (!radio.begin(vspi, MY_CE, MY_CSN)) {
        xSemaphoreTake(lcdMutex, portMAX_DELAY);
        lcd.setCursor(0, 0);
        lcd.print("RADIO INIT FAILED");
        while (1) {}  // hold in infinite loop
        xSemaphoreGive(lcdMutex);
    } 

    //Important Stuff
    resetSequence();
    Serial.println("Radio initialized");

    delay(100);

    pinMode(RH, INPUT);
    pinMode(RV, INPUT);
    pinMode(LH, INPUT);
    pinMode(LV, INPUT);
    pinMode(LL, INPUT_PULLUP);
    pinMode(LR, INPUT_PULLUP);
    pinMode(RL, INPUT_PULLUP);
    pinMode(RR, INPUT_PULLUP);

    Serial.println("Setup complete");

    xSemaphoreTake(lcdMutex, portMAX_DELAY);
    lcd.setCursor(0, 0);
    lcd.print("Controller Ready");
    xSemaphoreGive(lcdMutex);
    
    delay(100);
}


//Warning: use only in the main loop thread, not in the LCD thread
bool lastState = false;
bool currentState = false;
int countFail = 0;
int countSuccess = 0;

void loop() {
    if (interruptCounter) {
        unsigned long currentTime = millis(); 

        Packet payload;
        payload.rightHorizontal = analogRead(RH);
        payload.rightVertical = analogRead(RV);
        payload.leftHorizontal = analogRead(LH);
        payload.leftVertical = analogRead(LV);

        payload.rightR = digitalRead(RR);
        payload.rightL = digitalRead(RL);
        payload.leftR = digitalRead(LR);
        payload.leftL = digitalRead(LL);
        Serial.print("Values read!  ");

        Serial.print("RH: "); Serial.print(payload.rightHorizontal);
        Serial.print(", RV: "); Serial.print(payload.rightVertical);
        Serial.print(", LH: "); Serial.print(payload.leftHorizontal);
        Serial.print(", LV: "); Serial.print(payload.leftVertical);
        Serial.print(", RR: "); Serial.print(payload.rightR);
        Serial.print(", RL: "); Serial.print(payload.rightL);
        Serial.print(", LR: "); Serial.print(payload.leftR);
        Serial.print(", LL: "); Serial.print(payload.leftL);
        Serial.println();

        currentState = radio.write(&payload, sizeof(payload));

        // if (radio.write(&payload, sizeof(payload))) {
        //     isTransmitted.store(true);
        //     Serial.println("Transmitted Successfully!");
        //     if (!lcdSentSuccess) {
        //         lcd.clear();
        //         lcd.setCursor(3, 0);
        //         lcd.print("TRANSMITTED SUCCESSFULLY");
        //         lcdSentSuccess = true;
        //         lcdSentFailure = false; // Reset failure flag
        //     }
        // }
        // else {
        //     isTransmitted.store(false);
        //     Serial.println("Transmission Failed");
        //     if (!lcdSentFailure) {
        //         lcd.clear();
        //         lcd.setCursor(0, 0);
        //         lcd.print("TRANSMISSION FAILED");
        //         lcdSentFailure = true;
        //         lcdSentSuccess = false; // Reset success flag
        //     }
        //     radio.flush_tx();
        // }

        Serial.println("");

        if (!radio.isChipConnected()) {
            resetChip();}
        interruptCounter = false;
    }

    //Jump start logic for LCD
    if (millis() < 3000){
        xSemaphoreTake(lcdMutex, portMAX_DELAY);
        if(currentState){
            lcd.setCursor(0, 0);
            lcd.print("TRANSMIT SUCCESSFUL");
        }
        else{
            lcd.setCursor(0, 0);
            lcd.print("TRANSMISSION FAILED");
        }
        xSemaphoreGive(lcdMutex);}
        
    // Count consecutive successes and failures    
    if(currentState == false){
        countSuccess = 0;
        countFail++;} 
    else {
        countFail = 0;
        countSuccess++;}

    //Only update if sure of the state change to avoid flicker
    if (countFail >= 2400) {
        isTransmitted.store(currentState);
        xTaskNotifyGive(lcdTaskHandle);}
    else if (countSuccess >= 2400) {
        isTransmitted.store(currentState); 
        xTaskNotifyGive(lcdTaskHandle);}
    lastState = currentState;
}

void resetSequence(){
    xSemaphoreTake(radioMutex, portMAX_DELAY);
    radio.setChannel(76);
    radio.setDataRate(RF24_1MBPS);
    radio.setCRCLength(RF24_CRC_16);
    radio.setRetries(5, 15);
    radio.setAutoAck(true);
    radio.disableDynamicPayloads();
    radio.flush_tx();
    radio.flush_rx();
    radio.openWritingPipe(pipeAddress);
    radio.setPALevel(RF24_PA_MIN);
    radio.setPayloadSize(payloadSize);
    radio.stopListening();
    xSemaphoreGive(radioMutex);
}

void resetChip(){
    Serial.println("Resetting radio chip...");

    //LCD Message
    xSemaphoreTake(lcdMutex, portMAX_DELAY);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RADIO CHIP ERROR");
    lcd.setCursor(0, 3);
    lcd.print("RESETTING CHIP...");
    xSemaphoreGive(lcdMutex);
    

    xSemaphoreTake(radioMutex, portMAX_DELAY);
    radio.powerDown();
    delay(500);     //Important!!!

    if (!radio.begin(vspi, MY_CE, MY_CSN)) {
        Serial.println("Radio chip re-initialization failed");
        return;
    }

    //Important stuff
    resetSequence();

    if (!radio.isChipConnected()) {
        Serial.println("Radio chip re-initialization failed");
        resetChip();
    } else {
        Serial.println("Radio chip re-initialized successfully");
    }
    xSemaphoreGive(radioMutex);
}
