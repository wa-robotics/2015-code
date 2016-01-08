#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    powerExpanderStatus, sensorAnalog)
#pragma config(Sensor, dgtl11, redLED,         sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, led,            sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           intakeTop,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           flyWheelL1,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           flyWheelL2,    tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port4,           lFrontDrive,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           lBackDrive,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rFrontDrive,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rBackDrive,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           flyWheelR1,    tmotorVex393_MC29, openLoop, encoderPort, None)
#pragma config(Motor,  port9,           flyWheelR2,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port10,          intakeBack,    tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "..\..\Vex_Competition_Includes_No_LCD.c"   //Main competition background code...do not modify!
#include "..\..\Battery Check (header file).h"
#include "..\..\LCD Autonomous Play Selection.c"
#include "..\..\Flywheel Robots\Paper Flywheel\TBH Controller.h" //Contains the TBH algorithm for the flywheels

fw_controller lFly, rFly; //Create a controller for the TBH algorithm for each side of the flywheel
string str;

void setLFly(int output)
{
	motor[flyWheelL1] = output;
	motor[flyWheelL2] = output;
}

void setRFly(int output)
{
	motor[flyWheelR1] = output;
	motor[flyWheelR2] = output;
}


/*-----------------------------------------------------------------------------*/
/** @brief     Task to control the velocity of the left flywheel               */
/*-----------------------------------------------------------------------------*/
task leftFwControlTask()
{
	fw_controller *fw = lFly;

	// We are using Speed geared motors
	// Set the encoder ticks per revolution
	fw->ticks_per_rev = fw->MOTOR_TPR;

	while(1)
	{
		// debug counter
		fw->counter++;

		// Calculate velocity
		FwCalculateSpeed(fw, nMotorEncoder[flyWheelL2]);

		// Set current speed for the tbh calculation code
		fw->current = fw->v_current;

		// Do the velocity TBH calculations
		FwControlUpdateVelocityTbh( fw ) ;

		// Scale drive into the range the motors need
		fw->motor_drive  = (fw->drive * FW_MAX_POWER) + 0.5;

		// Final Limit of motor values - don't really need this
		if( fw->motor_drive >  127 ) fw->motor_drive =  127;
		if( fw->motor_drive < -127 ) fw->motor_drive = -127;

		// and finally set the motor control value
		setLFly( fw->motor_drive );
		str = sprintf( str, "%4d %4d  %5.2f", fw->target,  fw->current, nImmediateBatteryLevel/1000.0 );
		displayLCDString(0, 0, str );
		str = sprintf( str, "%4.2f %4.2f ", fw->drive, fw->drive_at_zero );
		displayLCDString(1, 0, str );
		// Run at somewhere between 20 and 50mS
		wait1Msec(40);
	}
}

/*-----------------------------------------------------------------------------*/
/** @brief     Task to control the velocity of the right flywheel              */
/*-----------------------------------------------------------------------------*/
task rightFwControlTask()
{
	fw_controller *fw = rFly;

	// We are using Speed geared motors
	// Set the encoder ticks per revolution
	fw->ticks_per_rev = fw->MOTOR_TPR;

	while(1)
	{
		// debug counter
		fw->counter++;

		// Calculate velocity
		FwCalculateSpeed(fw, nMotorEncoder[flyWheelR2]);

		// Set current speed for the tbh calculation code
		fw->current = fw->v_current;

		// Do the velocity TBH calculations
		FwControlUpdateVelocityTbh( fw ) ;

		// Scale drive into the range the motors need
		fw->motor_drive  = (fw->drive * FW_MAX_POWER) + 0.5;

		// Final Limit of motor values - don't really need this
		if( fw->motor_drive >  127 ) fw->motor_drive =  127;
		if( fw->motor_drive < -127 ) fw->motor_drive = -127;

		// and finally set the motor control value
		setRFly( fw->motor_drive );

		// Run at somewhere between 20 and 50mS
		wait1Msec( FW_LOOP_SPEED );
	}
}


