#include "app.h"
#include "main.h"
#include "AdcReader.hpp"
#include "UartLogger.hpp"

namespace {
UartLogger logger;
AdcReader  adc;
}  // namespace

extern "C" void App_Init(void)
{
    logger.begin(115200);
    adc.begin();

    logger.print("\r\n--- STM32 ADC potentiometer (PA0, 12-bit, VREF=VDDA) ---\r\n");
    logger.print(" RAW   U_manual(mV)   U_cali(mV)   Error(%)\r\n");
    logger.print("--------------------------------------------\r\n");
}

extern "C" void App_Run(void)
{
    while (true) {
        uint32_t raw      = adc.readRaw(ADC_CHANNEL_0);
        uint32_t vdda_mv  = adc.getVDDA_mV();

        /* U_manual: trust nominal VDDA = 3.300 V. Keep 1 decimal of mV via *10. */
        uint32_t u_manual_x10 = (raw * AdcReader::VDDA_CAL_MV * 10U) / AdcReader::FULL_SCALE;
        uint32_t u_man_int    = u_manual_x10 / 10U;
        uint32_t u_man_frac   = u_manual_x10 % 10U;

        /* U_cali: use VDDA derived from VREFINT. */
        uint32_t u_cali = (raw * vdda_mv) / AdcReader::FULL_SCALE;

        /* Error(%) with 2 decimals via integer math. */
        int32_t diff_mv = static_cast<int32_t>(u_man_int) - static_cast<int32_t>(u_cali);
        if (diff_mv < 0) diff_mv = -diff_mv;
        uint32_t err_x100 = u_cali ? (static_cast<uint32_t>(diff_mv) * 10000U) / u_cali : 0U;

        logger.printf("%4lu     %4lu.%lu        %4lu        %2lu.%02lu\r\n",
                      static_cast<unsigned long>(raw),
                      static_cast<unsigned long>(u_man_int),
                      static_cast<unsigned long>(u_man_frac),
                      static_cast<unsigned long>(u_cali),
                      static_cast<unsigned long>(err_x100 / 100U),
                      static_cast<unsigned long>(err_x100 % 100U));

        HAL_Delay(100);
    }
}
