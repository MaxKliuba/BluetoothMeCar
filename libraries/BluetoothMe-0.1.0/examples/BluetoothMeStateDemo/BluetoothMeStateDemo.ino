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

BluetoothMeState<bool> singleState = BluetoothMeState<bool>("tag", false); // tag, initialValue
// BluetoothMeState<byte> singleState = BluetoothMeState<byte>("tag", 0);
// BluetoothMeState<int> singleState = BluetoothMeState<int>("tag", 0);
// BluetoothMeState<float> singleState = BluetoothMeState<float>("tag", 0.0);
// BluetoothMeState<char> singleState = BluetoothMeState<char>("tag", ' ');
// BluetoothMeState<String> singleState = BluetoothMeState<String>("tag", "");

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
    // Send state
    bluetoothMe.send(singleState.getTag(), (String)singleState.getValue());

    return;
  }

  if (tag.equals(singleState.getTag())) {
    // Set value
    singleState.setValue((bool)value.toInt()); // bool (0 or 1)
    // singleState.setValue((byte)value.toInt()); // byte
    // singleState.setValue(value.toInt()); // int
    // singleState.setValue(value.toFloat()); // float (with .0)
    // singleState.setValue(value.length() > 0 ? value.charAt(0) : ' '); // char
    // singleState.setValue(value); // String
  } else {
    // Unknown tag
    Serial.println("[" + tag + "(unknown)]: " + value);
  }
}

void observeState() {
  // Check and apply state
  if (singleState.check()) {
    bool newValue = singleState.getValue(); // bool
    // byte newValue = singleState.getValue(); // byte
    // int newValue = singleState.getValue(); // int
    // float newValue = singleState.getValue(); // float
    // char newValue = singleState.getValue(); // char
    // String newValue = singleState.getValue(); // String

    Serial.println("[" + singleState.getTag() + "]: " + (String)newValue);
  }
}
