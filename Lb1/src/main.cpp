#include <Arduino.h>
#include "lights.h"

#define BUTTON_PIN (39)
#define BAUDRATE (115200)

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  Serial.begin(BAUDRATE);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button pressed");
    currentMode = (currentMode + 1) % NUM_MODES;
    currentModePtr = &modes[currentMode];
    ledState = 0;
    delay(300);
  }

  (*currentModePtr)();
}
