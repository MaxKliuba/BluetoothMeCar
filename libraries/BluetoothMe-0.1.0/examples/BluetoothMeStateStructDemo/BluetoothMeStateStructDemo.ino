#include <SoftwareSerial.h>
#include <SoftwareSerialAdapter.h>
// #include <HardwareSerialAdapter.h>
// #include <AsyncSoftwareSerialAdapter.h>
// #include <AsyncHardwareSerialAdapter.h>
#include <BluetoothMe.h>
#include <BluetoothMeState.h>

#define BLUETOOTH_RX 2 // Connect to TX on Bluetooth module
#define BLUETOOTH_TX 3 // Connect to RX on Bluetooth module
SoftwareSerial bluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

SoftwareSerialAdapter bluetoothAdapter(&bluetoothSerial, 9600, 100); // port (or RX, TX), speed, timeout
// HardwareSerialAdapter bluetoothAdapter(&Serial, 9600, 100);
// AsyncSoftwareSerialAdapter<100> bluetoothAdapter(&bluetoothSerial, 9600, 100);
// AsyncHardwareSerialAdapter<100> bluetoothAdapter(&Serial, 9600, 100);

BluetoothMe bluetoothMe(&bluetoothAdapter);

struct {
  BluetoothMeState<byte> param1 = BluetoothMeState<byte>("p1", 0); // tag, initialValue
  BluetoothMeState<byte> param2 = BluetoothMeState<byte>("p2", 0);
  BluetoothMeState<byte> param3 = BluetoothMeState<byte>("p3", 0);
  BluetoothMeState<int> param4 = BluetoothMeState<int>("p4", 0);
  BluetoothMeState<String> param5 = BluetoothMeState<String>("p5", "");
} state;

void onReceive(String tag, String value);

void setup() {
  Serial.begin(9600);
}

void loop() {
  bluetoothMe.listen(onReceive);

  observeState();
}

void onReceive(String tag, String value) {
  if (tag.equals(BluetoothMeState<byte>::GET_STATE_TAG)) {
    // Send state from `param1` to `param3` (inclusive)
    for (BluetoothMeState<byte>* statePtr = &state.param1; statePtr <= &state.param3; ++statePtr) {
      bluetoothMe.send(statePtr->getTag(), (String)statePtr->getValue());
      // delay(50); // May be required for BLE
    }
    bluetoothMe.send(state.param4.getTag(), (String)state.param4.getValue()); // Send `param4` state
    // delay(50);
    bluetoothMe.send(state.param5.getTag(), state.param5.getValue()); // Send `param5` state

    return;
  }

  // Set value for state from `param1` to `param3` (inclusive)
  for (BluetoothMeState<byte>* statePtr = &state.param1; statePtr <= &state.param3; ++statePtr) {
    if (tag.equals(statePtr->getTag())) {
      statePtr->setValue((byte)value.toInt());

      return;
    }
  }

  if (tag.equals(state.param4.getTag())) {
    state.param4.setValue(value.toInt()); // Set value for `param4` state
  } else if (tag.equals(state.param5.getTag())) {
    state.param5.setValue(value); // Set value for `param5` state
  } else {
    Serial.println("[" + tag + "(unknown)]: " + value); // Unknown tag
  }
}

void observeState() {
  // Check and apply state from `param1` to `param3` (inclusive)
  for (BluetoothMeState<byte>* statePtr = &state.param1; statePtr <= &state.param3; ++statePtr) {
    if (statePtr->check()) {
      byte newParamValue = statePtr->getValue();
      Serial.println("[" + statePtr->getTag() + "]: " + (String)newParamValue);
    }
  }

  // Check and apply `param4` state
  if (state.param4.check()) {
    int newParam4Value = state.param4.getValue();
    Serial.println("[" + state.param4.getTag() + "]: " + (String)newParam4Value);
  }

  // Check and apply `param5` state
  if (state.param5.check()) {
    String newParam5Value = state.param5.getValue();
    Serial.println("[" + state.param5.getTag() + "]: " + (String)newParam5Value);
  }
}
