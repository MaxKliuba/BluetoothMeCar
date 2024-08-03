#pragma once

class IBluetoothAdapter {
  public:
    virtual String readUntil(char terminator) = 0;

    virtual void write(String message) = 0;
};
