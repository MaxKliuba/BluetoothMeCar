#pragma once

#include "SerialAdapter.h"

class HardwareSerialAdapter : public SerialAdapter {
  public:
    HardwareSerialAdapter(HardwareSerial* port, uint32_t speed = 9600, uint16_t timeout = 100) : SerialAdapter(port, timeout) {
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
