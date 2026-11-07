#pragma once

#include <cstdint>

#include "spi.h"
#include "peripherals/gpio.hpp"

/**
 * @note timeoutの時間は100msで記述
 * 
 */

namespace peripheral {

class SPI {
  private:
    SPI_HandleTypeDef *hspi_;
    GPIO cs_pin_;
    GPIO interrupt_pin_;

  public:
    SPI(SPI_HandleTypeDef *hspi, 
      GPIO_TypeDef* cs_port,
      std::uint16_t cs_pin,
      GPIO_TypeDef* interrupt_port,
      std::uint16_t interrupt_pin) : hspi_(hspi), cs_pin_(cs_port, cs_pin), interrupt_pin_(interrupt_port, interrupt_pin) {}

    bool transmit(uint8_t *data, uint16_t size, uint32_t timeout = 100) {
        csPin_.low(); 
        HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_, data, size, timeout);
        csPin_.high(); 
        return status == HAL_OK;
    }

    bool receive(uint8_t *data, uint16_t size, uint32_t timeout = 100) {
        csPin_.low(); 
        HAL_StatusTypeDef status = HAL_SPI_Receive(hspi_, data, size, timeout);
        csPin_.high(); 
        return status == HAL_OK;
    }

    bool transmitReceive(uint8_t *txData, uint8_t *rxData, uint16_t size, uint32_t timeout = 100) {
        csPin_.low(); 
        HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_, txData, rxData, size, timeout);
        csPin_.high(); 
        return status == HAL_OK;
    }

    bool isInterruptActive() {
        return interruptPin_.read();
    }
}

}