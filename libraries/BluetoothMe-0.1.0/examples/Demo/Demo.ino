#include <SoftwareSerial.h>
#include <SoftwareSerialAdapter.h>
// #include <HardwareSerialAdapter.h>
// #include <AsyncSoftwareSerialAdapter.h>
// #include <AsyncHardwareSerialAdapter.h>
#include <BluetoothMe.h>

#define BLUETOOTH_RX 2 // Connect to TX on Bluetooth module
#define BLUETOOTH_TX 3 // Connect to RX on Bluetooth module
SoftwareSerial bluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

SoftwareSerialAdapter bluetoothAdapter(&bluetoothSerial, 9600, 100); // port (or RX, TX), speed, timeout
// HardwareSerialAdapter bluetoothAdapter(&Serial, 9600, 100);
// AsyncSoftwareSerialAdapter<100> bluetoothAdapter(&bluetoothSerial, 9600, 100);
// AsyncHardwareSerialAdapter<100> bluetoothAdapter(&Serial, 9600, 100);

BluetoothMe bluetoothMe(&bluetoothAdapter);

void onReceive(String tag, String value);

void setup() {
  Serial.begin(9600);
}

void loop() {
  bluetoothMe.listen(onReceive);
}

void onReceive(String tag, String value) {
  Serial.print("[tag]: ");
  Serial.print(tag);
  Serial.print("\t[value]: ");
  Serial.println(value);
  
  // TODO
}
