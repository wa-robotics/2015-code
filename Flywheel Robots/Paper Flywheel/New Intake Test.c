#pragma config(Motor,  port1,            ,             tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
while(1) {
	motor[port1] = vexRT[Btn6D] == 1 ? 125 : 0;

}


}