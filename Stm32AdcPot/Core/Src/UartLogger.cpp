#include "UartLogger.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>

bool UartLogger::begin(uint32_t baud)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio{};
    gpio.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &gpio);

    huart_.Instance          = USART1;
    huart_.Init.BaudRate     = baud;
    huart_.Init.WordLength   = UART_WORDLENGTH_8B;
    huart_.Init.StopBits     = UART_STOPBITS_1;
    huart_.Init.Parity       = UART_PARITY_NONE;
    huart_.Init.Mode         = UART_MODE_TX_RX;
    huart_.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart_.Init.OverSampling = UART_OVERSAMPLING_16;
    return HAL_UART_Init(&huart_) == HAL_OK;
}

void UartLogger::print(const char *s)
{
    HAL_UART_Transmit(&huart_, reinterpret_cast<const uint8_t *>(s),
                      std::strlen(s), HAL_MAX_DELAY);
}

void UartLogger::println(const char *s)
{
    print(s);
    print("\r\n");
}

void UartLogger::printf(const char *fmt, ...)
{
    char buf[160];
    std::va_list ap;
    va_start(ap, fmt);
    int n = std::vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0) {
        if (static_cast<size_t>(n) >= sizeof(buf)) n = sizeof(buf) - 1;
        HAL_UART_Transmit(&huart_, reinterpret_cast<uint8_t *>(buf),
                          static_cast<uint16_t>(n), HAL_MAX_DELAY);
    }
}
