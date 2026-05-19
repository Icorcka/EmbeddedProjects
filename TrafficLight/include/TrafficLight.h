#pragma once

#include "Led.h"
#include <cstdint>

class TrafficLight {
public:
    enum class State {
        Green,
        GreenBlink,
        Yellow,
        Red,
        RedYellow,
        Night,
    };

    TrafficLight();

    void start();
    void tick(int64_t now_us);
    void toggleNight();

    State state() const;
    bool  nightMode() const;

private:
    void enter(State s, int64_t now_us);
    void allOff();

    Led green_;
    Led yellow_;
    Led red_;

    State   state_;
    bool    night_mode_;
    int64_t state_start_us_;
    int64_t last_blink_us_;
};
