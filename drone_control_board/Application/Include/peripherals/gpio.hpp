#pragma once

#include "stm32f3xx_hal.h"

namespace peripheral {

class GPIO {
  private:
    GPIO_TypeDef *port_;
    uint16_t pin_;
    bool state_; // high = true, low = false
  
  public:
    GPIO(GPIO_TypeDef *port, uint16_t pin) : port_(port), pin_(pin) {}

    void high() {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        state_ = true;
    }

    void low() {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        state_ = false;
    }

    void toggle() {
        HAL_GPIO_TogglePin(port, pin);
        state_ = !state_;
    }

    bool read() {
        return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
    }
  };
}