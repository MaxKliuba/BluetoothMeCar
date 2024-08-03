#pragma once

#include "IBluetoothAdapter.h"

class SerialAdapter : public IBluetoothAdapter {
  public:
    SerialAdapter(Stream* port, int16_t timeout = 100) : IBluetoothAdapter(){
      _port = port;
      
      _port->setTimeout(timeout);
    }

    String readUntil(char terminator) {
      if (_port->available()) {
        return _port->readStringUntil(terminator);
      }

      return "";
    }

    virtual void write(String message) = 0;

  private:
    Stream* _port;
};
