float positionKp = .4,//.36, //proportional constant for positional PID
			straighteningKpLeft = 0.545,//0.2,//.43,//.195, //proportional constant for straightening response for the left side
			straighteningKpRight = 0.545,//0.2,//.22,//.16, //proportional constant for straightening response for the right side
			straighteningKpLeftTurn = 0.4,//.4,//.43,//.195, //proportional constant for straightening response for the left side when turning
			straighteningKpRightTurn = 0.4,//.4,//.22,//.16, //proportional constant for straightening response for the right side when turning
			positionKi = 0.000150,//0.000350, //integral constant
			positionKd = 2.9;//4; //derivative constant

static int STRAIGHT = 2; //the 2 here shouldn't matter as long as no variables are multiplied by 'direction' in driveDistancePID
static int ROTATE_LEFT = -1;
static int ROTATE_RIGHT = 1;


void driveDistancePID(int encoderCounts, int direction, int time) {
	writeDebugStreamLine("nPgmTime,error,nMotorEncoder[lDriveFront], nMotorEncoder[rDriveMiddle],pTerm,iTerm,dTerm,lPower,rPower");
	//reset encoder values
	nMotorEncoder[rDriveFront] = 0;
	nMotorEncoder[lDriveFront] = 0;

	int error = 0,
	straighteningError = 0,
	errorSum = 0,
	lastError = 0,
	target = encoderCounts,
	slewRateLimit = 15;

	float pTerm,
	iTerm,
	dTerm,
	straighteningCorrection,
	power,
	lPower,
	rPower,
	lastPower;
	time1[T1] = 0;
	if (direction == STRAIGHT) {
		while (time1[T1] < time) {
			//update error terms
			error = target - (nMotorEncoder[lDriveFront] + nMotorEncoder[rDriveFront])/2.0;
			errorSum += error;

			pTerm = error * (float) positionKp;
			iTerm = errorSum * (float) positionKi;
			dTerm = (error - lastError) * (float) positionKd; //calculate motor power
			power = pTerm + iTerm + dTerm;

			//limit the values of the power term to only be those that can be taken by the motors
			if (power > 127) {
				power = 127;
			} else if (power < -127) {
				power = -127;
			}

			lastError = error; //update last error

			//apply a slew rate to limit acceleration/deceleration
			if(abs(power-lastPower) > slewRateLimit) {
				if(power > lastPower) { //if the power is increasing (and the difference is greater than the slew rate allows)
					power = lastPower + slewRateLimit; //increment the power to only add
				} else { //if the power is decreasing (and the differene is greater than the slew rate allows)
					power = lastPower - slewRateLimit;
				}
			}

			//adjust the powers sent to each side if the encoder values don't match
			straighteningError = (abs(power) > 25) ? nMotorEncoder[lDriveFront] - nMotorEncoder[rDriveFront] : 0; //only straighten at higher powers so that robot doesn't turn because of straightening "correction" at low speeds

			/*if (straighteningError > 0) { //left side is ahead, so speed up the right side
				rPower = power + straighteningError*straighteningKpLeft;
			} else { //otherwise, just set the right side to the power
				rPower = power;
			}
			if (straighteningError < 0) { //right side is ahead, so speed up the left side
				lPower = power - straighteningError*straighteningKpRight;
			} else { //otherwise, just set the right side to the power
				lPower = power;
			}*/

			if (straighteningError < 0) { //left side is behind, so slow down the right side
				rPower = power + straighteningError*straighteningKpLeft;
			} else { //otherwise, just set the right side to the power
				rPower = power;
			}
			if (straighteningError > 0) { //right side is behind, so slow down the left side
				lPower = power - straighteningError*straighteningKpRight;
			} else { //otherwise, just set the right side to the power
				lPower = power;
			}

			lastPower = power; //update the last power
			writeDebugStreamLine("%d,%f,%f,%f,%f,%f,%f,%f,%f",nPgmTime,error,nMotorEncoder[lDriveFront], nMotorEncoder[rDriveFront],pTerm,iTerm,dTerm,lPower,rPower);
			setLDriveMotors(lPower);
			setRDriveMotors(rPower);
			wait1Msec(25);
		}
	} else if (direction == ROTATE_LEFT || direction == ROTATE_RIGHT) {
		while (time1[T1] < time) {
			//update error terms
			error = target - (abs(nMotorEncoder[lDriveFront]) + abs(nMotorEncoder[rDriveFront]))/2; //need to use absolute values here because one of
			errorSum += error;

			pTerm = error * (float) positionKp;
			iTerm = errorSum * (float) positionKi;
			dTerm = (error - lastError) * (float) positionKd; //calculate motor power
			power = pTerm + iTerm + dTerm;

			//limit the values of the power term to only be those that can be taken by the motors
			if (power > 127) {
				power = 127;
			} else if (power < -127) {
				power = -127;
			}

			lastError = error; //update last error

			//apply a slew rate to limit acceleration/deceleration
			if(abs(power-lastPower) > slewRateLimit) {
				if(power > lastPower) { //if the power is increasing (and the difference is greater than the slew rate allows)
					power = lastPower + slewRateLimit; //increment the power to only add
				} else { //if the power is decreasing (and the differene is greater than the slew rate allows)
					power = lastPower - slewRateLimit;
				}
			}

			//adjust the powers sent to each side if the encoder values don't match
			straighteningError = abs(nMotorEncoder[lDriveFront]) - abs(nMotorEncoder[rDriveFront]);

			if (straighteningError > 0) { //left side is ahead, so speed up the right side
				rPower = power + straighteningError*straighteningKpLeftTurn;
			} else { //otherwise, just set the right side to the power
				rPower = power;
			}
			if (straighteningError < 0) { //right side is ahead, so speed up the left side
				lPower = power - straighteningError*straighteningKpRightTurn;
			} else { //otherwise, just set the right side to the power
				lPower = power;
			}

			lastPower = power; //update the last power
			writeDebugStreamLine("%d,%f,%f,%f,%f,%f,%f,%f,%f",nPgmTime,error,nMotorEncoder[lDriveFront], nMotorEncoder[rDriveFront],pTerm,iTerm,dTerm,lPower,rPower);
			setLDriveMotors(lPower * direction); //for a left turn, ROTATE_LEFT = -1 so this moves the left side backwards for a left turn. For a right turn will go forward since ROTATE_RIGHT = 1
			setRDriveMotors(rPower * -1 * direction); //same idea as for a left turn, except this side needs to go the opposite way as the left side in order to turn, hence the * -1 in the calculation
			wait1Msec(25);
	}
}
}


