#pragma once

#include "IBluetoothAdapter.h"

template <uint16_t SIZE>
class AsyncSerialAdapter : public IBluetoothAdapter {
  public:
    AsyncSerialAdapter(Stream* port, int16_t timeout = 100) : IBluetoothAdapter() {
      _port = port;
      _timeout = timeout;

      _port->setTimeout(_timeout);
    }

    String readUntil(char terminator) {
      if (_port->available()) {
        char ch = _port->read();
        _timer = millis();
        if (ch == terminator) {
          _buffer[_index] = '\0';
          _index = 0;
          return _buffer;
        } else if (_index < SIZE) {
          _buffer[_index] = ch;
          _index++;
        }
      }

      if (millis() - _timer >= _timeout) {
        _timer = millis();
        _buffer[_index] = '\0';
        _index = 0;
        return _buffer;
      }

      return "";
    }

    virtual void write(String message) = 0;

  private:
    Stream* _port;
    char _buffer[SIZE];
    uint16_t _index = 0;
    uint16_t _timeout;
    uint32_t _timer = 0;
};
