#pragma once

#define INFINITE_BLINK_COUNT 255

enum LedState { LED_STATE_OFF, LED_STATE_ON, LED_STATE_BLINK };

class SimpleLED {
  public:
    SimpleLED(uint8_t pin, bool offState = false) {
      _pin = pin;
      _offState = offState;

      pinMode(_pin, OUTPUT);
      off();
    }

    uint8_t tick() {
      if (_state == LED_STATE_OFF || _state == LED_STATE_ON) {
        return _state;
      }

      if (_state == LED_STATE_BLINK) {
        if (_blinkCounter <= 0) {
          off();

          return _state;
        }

        if (millis() - _blinkTimer >= (_isOn ? _onPeriodMs : _offPeriodMs)) {
          _blinkTimer = millis();
          _isOn ? _off() : _on();

          if (!_isOn && _blinkCounter != INFINITE_BLINK_COUNT) {
            _blinkCounter--;
          }
        }
      }

      return _state;
    }

    void on() {
      _state = LED_STATE_ON;
      _on();

    }

    void off() {
      _state = LED_STATE_OFF;
      _off();
    }

    void set(bool isOn) {
      isOn ? on() : off();
    }

    void switchLed() {
      _isOn ? off() : on();
    }

    void startBlinking(uint32_t onPeriodMs, uint32_t offPeriodMs, uint8_t count = INFINITE_BLINK_COUNT) {
      _off();

      _state = LED_STATE_BLINK;
      _onPeriodMs = onPeriodMs;
      _offPeriodMs = offPeriodMs;
      _blinkCounter = count;
      _blinkTimer = 0;
    }

    uint8_t getState() {
      return _state;
    }

  private:
    uint8_t _pin;
    bool _offState;
    bool _isOn = false;
    uint8_t _state = LED_STATE_OFF;

    unsigned long _onPeriodMs = 0;
    unsigned long _offPeriodMs = 0;
    unsigned long _blinkTimer = 0;
    uint8_t _blinkCounter = 0;

    void _on() {
      _isOn = true;
      digitalWrite(_pin, !_offState);
    }

    void _off() {
      _isOn = false;
      digitalWrite(_pin, _offState);
    }
};