void intakeDistance (int encoderCounts, int direction, float power) {
	int encoderGoal = nMotorEncoder[intakeChain] - encoderCounts*direction; //intake encoder counts down for forward
	if (direction == 1) {
		while (nMotorEncoder[intakeChain] > encoderGoal) {
			setIntakeMotors(power*direction);
		}
	} else {
		while (nMotorEncoder[intakeChain] < encoderGoal) {
			setIntakeMotors(power*direction);
		}
	}

	setIntakeMotors(0);
}

/*void longShotAuton(bool waitAtStart) {
	if(waitAtStart) {
		wait1Msec(3000);
	}
	initializePIDLong();
	FwVelocitySet(lFly,132,.7);
	FwVelocitySet(rFly,132,.7);
	wait1Msec(1700);
	intakeDistance(150,1,125);
	wait1Msec(750);
	intakeDistance(150,1,125);
	wait1Msec(750);
	intakeDistance(300,1,125);
	wait1Msec(750);
	intakeDistance(300,1,125);
	wait1Msec(1000);
	stopFlywheel();
}*/

void closeShotAuton(bool waitAtStart) {
	if(waitAtStart) {
		wait1Msec(3000);
	}
	initializePIDShort();
	FwVelocitySet(lFly, 83, .5);
	FwVelocitySet(rFly, 83, .5);
	driveDistance(3300, 1, 122);
	wait1Msec(500);

	setIntakeMotors(122);
	wait1Msec(525);
	setIntakeMotors(0);
	wait1Msec(750);

	setIntakeMotors(122);
	wait1Msec(525);
	setIntakeMotors(0);
	wait1Msec(750);

	setIntakeMotors(122);
	wait1Msec(525);
	setIntakeMotors(0);
	wait1Msec(750);

	setIntakeMotors(122);
	wait1Msec(525);
	setIntakeMotors(0);
	wait1Msec(750);

	wait1Msec(1000);
	stopFlywheel();
	//wait1Msec(500);
	//setIntakeMotors(110); //previously 122
}

