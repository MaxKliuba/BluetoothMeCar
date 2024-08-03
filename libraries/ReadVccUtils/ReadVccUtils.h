#pragma once

byte convertVccToPercent(int vcc) {
  if (vcc > 4200) return 100;
  else if (vcc <= 4200 && vcc > 3870) return map(vcc, 4200, 3870, 100, 77);
  else if (vcc <= 3870 && vcc > 3750) return map(vcc, 3870, 3750, 77, 54);
  else if (vcc <= 3750 && vcc > 3680) return map(vcc, 3750, 3680, 54, 31);
  else if (vcc <= 3680 && vcc > 3400) return map(vcc, 3680, 3400, 31, 8);
  else if (vcc <= 3400 && vcc > 3000) return map(vcc, 3400, 3000, 8, 0);
  else return  0;
}

int readVcc(const float vccConst) {
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  uint8_t low  = ADCL;
  uint8_t high = ADCH;
  long result = (high << 8) | low;
  result = vccConst * 1023 * 1000 / result;

  return result;
}