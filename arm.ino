//LCD
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#include "src/arm.h"


#define I2C_ADDR 0x3F // Define I2C Address for controller

#define LED_OFF 0
#define LED_ON 1
/*  */

#define BACKLIGHT_PIN 3
#define En_pin        2
#define Rw_pin        1
#define Rs_pin        0
#define D4_pin        4
#define D5_pin        5
#define D6_pin        6
#define D7_pin        7

LiquidCrystal_I2C lcd(
    I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin
);

void lcdInit() {
    lcd.begin(16, 2);  // initialize the lcd
    // Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(LED_ON);
}

void lcdPrint(int degree, int pot) {
    // Reset the display
    lcd.clear();

    // Print on the LCD
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print(degree);
    lcd.setCursor(5, 1);
    lcd.print(pot);
}

bool on = false;
Arm arm = Arm();


void singleJointFromBytes() {
    byte command[2];
    if (Serial.available() > 1) {
      Serial.readBytes(command, 2);

        const int joint = command[0];
        const int degree = command[1];

        Serial.print("joint: ");
        Serial.print(joint);
        Serial.print(" deg: ");
        Serial.println(degree);

        lcdPrint(joint, degree);

        //arm.setTargetPosition(joint, degree);
    }
}

#define JOINTS_COUNT 6

void lcd6digits(byte *digits) {
    // Reset the display
    lcd.clear();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print(digits[0]);
    lcd.setCursor(5, 0);
    lcd.print(digits[1]);
    lcd.setCursor(10, 0);
    lcd.print(digits[2]);

    lcd.setCursor(0, 1);
    lcd.print(digits[3]);
    lcd.setCursor(5, 1);
    lcd.print(digits[4]);
    lcd.setCursor(10, 1);
    lcd.print(digits[5]);
}

void allJointsFromBytes(byte *values) {
    if (Serial.available() >= JOINTS_COUNT) {
        Serial.readBytes(values, JOINTS_COUNT);

        Serial.print(values[0]);
        Serial.print(values[1]);
        Serial.print(values[2]);
        Serial.print(values[3]);
        Serial.print(values[4]);
        Serial.print(values[5]);
        Serial.println("\n");
        lcd6digits(values);

        arm.setDestinationValues();
    }
}

//expects 2 int values
//eg: m1:180
void singleJointFromString() {
    while (Serial.available() > 0) {
        const int joint = Serial.parseInt();
        const int degree = Serial.parseInt();

        Serial.print("joint: ");
        Serial.print(joint);
        Serial.print(" deg: ");
        Serial.println(degree);

        lcdPrint(joint, degree);

        //arm.setTargetPosition(joint, degree);
    }
}

#define WAIST_PIN 3
#define SHOULDER_PIN 5
#define ELBOW_PIN 6
#define WRIST_PITCH_PIN 9
#define WRIST_ROLL_PIN 10
#define END_EFFECTOR_PIN 11

void setUpArm() {
    arm.joints[0].actuator.attach(WAIST_PIN);

    arm.joints[1].invert = true;//TODO: invert shoulder 180 - value
    arm.joints[1].position = 180;
    arm.joints[1].actuator.write(0);
    arm.joints[1].actuator.attach(SHOULDER_PIN);

    arm.joints[2].position = 0;
    arm.joints[2].actuator.write(0);
    arm.joints[2].actuator.attach(ELBOW_PIN);

    arm.joints[3].actuator.attach(WRIST_PITCH_PIN);

    arm.joints[4].actuator.attach(WRIST_ROLL_PIN);

    arm.joints[1].invert = true;
    arm.joints[5].actuator.attach(END_EFFECTOR_PIN);
}

void setup() {
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, on);

    lcdInit();

    setUpArm();
}

void loop() {
    /* singleJointFromBytes(); */

    if (arm.simulationFinished) {
        digitalWrite(LED_BUILTIN, false);
        allJointsFromBytes(arm.destinationValues);
    } else {
        digitalWrite(LED_BUILTIN, true);
        /* lcd6digits(arm.positionValues); */
        arm.step(millis());
    }

}
