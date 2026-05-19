#include "led.h"
#include "calibration.h"

#define PWM_FREQ 5000
#define PWM_RESOLUTION 8
#define FADE_STEP 10
#define HYSTERESIS 10

static uint8_t currentBrightness = 0;
static uint8_t lastBrightness = 0;
const uint8_t GAMMA_TABLE[256] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
      1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
      2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
      5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
     10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
     17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
     25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
     37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
     51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
     69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
     90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
    115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
    144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
    177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
    215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
  };

void ledInit() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    ledcSetup(i, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LED_PINS[i], i);
  }
}

void fadeLed(uint8_t targetBrightness) {
  if(currentBrightness < targetBrightness) {
    currentBrightness = min((int)currentBrightness + FADE_STEP, (int)targetBrightness);
  } else if (currentBrightness > targetBrightness) {
    currentBrightness = max((int)currentBrightness - FADE_STEP, (int)targetBrightness);
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    ledcWrite(i, GAMMA_TABLE[currentBrightness]);
  }
}

uint8_t normilizeBrightness(uint16_t ldrValue){
  int32_t brightness = map(ldrValue, getCalibratedMin(), getCalibratedMax(), 0, 255);
  brightness = constrain(brightness, 0, 255);

  if (abs(brightness - (int32_t)lastBrightness) < HYSTERESIS){
    return lastBrightness;
  } else {
    lastBrightness = (uint8_t)brightness;
    return lastBrightness;
  }
}
