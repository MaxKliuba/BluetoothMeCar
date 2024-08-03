#pragma once

#include "AsyncSerialAdapter.h"

template <uint16_t SIZE>
class AsyncHardwareSerialAdapter : public AsyncSerialAdapter<SIZE> {
  public:
    AsyncHardwareSerialAdapter(HardwareSerial* port, uint32_t speed = 9600, uint16_t timeout = 100) : AsyncSerialAdapter<SIZE>(port, timeout) {
      _port = port;

      _port->begin(speed);
      _port->setTimeout(timeout);
    }

    void write(String message) {
      _port->print(message);
    }

  private:
    HardwareSerial* _port;
};
