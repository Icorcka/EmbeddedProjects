#include "lights.h"

unsigned long previousMillis = 0;
int ledState = 0;

LightMode modes[] = {policeLights, trafficLights, dualLights, allLights, fastTrafficLights, slowTrafficLights};
const int NUM_MODES = sizeof(modes) / sizeof(modes[0]);
int currentMode = 0;
LightMode *currentModePtr = modes;

void policeLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= BLINK_DELAY_MS) {
    previousMillis = currentMillis;
    ledState = (ledState + 1) % 2;
    digitalWrite(RED_LED_PIN, ledState == 0 ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, ledState == 1 ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
}

void trafficLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= BLINK_DELAY_MS) {
    previousMillis = currentMillis;
    ledState = (ledState + 1) % 3;
    digitalWrite(RED_LED_PIN, ledState == 0 ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, ledState == 1 ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, ledState == 2 ? HIGH : LOW);
  }
}

void allLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= BLINK_DELAY_MS) {
    previousMillis = currentMillis;
    ledState = (ledState + 1) % 2;
    digitalWrite(RED_LED_PIN, ledState == 0 ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, ledState == 0 ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, ledState == 0 ? HIGH : LOW);
  }
}

void slowTrafficLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= SLOW_DELAY_MS) {
    previousMillis = currentMillis;
    ledState = (ledState + 1) % 3;
    digitalWrite(RED_LED_PIN, ledState == 0 ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, ledState == 1 ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, ledState == 2 ? HIGH : LOW);
  }
}

void fastTrafficLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= FAST_DELAY_MS) {
    previousMillis = currentMillis;
    ledState = (ledState + 1) % 3;
    digitalWrite(RED_LED_PIN, ledState == 0 ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, ledState == 1 ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, ledState == 2 ? HIGH : LOW);
  }
}

void dualLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= BLINK_DELAY_MS) {
    previousMillis = currentMillis;
    ledState = (ledState + 1) % 3;
    // red+blue, blue+green, green+red
    digitalWrite(RED_LED_PIN, (ledState == 0 || ledState == 2) ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, (ledState == 0 || ledState == 1) ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, (ledState == 1 || ledState == 2) ? HIGH : LOW);
  }
}
