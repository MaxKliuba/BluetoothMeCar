#include <AUnit.h> // https://www.arduino.cc/reference/en/libraries/aunit/
#include <BluetoothMeState.h>


test(checkState_StateWithInitialValue_ReturnsTrue) {
  BluetoothMeState<byte> testState = BluetoothMeState<byte>("test", 0);

  bool result = testState.check();

  assertTrue(result);
}

test(checkState_StateWithNewValue_ReturnsTrue) {
  BluetoothMeState<byte> testState = BluetoothMeState<byte>("test", 0);
  testState.check();
  testState.setValue(1);

  bool result = testState.check();

  assertTrue(result);
}

test(checkState_StateWithTheNewSameValue_ReturnsFalse) {
  BluetoothMeState<byte> testState = BluetoothMeState<byte>("test", 0);
  testState.check();
  testState.setValue(0);

  bool result = testState.check();

  assertFalse(result);
}

test(checkState_StateWithCheckedValue_ReturnsFalse) {
  BluetoothMeState<byte> testState = BluetoothMeState<byte>("test", 0);
  testState.check();

  bool result = testState.check();

  assertFalse(result);
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  aunit::TestRunner::run();
}
