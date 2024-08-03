#include <AUnit.h> // https://www.arduino.cc/reference/en/libraries/aunit/
#include <IBluetoothAdapter.h>
#include <BluetoothMe.h>

class TestBluetoothAdapter : public IBluetoothAdapter {
  public:
    TestBluetoothAdapter(String readMessage = "") {
      _readMessage = readMessage;
    }

    String readUntil(char terminator) {
      return _readMessage;
    }

    void write(String message) {
      _writtenMessage = message;
    }

    String getWrittenMessage() {
      return _writtenMessage;
    }

  private:
    String _readMessage;
    String _writtenMessage;
};

String messageTag, messageValue;

void onReceive(String tag, String value) {
   messageTag = tag;
   messageValue = value;
};


test(sendMessage_OutputMessageWithoutTag_InvokeWriteWithCorrectArg) {
  String message = "value1";
  String expectedMessage = message + BluetoothMe::MESSAGE_TERMINATOR;
  TestBluetoothAdapter adapter = TestBluetoothAdapter();
  BluetoothMe bluetoothMe(&adapter);

  bluetoothMe.send(message);

  assertEqual(adapter.getWrittenMessage(), expectedMessage);
}

test(sendMessage_OutputMessageWithTag_InvokeWriteWithCorrectArg) {
  String messageTag = "tag2";
  String messageValue = "value2";
  String expectedMessage = messageTag + BluetoothMe::TAG_TERMINATOR
                           + messageValue + BluetoothMe::MESSAGE_TERMINATOR;
  TestBluetoothAdapter adapter = TestBluetoothAdapter();
  BluetoothMe bluetoothMe(&adapter);

  bluetoothMe.send(messageTag, messageValue);

  assertEqual(adapter.getWrittenMessage(), expectedMessage);
}

test(listenAdapter_InputMessageWithoutTag_InvokeOnReceiveWithCorrectArgs) {
  String expectedMessageTag = "";
  String expectedMessageValue = "value3";
  String message = expectedMessageValue;
  TestBluetoothAdapter adapter(message);
  BluetoothMe bluetoothMe(&adapter);

  bluetoothMe.listen(onReceive);

  assertEqual(messageTag, expectedMessageTag);
  assertEqual(messageValue, expectedMessageValue);
}

test(listenAdapter_InputMessageWithEmptyTag_InvokeOnReceiveWithCorrectArgs) {
  String expectedMessageTag = "";
  String expectedMessageValue = "value4";
  String message = expectedMessageTag + BluetoothMe::TAG_TERMINATOR + expectedMessageValue;
  TestBluetoothAdapter adapter(message);
  BluetoothMe bluetoothMe(&adapter);

  bluetoothMe.listen(onReceive);

  assertEqual(messageTag, expectedMessageTag);
  assertEqual(messageValue, expectedMessageValue);
}

test(listenAdapter_InputMessageWithTag_InvokeOnReceiveWithCorrectArgs) {
  String expectedMessageTag = "tag5";
  String expectedMessageValue = "value5";
  String message = expectedMessageTag + BluetoothMe::TAG_TERMINATOR + expectedMessageValue;
  TestBluetoothAdapter adapter(message);
  BluetoothMe bluetoothMe(&adapter);

  bluetoothMe.listen(onReceive);

  assertEqual(messageTag, expectedMessageTag);
  assertEqual(messageValue, expectedMessageValue);
}

test(listenAdapter_InputMessageWithMultipleTags_InvokeOnReceiveWithCorrectArgs) {
  String expectedMessageTag = "tag6";
  String expectedMessageValue = expectedMessageTag + BluetoothMe::TAG_TERMINATOR + "value6";
  String message = expectedMessageTag + BluetoothMe::TAG_TERMINATOR + expectedMessageValue;
  TestBluetoothAdapter adapter(message);
  BluetoothMe bluetoothMe(&adapter);

  bluetoothMe.listen(onReceive);

  assertEqual(messageTag, expectedMessageTag);
  assertEqual(messageValue, expectedMessageValue);
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  aunit::TestRunner::run();
}
