#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RF24.h>
#include <string>
 

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


//LCD Setup Stuff
LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)

void setup() {
    //LCD Setup
    Wire.begin(SDA1, SCL1);
    Serial.println("I2C bus started");
    delay(100);

    lcd.init();
    lcd.backlight();


    //Radio Setup
    vspi->begin(MY_SCK, MY_MISO, MY_MOSI, MY_CSN);
    pinMode(MY_CE, OUTPUT);
    pinMode(MY_CSN, OUTPUT);

    Serial.begin(115200);
    delay(100);
    Serial.println("ESP32 started");

    if (!radio.begin(vspi, MY_CE, MY_CSN)) {
        lcd.setCursor(0, 0);
        lcd.print("RADIO INIT FAILED");
        while (1) {}  // hold in infinite loop
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

    lcd.setCursor(0, 0);
    lcd.print("Controller Ready");
    
    delay(100);
}

bool lcdSentSuccess = false;
bool lcdSentFailure = false;

void loop() {
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

    Serial.println("past0");

    bool tx_ok = false;
    bool tx_fail = false;
    bool rx_ready = false;

    radio.startWrite(&payload, sizeof(payload), false);

    uint32_t start = millis();

    while (!tx_ok && !tx_fail)
    {
        radio.whatHappened(tx_ok, tx_fail, rx_ready);

        if (millis() - start > 200)
        {
            Serial.print("status=");
            radio.printPrettyDetails();
            Serial.println("Manual timeout");
            Serial.print("Connected=");
            Serial.println(radio.isChipConnected());

            resetChip();
            break;
        }
    }

    if (tx_ok)
    {
        Serial.println("TX success");
    }

    if (tx_fail)
    {
        Serial.println("TX failed (MAX_RT)");
    }

    if (rx_ready)
    {
        Serial.println("Ack payload available");
    }

    tx_ok = false;
    tx_fail = false;
    rx_ready = false;
    
    // if (radio.write(&payload, sizeof(payload))) {
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

    // Serial.println("");

    // if (!radio.isChipConnected()) {
    //     resetChip();
    // }

    delay(100); // Delay for readability
}

void resetSequence(){
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
}

void resetChip(){
    Serial.println("Resetting radio chip...");

    //LCD Message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RADIO CHIP ERROR");
    lcd.setCursor(0, 3);
    lcd.print("RESETTING CHIP...");
    
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

    delay(100);
}
