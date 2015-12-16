#pragma config(Motor,  port1,           LFfly,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           LBfly,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           RBfly,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           RFfly,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LBdrive,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           RBdrive,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           RFdrive,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           LFdrive,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           Lintake,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          Rintake,       tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
  while(1 == 1)
  {
  	motor[LBdrive] = vexRT[Ch3];
  	motor[LFdrive] = vexRT[Ch3];
  	motor[RBdrive] = vexRT[Ch2];
  	motor[RFdrive] = vexRT[Ch2];
    if(vexRT[Btn5U] == 1)
    {
    		motor[Lintake] = 120;
    		motor[Rintake] = 120;
    }
    else if(vexRT[Btn5D] == 1)
    {
    		motor[Lintake] = -120;
    		motor[Rintake] = -120;
    }
    else
    {
    		motor[Lintake] = -0;
    		motor[Rintake] = -0;  	}

    if(vexRT[Btn6U] == 1)
    {
      	motor[LBfly] = 120;
				motor[RBfly] = 120;
				motor[LFfly] = 120;
				motor[RFfly] = 120;
    }
    if(vexRT[Btn6D] == 1)
    {
      	motor[LBfly] = -120;
				motor[RBfly] = -120;
				motor[LFfly] = -120;
				motor[RFfly] = -120;
    }
    else
    {
      	motor[LBfly] = 0;
				motor[RBfly] = 0;
				motor[LFfly] = 0;
				motor[RFfly] = 0;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++