task flashLED() {
	while (1) {

		if(lFly.current >= 70 && lFly.current <= 75 && rFly.current >= 70 && rFly.current <= 75) {
			SensorValue[led] = true;
			} else {
			SensorValue[led] = false;
		}
	}
}


void pre_auton()
{
	bLCDBacklight = true;
	checkBatteries();
	startTask(selectionController);

	bStopTasksBetweenModes = true;
}

//prepare to use TBH for flywheel velocity control
void initializeTBH() {
	tbhInit(lFly, 627.2, 0.01025);//1025); //initialize TBH for left side of the flywheel
	tbhInit(rFly, 627.2, 0.01000);//1000); //initialize TBH for the right side of the flywheel
	//motor[intake] = 127;
	//start the flywheel control tasks
	startTask(leftFwControlTask);
	startTask(rightFwControlTask);
}

void stopFlywheel() {
	//disable the control tasks to allow manual, programmatic control of the flywheel motors (switch to open-loop control, essentially)
	stopTask(leftFwControlTask);
	stopTask(rightFwControlTask);
	stopTask(flashLED);

	//set the motors for both sides of the flywheel to 0 to actually stop the flywheels
	setLFly(0);
	setRFly(0);
}

void moveIntake(int encoderCount, int power) {
	int initialEncoder = nMotorEncoder[intakeBack];
	int encoderGoal = initialEncoder + encoderCount;
	while (nMotorEncoder[intakeBack] <= encoderGoal) {
		motor[intakeBack] = power;
	}
	motor[intakeBack] = 0;
}

void moveIntakeTop(int time, int power) {
	motor[intakeTop] = power;
	wait1Msec(time);
	motor[intakeTop] = 0;
}

task autonomous()
{
	initializeTBH();
	FwVelocitySet(lFly, 72, 0.709);
	FwVelocitySet(rFly, 72, 0.709);
	wait1Msec(2810); //wait time 1st ball
	motor[intakeTop] = 125;
	wait1Msec(1220); //2nd ball wait
	moveIntake(621, 125);
	wait1Msec(1220); //3rd
	moveIntake(921, 125);
	wait1Msec(1320); //4th
	moveIntake(921, 125);
}




float normalizeMotorPower (float value) {
	return value/(float) 127;
}

task usercontrol()
{
	bool flywheelRunning = false;
	int LY = 0;
	int LX = 0;
	int RY = 0;
	int RX = 0;
	int threshold = 15;
  while(1)
  {
  	//for deadzones; when the joystick value for an axis is below the threshold, the motors controlled by that joystick will not move in that direction
  	LY = (abs(vexRT[Ch3]) > threshold) ? vexRT[Ch3] : 0;
  	LX = (abs(vexRT[Ch4]) > threshold) ? vexRT[Ch4] : 0;
  	RY = (abs(vexRT[Ch2]) > threshold) ? vexRT[Ch2] : 0;
  	RX = (abs(vexRT[Ch1]) > threshold) ? vexRT[Ch1] : 0;
    motor[lFrontDrive] = LY + LX;
  	motor[lBackDrive] = LY - LX;
  	motor[rFrontDrive] = RY + RX;
  	motor[rBackDrive] = RY - RX;

		if (vexRT[Btn6U] == 1) {
			motor[intakeBack] = 125;
			}
		else if (vexRT[Btn6D] == 1) {
			motor[intakeBack] = -125;
			}
		else
		{
			motor[intakeBack] = 0;
		}

		if (vexRT[Btn5U] == 1) {
			motor[intakeTop] = 125;
			}
		else if (vexRT[Btn5D] == 1) {
			motor[intakeTop] = -125;
			}
		else
		{
			motor[intakeTop] = 0;
		}

		if (vexRT[Btn7D] == 1 && !flywheelRunning) {
			initializeTBH();
			FwVelocitySet(lFly, 72, 0.709);
			FwVelocitySet(rFly, 72, 0.709);
			flywheelRunning = true;
		}

		if (vexRT[Btn8D] == 1 && flywheelRunning) {
			stopFlywheel();
			flywheelRunning = false;
		}
		wait1Msec(25);
	}
}
