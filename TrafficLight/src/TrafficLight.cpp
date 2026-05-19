#include "TrafficLight.h"
#include "Config.h"
#include "esp_timer.h"

TrafficLight::TrafficLight()
    : green_(cfg::PIN_GREEN),
      yellow_(cfg::PIN_YELLOW),
      red_(cfg::PIN_RED),
      state_(State::Red),
      night_mode_(false),
      state_start_us_(0),
      last_blink_us_(0) {}

TrafficLight::State TrafficLight::state() const { return state_; }
bool TrafficLight::nightMode() const            { return night_mode_; }

void TrafficLight::start() {
    enter(State::Red, esp_timer_get_time());
}

void TrafficLight::allOff() {
    green_.off();
    yellow_.off();
    red_.off();
}

void TrafficLight::enter(State s, int64_t now_us) {
    state_ = s;
    state_start_us_ = now_us;
    last_blink_us_  = now_us;
    allOff();
    switch (s) {
        case State::Green:      green_.on();  break;
        case State::GreenBlink: green_.on();  break;
        case State::Yellow:     yellow_.on(); break;
        case State::Red:        red_.on();    break;
        case State::RedYellow:  red_.on(); yellow_.on(); break;
        case State::Night:      yellow_.on(); break;
    }
}

void TrafficLight::toggleNight() {
    const int64_t now = esp_timer_get_time();
    if (!night_mode_) {
        night_mode_ = true;
        enter(State::Night, now);
    } else {
        night_mode_ = false;
        enter(State::Red, now);
    }
}

void TrafficLight::tick(int64_t now_us) {
    const uint32_t elapsed_ms =
        static_cast<uint32_t>((now_us - state_start_us_) / 1000);

    switch (state_) {
        case State::Green:
            if (elapsed_ms >= cfg::T_GREEN_MS)
                enter(State::GreenBlink, now_us);
            break;

        case State::GreenBlink: {
            const uint32_t since = static_cast<uint32_t>(
                (now_us - last_blink_us_) / 1000);
            if (since >= cfg::GREEN_BLINK_HALF_MS) {
                green_.toggle();
                last_blink_us_ = now_us;
            }
            if (elapsed_ms >= cfg::T_GREEN_BLINK_MS)
                enter(State::Yellow, now_us);
            break;
        }

        case State::Yellow:
            if (elapsed_ms >= cfg::T_YELLOW_MS)
                enter(State::Red, now_us);
            break;

        case State::Red:
            if (elapsed_ms >= cfg::T_RED_MS)
                enter(State::RedYellow, now_us);
            break;

        case State::RedYellow:
            if (elapsed_ms >= cfg::T_RED_YELLOW_MS)
                enter(State::Green, now_us);
            break;

        case State::Night: {
            const uint32_t since = static_cast<uint32_t>(
                (now_us - last_blink_us_) / 1000);
            if (since >= cfg::NIGHT_BLINK_HALF_MS) {
                yellow_.toggle();
                last_blink_us_ = now_us;
            }
            break;
        }
    }
}
