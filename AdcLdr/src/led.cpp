#include "led.h"

void ledInit() {
  pinMode(LED_PIN, OUTPUT);
}

void updateLed(int ldrValue) {
  if (ldrValue > LDR_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
