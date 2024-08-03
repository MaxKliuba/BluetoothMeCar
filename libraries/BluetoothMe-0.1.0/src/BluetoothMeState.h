#pragma once

template <typename T>
class BluetoothMeState {
  public:
    inline static const String GET_STATE_TAG = "get";

    BluetoothMeState(String tag, T initialValue = T()) {
      _tag = tag;
      _value = initialValue;
    }

    bool check() {
      bool result = !(_value == _prevValue);
      _prevValue = _value;

      return result;
    }

    String getTag() {
      return _tag;
    }

    T getValue() {
      return _value;
    }

    void setValue(T value) {
      _prevValue = _value;
      _value = value;
    }

  private:
    String _tag;
    T _value;
    T _prevValue;
};
