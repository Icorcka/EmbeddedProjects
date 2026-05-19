#pragma once

#include "main.h"
#include <cstdint>

class UartLogger {
public:
    bool begin(uint32_t baud);

    void print(const char *s);
    void println(const char *s);

    void printf(const char *fmt, ...);

private:
    UART_HandleTypeDef huart_{};
};
