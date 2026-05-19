#include "calibration.h"
#include "adc.h"

static uint16_t calMin = 4095;
static uint16_t calMax = 0;

void calibrate() {
    Serial.println("Calibrating... cover and uncover LDR!");
    uint32_t start = millis();

    while(millis() - start < CALIBRATION_TIME) {
        uint16_t raw = readRaw();
        if(raw < calMin) calMin = raw;
        if(raw > calMax) calMax = raw;
        delay(10);
    }
    Serial.print("Calibration done. Min: ");
    Serial.print(calMin);
    Serial.print(" Max: ");
    Serial.println(calMax);
}

uint16_t getCalibratedMin() { return calMin; }
uint16_t getCalibratedMax() { return calMax; }
