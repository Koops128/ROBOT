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
void performRandomLeft();
void sharpRight();
void sharpLeft();
task detectSensor();
task moveRandomly();

/*
 * This thread detects any input from the touch sensor.
 * It will start and stop the moveRandomly task according to touch sensor input
 */
task detectSensor() {
		int i = 0;
		bool back = false;
		while(true) {
				if (SensorValue[touchSensorR]) {
						while(i++ < 200) { //check for 300 ms (bar still isn't very sensitive)
							if(SensorValue[touchSensorL]) {
								back = true;
								i = 200;
								break;
							}
							wait1Msec(1);
						}
	  				stopTask(moveRandomly);
						stopAllMotors();
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
						while (i++ < 200) {
							if (SensorValue[touchSensorR]){
								back = true;
								i = 200;
								break;
							}
							wait1Msec(1);
						}
						stopTask(moveRandomly);
						stopAllMotors();
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

/*
 * This thread non-deterministically selects a movement for the robot to perform
 */
task moveRandomly {
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
	sleep(2000);
	stopAllMotors();
	sleep(2000);
	if (random(1)) { 
		sharpRight();
	} else {
		sharpLeft();
	}
	sleep(random(1500) + 1000);
}

/*
 * travelRight is a medium wide right curve.
 * After performing the right curve a random left curve is performed
 */
void travelRight() {
	displayCenteredBigTextLine(4, "Right");
	setLEDColor(ledGreen);
	setMotor(motorB, 50);
	setMotor(motorC, 60);
	sleep(random(3000) + 500);
	performRandomLeft();
}

/*
 * travelLeft is a left curve.
 */
void travelLeft() {
	displayCenteredBigTextLine(4, "Left");
	setLEDColor(ledRed);
	setMotor(motorB, 60);
	setMotor(motorC, 50);
	sleep(random(3000) + 500);
}

/*
 * travelRightWide is a wide right curve.
 * After performing the right curve a random left curve is performed
 */
void travelRightWide() {
	displayCenteredBigTextLine(4, "Right");
	setLEDColor(ledRed);
	setMotor(motorB, 50);
	setMotor(motorC, 55);
	sleep(random(3000) + 500);
	performRandomLeft();
}

/*
 * travelLeftWide is a wide left curve.
 */
void travelLeftWide() {
	displayCenteredBigTextLine(4, "Left");
	setLEDColor(ledRed);
	setMotor(motorB, 55);
	setMotor(motorC, 50);
	sleep(random(3000) + 500);
}

/*
 * travelRightNarrow is a narrow right curve.
 * After performing the right curve a random left curve is performed
 */
void travelRightNarrow() {
	displayCenteredBigTextLine(4, "Right");
	setLEDColor(ledRed);
	setMotor(motorB, 50);
	setMotor(motorC, 70);
	sleep(random(3000) + 500);
	performRandomLeft();
}

/*
 * travelLeftNarrow is a narrow left curve.
 */
void travelLeftNarrow() {
	displayCenteredBigTextLine(4, "Left");
	setLEDColor(ledRed);
	setMotor(motorB, 70);
	setMotor(motorC, 50);
	sleep(random(3000) + 500);
}

/*
 * sharpLeft is a simple left turn.
 */
void sharpLeft() {
	displayCenteredBigTextLine(4, "Left");
	setLEDColor(ledRed);
	setMotor(motorB, 50);
	setMotor(motorC, 0);
	sleep(400);
}

/*
 * sharpRight is a simple right turn.
 */
void sharpRight() {
	displayCenteredBigTextLine(4, "Right");
	setLEDColor(ledGreen);
	setMotor(motorB, 0);
	setMotor(motorC, 50);
	sleep(400);
}

/*
 * performRandomLeft randomly selects one of the three left curves.
 */
void performRandomLeft() {
	int randomMove = random(2);
	if (randomMove == 0) {
		travelLeft();
	} else if (randomMove == 1) {
		travelLeftWide();
	} else {
		travelLeftNarrow();
	}
}
