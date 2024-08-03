#pragma once

#include "AsyncSerialAdapter.h"
#include <SoftwareSerial.h>

template <uint16_t SIZE>
class AsyncSoftwareSerialAdapter : public AsyncSerialAdapter<SIZE> {
  public:
    AsyncSoftwareSerialAdapter(SoftwareSerial* port, uint32_t speed = 9600, uint16_t timeout = 100) : AsyncSerialAdapter<SIZE>(port, timeout) {
      _port = port;

      _port->begin(speed);
      _port->setTimeout(timeout);
    }

    AsyncSoftwareSerialAdapter(uint8_t receivePin, uint8_t transmitPin, uint32_t speed = 9600, uint16_t timeout = 100) :
      AsyncSoftwareSerialAdapter(new SoftwareSerial(receivePin, transmitPin), speed, timeout) { }

    void write(String message) {
      _port->print(message);
    }

  private:
    SoftwareSerial* _port;
};
