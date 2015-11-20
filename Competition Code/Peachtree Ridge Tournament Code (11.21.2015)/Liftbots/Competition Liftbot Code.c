#pragma config(Sensor, dgtl1,  PneuF1,         sensorDigitalOut)
#pragma config(Sensor, dgtl2,  PneuF2,         sensorDigitalOut)
#pragma config(Sensor, dgtl3,  PneuB1,         sensorDigitalOut)
#pragma config(Sensor, dgtl4,  PneuB2,         sensorDigitalOut)
#pragma config(Motor,  port1,           LBdrive,       tmotorVex393_HBridge, openLoop, driveLeft)
#pragma config(Motor,  port2,           LFdrive,       tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           ramp1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           ramp2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           flywheel1,     tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           flywheel2,     tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           intake1,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           intake2,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           RFdrive,       tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port10,          RBdrive,       tmotorVex393_HBridge, openLoop, reversed, driveRight)
#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
int time = 0;
int play = 0;
int LCDselect = 1;
void waitForRelease(){
	while(nLCDButtons != 0){}
	wait1Msec(5);
}
task LCD()
{
	//Declare count variable to keep track of our choice
	int count = 0;
	int screen = 0;
	int selected = 0;
	LCDselect = 0;
	//------------- Beginning of User Interface Code ---------------
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
	//Loop while center button is not pressed
	while(LCDselect == 0 && nSysTime < 20000)
	{
		//Switch case that allows the user to choose from 4 different options
		while(count == 0 && nSysTime < 20000){
			//Display first choice
			displayLCDCenteredString(0, "No Wait");
			displayLCDCenteredString(1, "<		 Enter		>");
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count = 5;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			else if(nLCDButtons == centerButton){
				time = 1;
				LCDselect = 1;
				selected++;
				break;
				waitForRelease();

			}
		}
		while(count == 1 && nSysTime < 20000){
			//Display second choice
			displayLCDCenteredString(0, "1");
			displayLCDCenteredString(1, "<		 Enter		>");
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			else if(nLCDButtons == centerButton){
				OoT = 1;
				BoR = 2;
				LCDselect = 1;
				selected++;
				break;
				waitForRelease();

			}
		}
		while(count == 2 && nSysTime < 20000){
			//Display third choice
			displayLCDCenteredString(0, "2 Seconds");
			displayLCDCenteredString(1, "<		 Enter		>");
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			else if(nLCDButtons == centerButton){
				OoT = 2;
				BoR = 1;
				LCDselect = 1;
				selected++;
				break;
				waitForRelease();

			}
		}
		while(count == 3 && nSysTime <20000){
			//Display fourth choice
			displayLCDCenteredString(0, "3 Seconds");
			displayLCDCenteredString(1, "<		 Enter		>");
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count = 4;
			}
			else if(nLCDButtons == centerButton){
				OoT = 2;
				BoR = 2;
				LCDselect = 1;
				selected++;
				break;
				waitForRelease();
			}
		}
		while(count == 4 && nSysTime <20000){
			//Display fourth choice
			displayLCDCenteredString(0, "4 Seconds");
			displayLCDCenteredString(1, "<		 Enter		>");
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			else if(nLCDButtons == centerButton){
				OoT = 0;
				BoR = 1;
				LCDselect = 1;
				selected++;
				break;
				waitForRelease();
			}
		}
		while(count == 5 && nSysTime <20000){
			//Display fourth choice
			displayLCDCenteredString(0, "5 Seconds");
			displayLCDCenteredString(1, "<		 Enter		>");
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count = 0;
			}
			else if(nLCDButtons == centerButton){
				OoT = 0;
				BoR = 2;
				LCDselect = 1;
				selected++;
				break;
				waitForRelease();
			}

		}
	}
	while(selected == 1 && LCDselect == 0 && nSysTime < 20000)
	{

	}
	clearLCDLine(0);
	clearLCDLine(1);
}

//------------- End of User Interface Code ---------------------

//------------- Beginning of Robot Movement Code ---------------
//Clear LCD
//Switch Case that actually runs the user choice



//------------- End of Robot Movement Code -----------------------



void LCDselection(){

	if(OoT == 1 && BoR == 1){
		startTask(compensation);
		startTask(autonomous2);
		wait1Msec(15000);
		stopTask(compensation);
		stopTask(autonomous2);
	}

	else if(OoT == 1 && BoR == 2){
		startTask(compensation);
		startTask(autonomous3);
		wait1Msec(15000);
		stopTask(compensation);
		stopTask(autonomous3);

	}

	else if(OoT == 2 && BoR == 1){
		startTask(compensation);
		startTask(autonomous3);
		wait1Msec(15000);
		stopTask(compensation);
		stopTask(autonomous3);
	}

	else if(OoT == 2 && BoR == 2){
		startTask(compensation);
		startTask(autonomous2);
		wait1Msec(15000);
		stopTask(compensation);
		stopTask(autonomous2);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................

	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	// User control code here, inside the loop
  while(1 == 1)
  {
  	motor[LBdrive] = vexRT[Ch3];		//sets drive motor speeds to joysticks
  	motor[LFdrive] = vexRT[Ch3];
  	motor[RBdrive] = vexRT[Ch2];
  	motor[RFdrive] = vexRT[Ch2];

    if(vexRT[Btn5U] == 1)						//intakes balls
    {
    	motor[intake1] = 117;
    	motor[intake2] = 117;
    }
    else if (vexRT[Btn5D] == 1)					//outputs balls
    {
    	motor[intake1] = -117;
    	motor[intake2] = -117;
    }
    else														//turns off intake
    {
    	motor[intake1] = 0;
    	motor[intake2] = 0;
    }

    if(vexRT[Btn8D])						//lowers ramp
    {
    	motor[ramp1] = 110;
    	motor[ramp2] = 110;
    }
    else if(vexRT[Btn8U])						//pulls in ramp pushers
    {
    	motor[ramp1] = -110;
    	motor[ramp2] = -110;
    }
    else
    {
    	motor[ramp1] = 0;
    	motor[ramp2] = 0;
    }
    if(vexRT[Btn6D] == 1)
    {
    	motor[flywheel1] = 127;
    	motor[flywheel2] = 127;
    }
    else if(vexRT[Btn6U] == 1)
    {
    	motor[flywheel2] = -127;
    	motor[flywheel1] = -127;
    }
    else
    {
    	motor[flywheel1] = 0;
    	motor[flywheel2] = 0;
    }
    if(vexRT[Btn7U] == 1)
    {
    	SensorValue[PneuF1] = 1;
    	SensorValue[PneuF2] = 1;
    	SensorValue[PneuB1] = 1;
    	SensorValue[PneuB2] = 1;
    }
		else if(vexRT[Btn7D] == 1)
		{
			SensorValue[PneuF1] = 1;
			SensorValue[PneuF2] = 1;
		}
    else
    {
    	SensorValue[PneuF1] = 0;
    	SensorValue[PneuF2] = 0;
    	SensorValue[PneuB1] = 0;
    	SensorValue[PneuB2] = 0;
   	}
    if(!bVEXNETActive)
   	{
   		SensorValue[PneuF1] = 0;
    	SensorValue[PneuF2] = 0;
    	SensorValue[PneuB1] = 0;
    	SensorValue[PneuB2] = 0;
  	}

   }
}
