#pragma once

#include <array>
#include <cstdint>

#include "can.h"

namespace peripheral {

struct CANFrame {
  std::uint32_t id; // 11-bit or 29-bit identifier
  std::uint8_t length;
  bool extended;
  bool remote;
  std::array<std::uint8_t, 8> data; 
}

class CANBus {
  private:
    CAN_HandleTypeDef *hcan_;

    bool configure_accept_all_filter() noexcept
    {
        CAN_FilterTypeDef filter{};

        filter.FilterBank = 0;
        filter.FilterMode = CAN_FILTERMODE_IDMASK;
        filter.FilterScale = CAN_FILTERSCALE_32BIT;

        filter.FilterIdHigh = 0;
        filter.FilterIdLow = 0;
        filter.FilterMaskIdHigh = 0;
        filter.FilterMaskIdLow = 0;

        filter.FilterFIFOAssignment = CAN_RX_FIFO0;
        filter.FilterActivation = ENABLE;
        filter.SlaveStartFilterBank = 14;

        return HAL_CAN_ConfigFilter(
            &handle_,
            &filter
        ) == HAL_OK;
    }

  public:
    CANBus(CAN_HandleTypeDef *hcan) : hcan_(hcan) {}

    bool start() {
        if (HAL_CAN_Start(hcan_) != HAL_OK) {
            return false;
        }
        return configure_accept_all_filter();
    }

    bool stop() {
        return HAL_CAN_Stop(hcan_) == HAL_OK;
    }

    bool transmit(const CANFrame &frame, uint32_t timeout = 100) {
        if(frame.length > 8) {
            return false; // Invalid frame length
        }

        if(!frame.extended && frame.id > 0x7FF) {
            return false; // Invalid standard ID
        }

        if(frame.extended && frame.id > 0x1FFFFFFF) {
            return false; // Invalid extended ID
        }

        CAN_TxHeaderTypeDef txHeader;
        txHeader.StdId = frame.id;
        txHeader.ExtId = frame.id;
        txHeader.RTR = frame.remote ? CAN_RTR_REMOTE : CAN_RTR_DATA;
        txHeader.IDE = frame.extended ? CAN_ID_EXT : CAN_ID_STD;
        txHeader.DLC = frame.length;

        uint32_t mailbox;
        HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(hcan_, &txHeader, frame.data.data(), &mailbox);
        return status == HAL_OK;
    }

    bool receive(CANFrame &frame, uint32_t timeout = 100) {
        CAN_RxHeaderTypeDef rxHeader;
        HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(hcan_, CAN_RX_FIFO0, &rxHeader, frame.data.data());
        
        if (status == HAL_OK) {
            frame.id = rxHeader.StdId;
            frame.length = rxHeader.DLC;
            frame.extended = (rxHeader.IDE == CAN_ID_EXT);
            frame.remote = (rxHeader.RTR == CAN_RTR_REMOTE);
            return true;
        }
        return false;
    }
}

}