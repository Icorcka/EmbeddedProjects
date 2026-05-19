#include <Arduino.h>

#define RED_LED_PIN (3)
#define BLUE_LED_PIN (18)
#define GREEN_LED_PIN (6)
#define BUTTON_PIN (39)
#define BOOT_BUTTON_PIN (0)
#define BAUDRATE (115200)

const int speeds[] = {1000, 500, 250, 100, 50};
const int numSpeeds = sizeof(speeds) / sizeof(speeds[0]);
const unsigned long longPressTime = 1000;
int speedIndex = 0;

bool buttonPressed = false;
bool bootButtonPressed = false;
unsigned long buttonPressStart = 0;
unsigned long bootButtonPressStart = 0;
unsigned long lastToggleTime = 0;
bool ledsOn = false;

void printSpeed() {
  Serial.print("Speed: ");
  Serial.print(speedIndex + 1);
  Serial.print(" (delay: ");
  Serial.print(speeds[speedIndex]);
  Serial.println("ms)");
}

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(BAUDRATE);
}

void loop() {
  unsigned long now = millis();

  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
    }
  } else {
    if (buttonPressed) {
      speedIndex = (speedIndex + 1) % numSpeeds;
      printSpeed();
      buttonPressed = false;
    }
  }

  if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
    if (!bootButtonPressed) {
      bootButtonPressed = true;
      bootButtonPressStart = now;
    }
  } else {
    if (bootButtonPressed) {
      unsigned long held = now - bootButtonPressStart;
      int step = (held >= longPressTime) ? 2 : 1;
      speedIndex = (speedIndex - step + numSpeeds) % numSpeeds;
      Serial.print(step == 2 ? "LONG" : "SHORT");
      Serial.print(" press -> ");
      printSpeed();
      bootButtonPressed = false;
    }
  }

  if (now - lastToggleTime >= (unsigned long)speeds[speedIndex]) {
    ledsOn = !ledsOn;
    digitalWrite(RED_LED_PIN, ledsOn);
    digitalWrite(BLUE_LED_PIN, ledsOn);
    digitalWrite(GREEN_LED_PIN, ledsOn);
    lastToggleTime = now;
  }
}