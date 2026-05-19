#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>

#define RED_LED_PIN (3)
#define BLUE_LED_PIN (18)
#define GREEN_LED_PIN (6)
#define BLINK_DELAY_MS (1000)
#define FAST_DELAY_MS (250)
#define SLOW_DELAY_MS (2000)

typedef void (*LightMode)();

extern unsigned long previousMillis;
extern int ledState;
extern LightMode modes[];
extern const int NUM_MODES;
extern int currentMode;
extern LightMode *currentModePtr;

void policeLights();
void trafficLights();
void dualLights();
void allLights();
void fastTrafficLights();
void slowTrafficLights();

#endif
