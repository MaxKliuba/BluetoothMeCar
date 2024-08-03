#pragma once

#include "IBluetoothAdapter.h"

class BluetoothMe {
  public:
    static const char TAG_TERMINATOR = '/';
    static const char MESSAGE_TERMINATOR = '\n';

    BluetoothMe(IBluetoothAdapter* adapter) {
      _adapter = adapter;
    }

    String listen(void(*onReceive)(String, String) = NULL) {
      String message = _adapter->readUntil(MESSAGE_TERMINATOR);

      if (!message.equals("") && onReceive != NULL) {
        int tagIndex = message.indexOf(TAG_TERMINATOR);
        String tag = tagIndex > 0 ? message.substring(0, tagIndex) : "";
        String value = message.substring(tagIndex + 1);

        onReceive(tag, value);
      }

      return message;
    }

    void send(String message) {
      _adapter->write(message + MESSAGE_TERMINATOR);
    }

    void send(String tag, String value) {
      send(createMessage(tag, value));
    }

    void exec(String command) {
      _adapter->write(command);
    }

  private:
    IBluetoothAdapter* _adapter;

    String createMessage(String tag, String value) {
      return tag + TAG_TERMINATOR + value;
    }
};
