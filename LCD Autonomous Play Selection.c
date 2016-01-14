/* About all these constants:
To reduce the cyclic complexity of this code (especially for deciding which autonomous program to run),
numbers have been used to represent many of the possibilities.  Once the selection process is done, this means
that, because of how the variables' values work out when added, that the autonomous program to run can be found
just by adding the values assigned to each question, based on the users' answers.  The possibilities have been verified
to have no overlap to ensure the program will run correctly.

If a question is added at a later date, verify that the possibilities do not overlap (very important).*/

//constants; some of these numbers correspond with LCD button values
const int LEFT_BUTTON = 1;
const int CENTER_BUTTON = 2;
const int RIGHT_BUTTON = 4;

const int RED = LEFT_BUTTON;
const int BLUE = RIGHT_BUTTON;
const int SKILLS = CENTER_BUTTON;

//these variables are used in the skills challenge selection question to represent button presses (improves readability)
const int ROBOT_SKILLS = LEFT_BUTTON;
const int PROG_SKILLS = RIGHT_BUTTON;

//these variables represent the answers in the skills challenge selection question (ensures proper program is selected)
const int ROBOT_SKILLS_ANSWER = 10;
const int PROG_SKILLS_ANSWER = 16;


const int SIDE = 10;
const int TIMING = 20;
const int SKILLS_SELECT = 40;
const int NO = LEFT_BUTTON;
const int YES = RIGHT_BUTTON;
const int CONFIRM_QUESTION = 30; //the variable that leads to the final confirmatory question

int question = SIDE;

string pgmName; //for confirming choice
int sumChoices; //for determining which program to run

//autonomous play selection variables
int allianceColor; //possible values: 1, 4
bool delayStart; //possible values: true, false
int skillsMode; //possible values: 10, 16
//poss
bool selectionDone = false;

void waitForPress() {
	while (nLCDButtons == 0) {
		//wait until a button on the LCD is pressed
	}
}

void waitForRelease() {
	while (nLCDButtons > 0) {
		//wait until no buttons on the LCD are pressed
	}
}

int waitForButtonPress() {
	int btnPressed;
	waitForPress();
	btnPressed = nLCDButtons;
	waitForRelease();
	return btnPressed; //return the buttons that were pressed
}

void resetSelections() {
	question = SIDE;
	//make sure none of these variables have values, so that when the program decides which autonomous program to run, it doesn't run the wrong one
	//because one of these variables still had a value
	allianceColor = 0;
	delayStart = false;
	skillsMode = 0;
	sumChoices = 0;
}

void getProgramToRun(int sum, bool run) {

	switch (sum) {
	case 1:
		if(run) {
			//do something
		} else {
			pgmName = "Red, no wait";
		}
		break;
	case 2:
		if(run) {
			//do something
		} else {
			pgmName = "Red, wait";
		}
		break;
	case 4:
		if(run) {
			//do something
		} else {
			pgmName = "Blue, no wait";
		}
		break;
	case 5:
		if(run) {
			//do something
		} else {
			pgmName = "Blue, wait";
		}
		break;
	case 10:
		if(run) {
			//do something
		} else {
			pgmName = "Robot skills";
		}
		break;
	case 16:
		if(run) {
			//do something
		} else {
			pgmName = "Prog. skills";
		}
		break;
	}
}

task confirmChoice() {
	sumChoices = allianceColor + delayStart + skillsMode; //sum the values of the users' responses to determine which autonomous program to run
	getProgramToRun(sumChoices,false);
	while(!selectionDone) {
		displayLCDCenteredString(0,"Confirm choice:");
		displayLCDCenteredString(1,pgmName);
		wait1Msec(2000);
		displayLCDCenteredString(0, "Center - confirm");
		displayLCDCenteredString(1, "L/R - start over");
		wait1Msec(2000);
	}
}

void showQuestion(int type) {
	int choice;
	switch(type) {
	case 10: //side
		displayLCDCenteredString(0, "Which side?");
		displayLCDCenteredString(1, "Red   Sk.   Blue");
		choice = waitForButtonPress();
		if (choice == RED) {
			allianceColor = RED;
			question = TIMING;
			} else if (choice == BLUE) {
			allianceColor = BLUE;
			question = TIMING;
			} else if (choice == SKILLS) {
			question = SKILLS_SELECT;
		}
		break;
	case 20: //timing
		displayLCDCenteredString(0, "Wait at start?");
		displayLCDCenteredString(1, "No           Yes");
		choice = waitForButtonPress();
		if (choice == NO) {
			delayStart = false;
			question = CONFIRM_QUESTION;
			} else if (choice == YES) {
			delayStart = true;
			question = CONFIRM_QUESTION;
		}
		break;
	case 30: //confirm
		startTask(confirmChoice);
		choice = waitForButtonPress();
		stopTask(confirmChoice);
		if (choice == CENTER_BUTTON) {
			selectionDone = true;
			} else if (choice == LEFT_BUTTON || choice == RIGHT_BUTTON) {
			resetSelections();
		}
		break;

	case 40: //skills mode
		displayLCDCenteredString(0, "Which skills?");
		displayLCDCenteredString(1, "Robot      Prog.");
		choice = waitForButtonPress();
		if (choice == ROBOT_SKILLS) {
			skillsMode = ROBOT_SKILLS_ANSWER;
			question = CONFIRM_QUESTION;
			} else if (choice == PROG_SKILLS) {
			skillsMode = PROG_SKILLS_ANSWER;
			question = CONFIRM_QUESTION;
		}
		break;
	}
}



task selectionController ()
{
	bLCDBacklight = true;
	question = 10;
	while (nPgmTime <= 20000 && !selectionDone) {
		switch(question) {
		case 10: //side
			showQuestion(SIDE);
			break;
		case 20: //timing
			showQuestion(TIMING);
			break;
		case 30: //confirm
			showQuestion(CONFIRM_QUESTION);
			break;
		case 40: //skills
			showQuestion(SKILLS_SELECT);
			break;
		}
		wait1Msec(25); //don't hog the CPU
	}
	if(!selectionDone) {
		resetSelections(); //if no selection is confirmed, make sure nothing runs during autonomous
	}
	displayLCDCenteredString(0,"Play selected");
	displayLCDCenteredString(1,"");
	getProgramToRun(sumChoices,true);
	wait1Msec(5000);
}