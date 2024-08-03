void tryCheckBatteryLevelOnTimer() {
  if (batteryLevelCheckTimer.tick()) {
    checkAndUpdateBatteryLevel();
  }
}

void tryShowBatteryLevelOnTimer() {
  // Tick the timer to show the battery level
  if (batteryLevelShowTimer.tick()) {
    batteryLevelShowTimer.stop();
    showBatteryLevel();
  }

  // Tick the LED to blink and restart the timer if it is inactive
  if (batteryLevelLed.tick() != LED_STATE_BLINK && !batteryLevelShowTimer.active()) {
    batteryLevelShowTimer.restart();
  }
}

void showBatteryLevel() {
  batteryLevelLed.startBlinking(
    BLINK_ON_PERIOD_MS,
    BLINK_OFF_PERIOD_MS,
    convertPercentToPoint(state.batteryLevel.getValue())
  );
}

void sendBatteryLevel() {
  bluetoothMe.send(state.batteryLevel.getTag(), (String)state.batteryLevel.getValue() + "%");
}

void checkAndUpdateBatteryLevel() {
  int vcc = readVcc(VCC_CONST);
  byte percent = convertVccToPercent(vcc);
  state.batteryLevel.setValue(percent);
}

byte convertPercentToPoint(byte percent) {
  return map(percent, 100, 0, 5, 1);
}
