void applyMovementDirectionByState(bool forward, bool backward, bool turnLeft, bool turnRight, byte speed) {
  int leftMotorDirection = forward - backward - (turnLeft - turnRight) + 2 * backward * (turnLeft - turnRight);
  int rightMotorDirection = forward - backward - (turnRight - turnLeft) + 2 * backward * (turnRight - turnLeft);

  applyMotorSpeed(LEFT_MOTOR_IN3, LEFT_MOTOR_IN4, leftMotorDirection, speed, LEFT_MOTOR_MIN_DUTY);
  applyMotorSpeed(RIGHT_MOTOR_IN2, RIGHT_MOTOR_IN1, rightMotorDirection, speed, RIGHT_MOTOR_MIN_DUTY);
}

void applyMotorSpeed(byte pin, byte pinPwm, int direction, byte speed, byte offset) {
  digitalWrite(pin, direction < 0);

  if (direction > 0) analogWrite(pinPwm, map(speed, 0, 255, offset, 255));
  else if (direction < 0) analogWrite(pinPwm, map(speed, 0, 255, 255 - offset, 0));
  else digitalWrite(pinPwm, LOW);
}

void setupMotors() {
  pinMode(LEFT_MOTOR_IN3, OUTPUT);
  pinMode(LEFT_MOTOR_IN4, OUTPUT);
  pinMode(RIGHT_MOTOR_IN2, OUTPUT);
  pinMode(RIGHT_MOTOR_IN1, OUTPUT);

  digitalWrite(LEFT_MOTOR_IN3, LOW);
  digitalWrite(LEFT_MOTOR_IN4, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
}
