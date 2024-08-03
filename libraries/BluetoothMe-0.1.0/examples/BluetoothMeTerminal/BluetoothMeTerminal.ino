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

void setup() {
  // In the Serial Monitor, select the correct baud rate and the `Newline` option
  Serial.begin(9600);
  Serial.setTimeout(1000);
}

void loop() {
  String inputMessage = bluetoothMe.listen();
  if (!inputMessage.equals("")) {
    writeSerialMonitor(inputMessage, false);
  }

  listenSerialMonitor();
}

void listenSerialMonitor() {
  if (Serial.available()) {
    String outputMessage = Serial.readStringUntil('\n');
    writeSerialMonitor(outputMessage, true);
    bluetoothMe.send(outputMessage);
    // bluetoothMe.exec(outputMessage + "\r\n"); // May require CR+LF characters at the end of the AT-command depending on the model
  }
}

void writeSerialMonitor(String message, bool output) {
  String commandStr = output ? "[<-]: " : "[->]: ";
  Serial.println(commandStr + message);
}
