#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl12, led,            sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           lDriveFront,   tmotorVex393HighSpeed_HBridge, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port2,           lDriveBack,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           flywheelL1,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           flywheelR2,    tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port5,           intakeRoller,  tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           intakeChain,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           flywheelR1,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           flywheelL2,    tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port9,           rDriveBack,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rDriveFront,   tmotorVex393HighSpeed_HBridge, openLoop, reversed, encoderPort, I2C_3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)
int testing = 0;
//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
#include "TBH Controller with Averaging RPM.h" //Contains the TBH algorithm for the flywheels; only included here to provide RPM output

fw_controller lFly, rFly; //Create a controller for the TBH algorithm for each side of the flywheel
string str;
void pre_auton()
{
}

task autonomous()
{
}

int flashTime = 1000;
task flashLED() {
	while(1) {
		SensorValue[led] = true;
		wait1Msec(flashTime);
		SensorValue[led] = false;
		wait1Msec(flashTime);
	}
}

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
        getEncoderAndTimeStamp(flyWheelL2,fw->e_current, fw->encoder_timestamp);
        FwCalculateSpeed(fw);

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
        wait1Msec( FW_LOOP_SPEED );
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
        getEncoderAndTimeStamp(flyWheelR2,fw->e_current, fw->encoder_timestamp);
        FwCalculateSpeed(fw);

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

//prepare to use TBH for flywheel velocity control
void initializeTBH() {
	tbhInit(lFly, 392, 0.000600);//.0002 //initialize TBH for left side of the flywheel
	tbhInit(rFly, 392, 0.000600); //.000275 //initialize TBH for right side of the flywheel
	//motor[intake] = 127;
	//start the flywheel control tasks
	startTask(leftFwControlTask);
	startTask(rightFwControlTask);
}

void stopFlywheel() {
	//disable the control tasks to allow manual, programmatic control of the flywheel motors (switch to open-loop control, essentially)
	stopTask(leftFwControlTask);
	stopTask(rightFwControlTask);

	//set the motors for both sides of the flywheel to 0 to actually stop the flywheels
	setLFly(0);
	setRFly(0);
	motor[intakeRoller] = 0;
}

float normalizeMotorPower (float value) {
	return value/(float) 127;
}
void stopMotors()
{
	  motor[lDriveFront] = 0;
    motor[rDriveFront] = 0;
    motor[lDriveBack] = 0;
    motor[rDriveBack] = 0;
}
void driveRobot(int direction, int speed, int time)
{
		motor[lDriveFront] = -direction*speed;
    motor[rDriveFront] = -direction*speed;
    motor[lDriveBack] = -direction*speed;
    motor[rDriveBack] = -direction*speed;
    wait10Msec(time);
		stopMotors();
 }
//int rPower=75, lPower=55;
task usercontrol()
{
	while(1) {
	//	writeDebugStreamLine("%d,%d,%d,%d,%d,%d,%d",nPgmTime, lFly.current, lFly.motor_drive, lFly.raw_last_rpm, rFly.current, rFly.motor_drive, rFly.raw_last_rpm);
	//	wait1Msec(25);
	    //Driving Motor Control
    motor[lDriveFront] = -vexRT[Ch2];
    motor[rDriveFront] = -vexRT[Ch3];
    motor[lDriveBack] = -vexRT[Ch2];
    motor[rDriveBack] = -vexRT[Ch3];
    //Arm Control
    if(testing == 3) //Tests auto
    {
    	driveRobot(1,100,350);
    	initializeTBH();
      FwVelocitySet(lFly, 30, normalizeMotorPower(60));
			FwVelocitySet(rFly, 25, normalizeMotorPower(50));
			motor[intakeRoller] = 90;
			motor[intakeChain] = -90;
			driveRobot(-1,100,20);
			driveRobot(1,0,100000);
    	wait10Msec(1000000000000000000000000000000000000000000000000000000000000000000000000);
    }
    if(vexRT[Btn6D] == 1)
    {
      motor[intakeRoller] = 127;
    }
    else if(vexRT[Btn6U] == 1)
    {
      motor[intakeRoller] = -127;
    }
    else
    {
      motor[intakeRoller] = 0;
    }
    if(vexRT[Btn5U] == 1)
    {
      motor[intakeChain] = 127;
    }
    else if(vexRT[Btn5D] == 1)
    {
      motor[intakeChain] = -127;
    }
    else
    {
      motor[intakeChain] = 0;
    }
    if(vexRT[Btn7D] == 1)
    {
    		initializeTBH();
      	FwVelocitySet(lFly, 130, normalizeMotorPower(75));
				FwVelocitySet(rFly, 130, normalizeMotorPower(75));
    }
    else if(vexRT[Btn7L] == 1)
    {
    		initializeTBH();
      	FwVelocitySet(lFly, 45, normalizeMotorPower(60));
				FwVelocitySet(rFly, 45, normalizeMotorPower(60));
    }
    else if(vexRT[Btn8D] == 1)
    {
     stopFlywheel();
    }
		}
}
