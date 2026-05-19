#include "AdcReader.hpp"

/* VREFINT_CAL_ADDR is supplied by stm32f4xx_ll_adc.h as ((uint16_t*) 0x1FFF7A2AU)
 * — raw ADC value of VREFINT measured at the factory with VDDA=3.3V, Ta=30C.
 */

bool AdcReader::begin()
{
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio{};
    gpio.Pin  = GPIO_PIN_0;
    gpio.Mode = GPIO_MODE_ANALOG;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    hadc_.Instance                   = ADC1;
    hadc_.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc_.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc_.Init.ScanConvMode          = DISABLE;
    hadc_.Init.ContinuousConvMode    = DISABLE;
    hadc_.Init.DiscontinuousConvMode = DISABLE;
    hadc_.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc_.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc_.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc_.Init.NbrOfConversion       = 1;
    hadc_.Init.DMAContinuousRequests = DISABLE;
    hadc_.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    return HAL_ADC_Init(&hadc_) == HAL_OK;
}

uint32_t AdcReader::readRaw(uint32_t channel)
{
    ADC_ChannelConfTypeDef cfg{};
    cfg.Channel      = channel;
    cfg.Rank         = 1;
    cfg.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc_, &cfg) != HAL_OK) {
        return 0U;
    }

    HAL_ADC_Start(&hadc_);
    HAL_ADC_PollForConversion(&hadc_, HAL_MAX_DELAY);
    uint32_t value = HAL_ADC_GetValue(&hadc_);
    HAL_ADC_Stop(&hadc_);
    return value;
}

uint32_t AdcReader::getVDDA_mV()
{
    uint32_t vrefint_raw = readRaw(ADC_CHANNEL_VREFINT);
    if (vrefint_raw == 0U) {
        return VDDA_CAL_MV;
    }
    return (VDDA_CAL_MV * static_cast<uint32_t>(*VREFINT_CAL_ADDR)) / vrefint_raw;
}
