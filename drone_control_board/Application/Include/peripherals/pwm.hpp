#pragma once

#include <cstdint>

#include "tim.h"

namespace peripheral {
  class PWM {
    private:
      TIM_HandleTypeDef *htim_;
      uint32_t channel_;

    public:
      PWM(TIM_HandleTypeDef *htim, uint32_t channel) : htim_(htim), channel_(channel) {}

      bool start() {
        return HAL_TIM_PWM_Start(htim_, channel_) == HAL_OK;
      }

      void stop() {
        return HAL_TIM_PWM_Stop(htim_, channel_) == HAL_OK;
      }

      /**
       * @brief パルスを指定する 0.0 ~ 1.0まで
       * 
       * @param dutyCycle 
       */
      void setDutyCycle(float dutyCycle) {
        if (dutyCycle < 0.0f || dutyCycle > 1.0f) {
          return; // Invalid duty cycle
        }
        uint32_t pulse = static_cast<uint32_t>(dutyCycle * (__HAL_TIM_GET_AUTORELOAD(htim_) + 1));
        __HAL_TIM_SET_COMPARE(htim_, channel_, pulse);
      }
  };
}