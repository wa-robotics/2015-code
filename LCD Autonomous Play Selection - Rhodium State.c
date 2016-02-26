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
const int ROBOT_SKILLS_ANSWER = 25;
const int PROG_SKILLS_ANSWER = 26;

const int	COLOR = 10;
const int TILE = 20;
const int PATH = 30;
const int WAIT = 35;
const int SKILLS_SELECT = 40;
const int NO = LEFT_BUTTON;
const int YES = RIGHT_BUTTON;
const int CONFIRM_QUESTION = 50; //the variable that leads to the final confirmatory question

int question = COLOR;

string pgmToRun; //for confirming choice
int sumChoices; //for determining which program to run

//autonomous play selection variables
int allianceColor; //possible values: 1, 4
int startTile;
int autonPath;
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
	question = COLOR;
	//make sure none of these variables have values, so that when the program decides which autonomous program to run, it doesn't run the wrong one
	//because one of these variables still had a value
	allianceColor = 0;
	delayStart = false;
	skillsMode = 0;
	autonPath = 0;
	startTile = 0;
	sumChoices = 0;
}

void getProgramToRun(int sum, bool run, bool delayStart) {

	switch (sum) {
	case 15:
		if(run) {
			//do something
			} else {
			pgmToRun = "R Back Long";
		}
		break;
	case 19:
		if(run) {
			//do something
			} else {
			pgmToRun = "R Back Close";
		}
		break;
	case 17:
		if(run) {
			//do something
			} else {
			pgmToRun = "R Side Long";
		}
		break;
	case 21:
		if(run) {
			//do something
			} else {
			pgmToRun = "R Side Close";
		}
		break;
	case 18:
		if(run) {
			//do something
			} else {
			pgmToRun = "B Back Long";
		}
		break;
	case 22:
		if(run) {
			//do something
			} else {
			pgmToRun = "B Back Close";
		}
		break;
	case 20:
		if(run) {
			//do something
			} else {
			pgmToRun = "B Side Long";
		}
		break;
	case 24:
		if(run) {
			//do something
			} else {
			pgmToRun = "B Side Close";
		}
		break;
	case 25:
		if(run) {
			//do something
			} else {
			pgmToRun = "Robot skills";
		}
		break;
	case 26:
		if(run) {
			//do something
			} else {
			pgmToRun = "Prog. skills";
		}
		break;
	}
}

task confirmChoice() {
	sumChoices = allianceColor + skillsMode + startTile + autonPath; //sum the values of the users' responses to determine which autonomous program to run
	getProgramToRun(sumChoices,false, delayStart);
	while(!selectionDone) {
		displayLCDCenteredString(0,"Confirm choice:");
		displayLCDCenteredString(1,pgmToRun);
		wait1Msec(2000);
		displayLCDCenteredString(0, "Center - confirm");
		displayLCDCenteredString(1, "L/R - start over");
		wait1Msec(2000);
	}
}

void showQuestion(int type) {
	int choice;
	switch(type) {
	case 10: //color
		displayLCDCenteredString(0, "Which side?");
		displayLCDCenteredString(1, "Red   Sk.   Blue");
		choice = waitForButtonPress();
		if (choice == RED) {
			allianceColor = RED;
			question = TILE;
			} else if (choice == BLUE) {
			allianceColor = BLUE;
			question = TILE;
			} else if (choice == SKILLS) {
			question = SKILLS_SELECT;
		}
		break;
	case 20: //tile
		displayLCDCenteredString(0, "Start tile?");
		displayLCDCenteredString(1, "Side        Back");
		startTile = waitForButtonPress();
		if (startTile == LEFT_BUTTON) {
			startTile = 6;
			question = PATH;
			}	else if (startTile == RIGHT_BUTTON) {
			startTile = 4;
			question = PATH;
		}
		break;

	case 30: //path
		displayLCDCenteredString(0, "Shot type?");
		displayLCDCenteredString(1, "Long       Close");
		autonPath = waitForButtonPress();
		if (autonPath == LEFT_BUTTON) {
			autonPath = 10;
			question = WAIT;
			}	else if (autonPath == RIGHT_BUTTON) {
			autonPath = 14;
			question = WAIT;
		}
		break;
	case 35: //timing
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

	case 50: //confirm
		startTask(confirmChoice);
		choice = waitForButtonPress();
		stopTask(confirmChoice);
		if (choice == CENTER_BUTTON) {
			selectionDone = true;
			} else if (choice == LEFT_BUTTON || choice == RIGHT_BUTTON) {
			resetSelections();
		}
		break;
	}
}



task selectionController ()
{
	bLCDBacklight = true;
	question = 10;
	while (nPgmTime <= 30000 && !selectionDone) {
		switch(question) {
		case 10: //color
			showQuestion(COLOR);
			break;
		case 20: //tile
			showQuestion(TILE);
			break;
		case 30: //path
			showQuestion(PATH);
			break;
		case 35: //wait
			showQuestion(WAIT);
			break;
		case 40: //skills
			showQuestion(SKILLS_SELECT);
			break;
		case 50: //confirm
			showQuestion(CONFIRM_QUESTION);
			break;
		}
		wait1Msec(25); //don't hog the CPU
	}
	if(!selectionDone) {
		resetSelections(); //if no selection is confirmed, make sure nothing runs during autonomous
	}
	displayLCDCenteredString(0,"Play selected");
	displayLCDCenteredString(1,"");
	getProgramToRun(sumChoices,true, delayStart);
}
