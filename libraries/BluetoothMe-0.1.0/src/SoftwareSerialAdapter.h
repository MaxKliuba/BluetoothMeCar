#pragma once

#include "SerialAdapter.h"
#include <SoftwareSerial.h>

class SoftwareSerialAdapter : public SerialAdapter {
  public:
    SoftwareSerialAdapter(SoftwareSerial* port, uint32_t speed = 9600, uint16_t timeout = 100) : SerialAdapter(port, timeout) {
      _port = port;

      _port->begin(speed);
      _port->setTimeout(timeout);
    }

    SoftwareSerialAdapter(uint8_t receivePin, uint8_t transmitPin, uint32_t speed = 9600, uint16_t timeout = 100) :
      SoftwareSerialAdapter(new SoftwareSerial(receivePin, transmitPin), speed, timeout) { }

    void write(String message) {
      _port->print(message);
    }

  private:
    SoftwareSerial* _port;
};
