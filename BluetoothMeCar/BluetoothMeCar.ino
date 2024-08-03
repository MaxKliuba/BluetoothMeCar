#include <SoftwareSerialAdapter.h>
#include <BluetoothMe.h>
#include <BluetoothMeState.h>
#include <TimerMs.h>
#include <SimpleLED.h>
#include <ReadVccUtils.h>


#define BLUETOOTH_RX 2 // Connect to TX on Bluetooth module
#define BLUETOOTH_TX 3 // Connect to RX on Bluetooth module

// RX, TX, speed, timeout
SoftwareSerialAdapter bluetoothAdapter(BLUETOOTH_RX, BLUETOOTH_TX, 9600, 100);
BluetoothMe bluetoothMe(&bluetoothAdapter);

#define STOP_TAG "s"

struct {
  BluetoothMeState<byte> moveForward = BluetoothMeState<byte>("f", 0);
  BluetoothMeState<byte> moveBackward = BluetoothMeState<byte>("b", 0);
  BluetoothMeState<byte> turnLeft = BluetoothMeState<byte>("l", 0);
  BluetoothMeState<byte> turnRight = BluetoothMeState<byte>("r", 0);
  BluetoothMeState<byte> speed = BluetoothMeState<byte>("spd", 200);
  BluetoothMeState<byte> led = BluetoothMeState<byte>("led", 0);
  BluetoothMeState<byte> batteryLevel = BluetoothMeState<byte>("bat", 0);
} state;

void onReceive(String tag, String value);


#define LEFT_MOTOR_IN4 6 // PWM
#define LEFT_MOTOR_IN3 7
#define RIGHT_MOTOR_IN2 8
#define RIGHT_MOTOR_IN1 9 // PWM

#define LEFT_MOTOR_MIN_DUTY 105
#define RIGHT_MOTOR_MIN_DUTY 85


#define BLINK_ON_PERIOD_MS 300
#define BLINK_OFF_PERIOD_MS 200

#define BATTERY_LEVEL_LED_PIN 13
#define LED_PIN 4

SimpleLED batteryLevelLed(BATTERY_LEVEL_LED_PIN);
SimpleLED led(LED_PIN);


#define BATTERY_LEVEL_CHECK_TIMER_PERIOD_MS 10000
#define BATTERY_LEVEL_SHOW_TIMER_PERIOD_MS 1500

// period (ms), state (true/false - on/off), mode (true/false - period/timer)
TimerMs batteryLevelCheckTimer(BATTERY_LEVEL_CHECK_TIMER_PERIOD_MS, true, false);
TimerMs batteryLevelShowTimer(BATTERY_LEVEL_SHOW_TIMER_PERIOD_MS, true, false);


#define VCC_CONST 1.1f


void setup() {
  setupMotors();

  batteryLevelCheckTimer.force();
}

void loop() {
  bluetoothMe.listen(onReceive);

  tryCheckBatteryLevelOnTimer();
  tryShowBatteryLevelOnTimer();

  observeState();
}

void onReceive(String tag, String value) {
  if (tag.equals(BluetoothMeState<byte>::GET_STATE_TAG)) {
    // Send state
    for (BluetoothMeState<byte>* statePtr = &state.moveForward; statePtr <= &state.led; ++statePtr) {
      bluetoothMe.send(statePtr->getTag(), (String)statePtr->getValue());
    }
    sendBatteryLevel();

    return;
  }

  // Set value for state from `moveForward` to `led` (inclusive)
  for (BluetoothMeState<byte>* statePtr = &state.moveForward; statePtr <= &state.led; ++statePtr) {
    if (tag.equals(statePtr->getTag())) {
      statePtr->setValue((byte)value.toInt());

      return;
    }
  }

  if (tag.equals(STOP_TAG) && (bool)value.toInt()) {
    // Set 0 for state from `moveForward` to `turnRight` (inclusive)
    for (BluetoothMeState<byte>* statePtr = &state.moveForward; statePtr <= &state.turnRight; ++statePtr) {
      statePtr->setValue(0);
    }

    return;
  }

  if (tag.equals(state.batteryLevel.getTag())) {
    sendBatteryLevel();

    return;
  }
}

void observeState() {
  // Check and apply `led` state
  if (state.led.check()) {
    led.set(state.led.getValue());
  }

  // Check and apply state from `moveForward` to `turnRight` (inclusive)
  if (state.moveForward.check() | state.moveBackward.check() | state.turnLeft.check() | state.turnRight.check() | state.speed.check()) {
    applyMovementDirectionByState(
      state.moveForward.getValue(),
      state.moveBackward.getValue(),
      state.turnLeft.getValue(),
      state.turnRight.getValue(),
      state.speed.getValue()
    );
  }

  // Check and send battery level
  if (state.batteryLevel.check()) {
    sendBatteryLevel();
  }
}
