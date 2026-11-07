#pragma once

#include <cstdint>
#include <optional>

#include "adc.h"
#include "stm32f3xx_hal_adc.h"

namespace peripheral {
  class ADC {
    private:
      ADC_TypeDef adc_;

    public:
      ADC(ADC_TypeDef &adc) : adc_(adc) {}

      /**
       * @brief ADCの値を読み取る
       * 
       * @param timeout_ms デフォルトの値は100ms
       * @return std::optional<float> 
       */
      std::optional<float> read(std::uint32 timeout_ms = 100) {

        if (HAL_ADC_Start(&adc_) != HAL_OK) {
          return std::nullopt;
        }

        if(HAL_ADC_PollForConversion(&adc_, timeout_ms) != HAL_OK) {
          HAL_ADC_Stop(&adc_);
          return std::nullopt;
        }

        float value = HAL_ADC_GetValue(&adc_);

        HAL_ADC_Stop(&adc_);

        return value;
      }

      /**
       * @brief キャリブレーションを行う
       * 
       * @return true 
       * @return false 
       */
      bool calibrate() {
        if (HAL_ADCEx_Calibration_Start(&adc_) != HAL_OK) {
          return false;
        }
        return true;
      }    
  };
}