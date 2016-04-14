#pragma config(Sensor, S4,     touchSensorL,    sensorEV3_Touch)
#pragma config(Sensor, S1,     touchSensorR,    sensorEV3_Touch)
#pragma config(Motor,  motorB,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)


void turnRight();
void turnLeft();
void backUp();
void travelLeft();
void travelRight();
void travelRightWide();
void travelLeftWide();
void travelRightNarrow();
void travelLeftNarrow();
void moveRandomLeft();
void sharpRight();
void sharpLeft();
task detectSensor();
task moveRandomly();
void moveRandomRight();

/*
 * This thread detects any input from the touch sensor.
 * It will start and stop the moveRandomly task according to touch sensor input
 */
task detectSensor() {
		int i = 0;
		bool back = false;
		while(true) {

				if (SensorValue[touchSensorR]) {
					setLEDColor(ledRed);
					stopTask(moveRandomly);
					stopAllMotors();
						while(i++ < 50) { //check for 50 ms (bar still isn't very sensitive)
							if(SensorValue[touchSensorL]) {
								back = true;
								i = 50;
								break;
							}
							wait1Msec(1);
						}
						setLEDColor(ledOff);
						if (back) {
							backUp();
						} else {
							turnLeft();
							travelLeft();
						}
						back = false;
						i = 0;
						startTask(moveRandomly);
				}
				if (SensorValue[touchSensorL]) {
					setLEDColor(ledRed);
					stopTask(moveRandomly);
					stopAllMotors();
						while (i++ < 50) {
							if (SensorValue[touchSensorR]){
								back = true;
								i = 50;
								break;
							}
							wait1Msec(1);
						}
						setLEDColor(ledOff);
						if (back) {
							backUp();
						} else {
							turnRight();
							travelRight();
						}
						back = false;
						i = 0;
						startTask(moveRandomly);
				}
				sleep(15);
		}
}

task moveRandomly() {
	int i = random(1);
	while (true) {
		if (i++ % 2 == 0) {
			moveRandomRight();
		}
		else {
			moveRandomLeft();
		}
	}
}

/*
 * This thread non-deterministically selects a movement for the robot to perform
 */
void moveRandomRight() {
	int randomMove = random(2);
	while (true) {
		if (randomMove == 0) {
			travelRight();
	  } else if (randomMove == 1) {
			travelRightWide();
	  } else {
	  	travelRightNarrow();
		}
	}
}

/*
 * -+ randomly selects one of the three left curves.
 */
void moveRandomLeft() {
	int randomMove = random(2);
	if (randomMove == 0) {
		travelLeft();
	} else if (randomMove == 1) {
		travelLeftWide();
	} else {
		travelLeftNarrow();
	}
}

/*
 * This thread is the main thread that starts all other threads
 */
task main() {
	startTask(moveRandomly);
	startTask(detectSensor);
	while(true) {
		wait(300);
	}
}

/*
 * travelRight is a medium wide right curve.
 * After performing the right curve a random left curve is performed
 */
void travelRight() {
	displayCenteredBigTextLine(4, "Right");
	int i;
	for (i = 0; i < random(60) + 20; i++) {
		if (SensorValue[touchSensorR] || SensorValue[touchSensorL]) {
			break;
		}
		setMotor(motorB, 50);
		setMotor(motorC, 60);
		sleep(50);
	}
}

/*
 * travelLeft is a left curve.
 */
void travelLeft() {
	displayCenteredBigTextLine(4, "Left");
	int i;
	for (i = 0; i < random(60) + 20; i++) {
		if (SensorValue[touchSensorR] || SensorValue[touchSensorL]) {
			break;
		}
		setMotor(motorB, 60);
		setMotor(motorC, 50);
		sleep(50);
	}
}

/*
 * travelRightWide is a wide right curve.
 * After performing the right curve a random left curve is performed
 */
void travelRightWide() {
	displayCenteredBigTextLine(4, "Right");
	int i;
	for (i = 0; i < random(60) + 20; i++) {
		if (SensorValue[touchSensorR] || SensorValue[touchSensorL]) {
			break;
		}
		setMotor(motorB, 50);
		setMotor(motorC, 55);
		sleep(50);
	}
}

/*
 * travelLeftWide is a wide left curve.
 */
void travelLeftWide() {
	displayCenteredBigTextLine(4, "Left");
	int i;
	for (i = 0; i < random(60) + 20; i++) {
		if (SensorValue[touchSensorR] || SensorValue[touchSensorL]) {
			break;
		}
		setMotor(motorB, 55);
		setMotor(motorC, 50);
		sleep(50);
	}
}

/*
 * travelRightNarrow is a narrow right curve.
 * After performing the right curve a random left curve is performed
 */
void travelRightNarrow() {
	displayCenteredBigTextLine(4, "Right");
	int i;
	for (i = 0; i < random(60) + 20; i++) {
		if (SensorValue[touchSensorR] || SensorValue[touchSensorL]) {
			break;
		}
		setMotor(motorB, 50);
		setMotor(motorC, 70);
		sleep(50);
	}
}

/*
 * travelLeftNarrow is a narrow left curve.
 */
void travelLeftNarrow() {
	displayCenteredBigTextLine(4, "Left");
	int i;
	for (i = 0; i < random(60) + 20; i++) {
		if (SensorValue[touchSensorR] || SensorValue[touchSensorL]) {
			break;
		}
		setMotor(motorB, 70);
		setMotor(motorC, 50);
		sleep(50);
	}
}

/*
 * sharpLeft is a simple left turn.
 */
void sharpLeft() {
	displayCenteredBigTextLine(4, "Left");
	setMotor(motorB, 50);
	setMotor(motorC, 0);
	sleep(400);
}

/*
 * sharpRight is a simple right turn.
 */
void sharpRight() {
	displayCenteredBigTextLine(4, "Right");
	setMotor(motorB, 0);
	setMotor(motorC, 50);
	sleep(400);
}

/************************
*****BACK UP METHODS*****
************************/

/*
 * turnRight occurs when the left touch sensor is triggered.
 * The robot will back up for 2 seconds, then turns right
 */
void turnRight() {
	displayCenteredBigTextLine(4, "Back");
	setLEDColor(ledOff);
	setMotor(motorC, -50);
	setMotor(motorB, -50);
	sleep(2000);
	sharpRight();
}

/*
 * turnLeft occurs when the left touch sensor is triggered.
 * The robot will back up for 2 seconds, then turns left
 */
void turnLeft() {
	displayCenteredBigTextLine(4, "Back");
	setLEDColor(ledOff);
	setMotor(motorB, -50);
	setMotor(motorC, -50);
	sleep(2000);
	sharpLeft();
}

/*
 * backUp plays a tone then reverses for 2 seconds and then pauses for another
 * 2 seconds. After this it
 */
void backUp() {
	playTone(400, 20);
	displayCenteredBigTextLine(4, "Back");
	setLEDColor(ledOff);
	setMotor(motorB, -50);
	setMotor(motorC, -50);
	sleep(1500);
	stopAllMotors();
	sleep(2000);
	if (random(1)) {
		sharpRight();
	} else {
		sharpLeft();
	}
	sleep(random(1500) + 1000);
}
