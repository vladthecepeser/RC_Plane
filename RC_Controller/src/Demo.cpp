#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RF24.h>
 

using namespace std;

// Pin definitions
    //Joysticks
    int RH = 35;
    int RV = 34;
    int LH = 39;
    int LV = 36;

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

//LCD Setup Stuff
LiquidCrystal_I2C lcd(address, 20, 4); // set LCD (address, columns, rows)


void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 started");

    Wire.begin(SDA1, SCL1);
    Serial.println("I2C bus started");
    delay(1000);

    pinMode(RH, INPUT);
    pinMode(RV, INPUT);
    pinMode(LH, INPUT);
    pinMode(LV, INPUT);
    pinMode(LL, INPUT_PULLUP);
    pinMode(LR, INPUT_PULLUP);
    pinMode(RL, INPUT_PULLUP);
    pinMode(RR, INPUT_PULLUP);

        //Initialize the LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Controller Ready");
    Serial.println("Setup complete");
    delay(3000);
}

bool lcdSentSuccess = false;
bool lcdSentFailure = false;
int location;

void loop() {
    int rightHorizontal = analogRead(RH);
    int rightVertical = analogRead(RV);
    int leftHorizontal = analogRead(LH);
    int leftVertical = analogRead(LV);

    int rightR = digitalRead(RR);
    int rightL = digitalRead(RL);
    int leftR = digitalRead(LR);
    int leftL = digitalRead(LL);
    Serial.print("Values read!  ");
    // Serial.print("RH: "); Serial.print(rightHorizontal);
    // Serial.print(", RV: "); Serial.print(rightVertical);
    // Serial.print(", LH: "); Serial.print(leftHorizontal);
    // Serial.print(", LV: "); Serial.print(leftVertical);
    // Serial.print(", RR: "); Serial.print(rightR);
    // Serial.print(", RL: "); Serial.print(rightL);
    // Serial.print(", LR: "); Serial.print(leftR);
    // Serial.print(", LL: "); Serial.print(leftL);
    // Serial.println();

    int locationNew = map(rightHorizontal, 0, 4095, 0, 5);
    locationNew = locationNew*4+2;

    if (locationNew != location) {
        location = locationNew;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("      Position");
        lcd.setCursor(0, 2);
        for (int i = 0; i < 20; i++) {
            if (i == location) {
                lcd.print("X");
            } else {
                lcd.print("-");
            }
        }
    }

    delay(10); // Delay for readability
}