void longShotAuton(bool waitAtStart) {
	if(waitAtStart) {
		wait1Msec(5000);
	}
	initializePIDPurple();
	FwVelocitySet(lFly, 120, .5);
	FwVelocitySet(rFly, 120, .5);
	driveDistance(1775, 1, 125);
	wait1Msec(500);
	setIntakeMotors(125);
}

void programmingSkills() {
	initializePIDPurple();
	setIntakeMotors(127);
	FwVelocitySet(lFly,115,.7);
	FwVelocitySet(rFly,115,.7);
	wait1Msec(25000);
	rotate(850,-1);
	motor[intakeChain] = 127;
	motor[intakeRoller] = 0;
	wait1Msec(750);
	driveDistance(3275, 1, 60);
	wait1Msec(750);
	rotate(780,1);
	motor[intakeRoller] = 127;
	wait1Msec(30000);
}


task autonomous()
{
	//testing
	//pgmToRun = "Prog. Skills";
	//delayStart = false;
	if (pgmToRun == "R Side Long" || pgmToRun == "R Back Long"
			|| pgmToRun == "B Side Long"
			|| pgmToRun == "B Back Long") {
			longShotAuton(delayStart);
	} else if (pgmToRun == "B Side Close" || pgmToRun == "B Back Close"
			|| pgmToRun == "R Side Close"
			|| pgmToRun == "R Back Close") {
			closeShotAuton(delayStart);
	} else if (pgmToRun == "Prog. skills") {
			programmingSkills();
	}
}



task testautonomous()
{
	//wait1Msec(5000);

//	bool buttonPressed = false;
//	while(1) {
//	if(vexRT[Btn8D] == 1) {
//		if (!buttonPressed) {
//			buttonPressed = true;
//			driveDistancePID(0, FORWARD, 4000);
//			buttonPressed = false;
//		}
//	}
//}

	//configure gyro.  normally done in pre_auton function
	//SensorType[gyro] = sensorNone;
 // wait1Msec(500);
 // //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
 // SensorType[gyro] = sensorGyro;
 // wait1Msec(2000);

	//rotate left, negative target
	//rotate right, positive target
  //driveDistancePID(-300, ROTATE_LEFT, 1500);

	//starting from red back tile
	//motor[intake] = 127;
	//driveDistancePID(450, STRAIGHT, 1250); //drive forward from the tile
	//setLDriveMotors(0);
	//setRDriveMotors(0);
	//wait1Msec(250); //wait a bit to let things settle out before turning
	//rotateDegrees(500,1); //turn left to face the blue side wall
	//wait1Msec(250);
	//driveDistancePID(725, FORWARD, 1750);
	//wait1Msec(250);
	//rotateDegrees(900,-1);

	//driveDistancePID(-300, FORWARD,1000);
	//driveDistancePID(200, FORWARD, 1000);
	////rotateDegrees(75, -1);
	//driveDistancePID(-300, FORWARD,1000);
	//setLDriveMotors(0);
	//setRDriveMotors(0);
	//wait1Msec(1500);

  //starting from red side tile
	//wait1Msec(7000);

	//HERE.  LOOK HERE - this is the testing code, commented out so nothing runs accidentally
 // driveDistancePID(1000, STRAIGHT, 5000); //move forward from tile
 // setLDriveMotors(0);
	//setRDriveMotors(0);


  //driveDistancePID(285,ROTATE_RIGHT, 750); //turn towards blue net
  //driveDistancePID(815,STRAIGHT,1250); //move forward towards blue net
  //driveDistancePID(460,ROTATE_LEFT, 1250); //turn so intake faces ball stack on side wall
  //motor[intakeChain] = 127; //start the intake to prepare to pick up balls
  //driveDistancePID(-445,STRAIGHT,1000); //first pass at ball stack
  ////driveDistancePID doesn't stop motors when it's done.  Stop the motors here while we wait for a little to pick up balls on the first pass
  //setLDriveMotors(0);
  //setRDriveMotors(0);
  //wait1Msec(750);
  //driveDistancePID(200,STRAIGHT,750);
  //driveDistancePID(-300,STRAIGHT,750);
  //wait1Msec(750);
  //motor[intake] = 0;
  //startTask(moveIntakeBack);
}
