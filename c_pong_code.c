#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>

// macros
#define ARR_LEN(array) sizeof(array)/sizeof(array[0]) //size of array
#define SET_SEED(seed) srand((unsigned)seed) //set seed for randomness

//enums
enum {
    KEY_SPACE = 32,

    KEY_ZERO = 48,
    KEY_ONE = 49,
    KEY_TWO = 50,
    KEY_THREE = 51,
    KEY_FOUR = 52,
    KEY_FIVE = 53,
    KEY_SIX = 54,
    KEY_SEVEN = 55,
    KEY_EIGHT = 56,
    KEY_NINE = 57,

    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
	KEY_ESC = 27
}; //ascii keys to decimal value

int i, j, score1, score2, spd;
int x, y, cpux, cpuy, targetx, targety, target; //target 1 = targets cpu

float ball_x, ball_y;
//ball- ball = -1 means its going up, 1 means going down, 1 is right, -1 is left
int ballV, ballH;

char grid[20][60];

int init;

char ch;

//ENUMS
typedef enum Scenes {
	Intro,
	Game,
	Pause
} Scenes;

typedef enum BallDir {
	Normal,
	Little,
	None
} BallDir;


BallDir Ball;
Scenes Scene = Intro;

//initializing
//FUNCTIONS
//change target
flipTarget() {
	if (target) {
		target = 0;
	}
	else {
		target = 1;
	}
	ball_x -= ballH;
	ballH = 0 - ballH;
}

//set target pos
setTarget() {
	if (target) {
		targetx = cpux;
		targety = cpuy;
	}
	else {
		targetx = x;
		targety = y;
	}
}

//reset ball pos
resetBall() {
	ball_x = ARR_LEN(grid[0]) / 2;
	ball_y = ARR_LEN(grid) / 2;
	Ball = None;
	flipTarget();
}

//reset values
void reset() {
	x = 0 + 2, y = ARR_LEN(grid) / 2;
	cpux = ARR_LEN(grid[0]) - 2 - 2, cpuy = ARR_LEN(grid) / 2;
	Ball = None;
	ballH = -1;
	ballV = -1;
	target = 0;

	score1 = 0, score2 = 0;

	ball_x = ARR_LEN(grid[0]) / 2;
	ball_y = ARR_LEN(grid) / 2;
}

//draws game
void drawGame() {
	//title
	printf("\nPong by alexa in C :-)       Move:  W/S    Quit:  Esc    Main menu:  R    Pause:  P   Difficulty: %d/3\n\n", spd);

	//make game
	for ( i = 0; i < ARR_LEN(grid); i++)
	{
		printf("#");
		for ( j = 0; j < ARR_LEN(grid[0]); j++)
		{
			if ( (i == 0) | (j == ARR_LEN(grid[0]) - 1) | (i == ARR_LEN(grid) - 1)) {
				printf("#");
			}
			else if (j==(ARR_LEN(grid[0])-1)/2) {
				printf(":");
			}
			else if ( (i == y && j == x) |
					(i == y - 1 && j == x) |
					(i == y + 1 && j == x) |
					(i == y - 2 && j == x) |
					(i == y + 2 && j == x) ) {
				printf("|");
			}
			else if ((i == cpuy && j == cpux) |
				(i == cpuy - 1 && j == cpux) |
				(i == cpuy + 1 && j == cpux) |
				(i == cpuy - 2 && j == cpux) |
				(i == cpuy + 2 && j == cpux)) {
				printf("|");
			}
			else if (i == (int)ball_y  && j == (int)ball_x) {
				printf("*");
			}
			else printf(" ");
		}
		printf("\n");
	}
	printf("\n   %d - YOU                 SCORES                 CPU - %d\n\n",score1,score2);
}

//MAIN FUNCTION
//

int main(){
	SET_SEED(time(NULL));
    while (1) {
		Sleep(16);

		//scene intro init
        if (Scene == Intro && init==0) {
			init = 1;
			system("cls");
			printf("\nPong by alexa in C :-)\n\nWelcome to pong!\n");
			printf("Enter your difficulty; The higher, the harder it will be to get a point..\nPress a number from 1 to 3\n");

			ch = toupper(_getch());
			if ( (ch == KEY_ONE) | (ch == KEY_TWO) | (ch == KEY_THREE) ) {
				switch (ch) {
				case KEY_ONE: spd = 1; break;
				case KEY_TWO: spd = 2; break;
				case KEY_THREE: spd = 3; break;
				}
				Scene = Game;
			}
			init = 0;
        }

		//scene game loop
		if (Scene == Game) {
			if (init == 0) {
				init = 1;
				reset();
			}
			system("cls");
			drawGame();

			//key press events
			if (_kbhit()) {
				ch = toupper(_getch());
				switch (ch) {
				case KEY_W: y-=1; break;
				case KEY_S: y+=1; break;
				case KEY_ESC: exit(0);
				case KEY_R: Scene = Intro; init = 0; break;
				case KEY_P: Scene = Pause; init = 0; break;
				//case KEY_P: Scene = Pause; init = 0; break;
				}
				printf("%c", ch);

			}

			//change pos of ball
			switch (Ball) {
			case Normal: ball_x += 0.7f * ballH; ball_y += 0.6f * ballV; break;
			case Little: ball_x += 0.9f * ballH; ball_y += 0.4f * ballV; break;
			case None: ball_x += 1.0f * ballH; break;
			}

			setTarget();

			//if ball touches one of the players
			if ((int)ball_x == targetx) {

				//mid
				if ((int)ball_y == targety) {
					Ball = None;
					flipTarget();
				}
				//little up
				if ( (int)ball_y == targety - 1 ) {
					Ball = Little;
					ballV = -1;
					flipTarget();
				}
				//little down
				if ((int)ball_y == targety + 1) {
					Ball = Little;
					ballV = 1;
					flipTarget();
				}
				//normal up
				if ( (int)ball_y == targety - 2 ) {
					Ball = Normal;
					ballV = -1;
					flipTarget();
				}
				//normal down
				if ((int)ball_y == targety + 2) {
					Ball = Normal;
					ballV = 1;
					flipTarget();
				}
			}

			//when ball is missed by a player
			if (ball_x <= 0) {
				resetBall();
				score2++;
			}
			else if (ball_x >= ARR_LEN(grid[0])-1) {
				resetBall();
				score1++;
			}

			//cpu moving
			int ai;
			switch (spd) {
			case 1: ai = rand() % 7; break;
			case 2: ai = rand() % 4; break;
			case 3: ai = rand() % 2; break;
			}
			if (ai==1) {
				if (cpuy != (int)ball_y && cpuy != (int)ball_y+1) {
					cpuy += ((int)(ball_y - cpuy) / abs((int)(ball_y - cpuy)));
				}
			}

			//if slides touch floor/ceil
			if (y == 0 + 2) {
				y++;
			}
			else if (y == ARR_LEN(grid) - 1 - 2) {
				y--;
			}
			if (cpuy == 0 + 2) {
				cpuy++;
			}
			else if (cpuy == ARR_LEN(grid) - 1 - 2) {
				cpuy--;
			}

			//if ball touches floor/ceil
			if ((int)ball_y == 1) {
				ballV = 1;
			}
			if ((int)ball_y == ARR_LEN(grid) -2 ) {
				ballV = -1;
			}
		}
		if (Scene == Pause) {
			if (init == 0) {
				init = 1;
				system("cls");
				drawGame();

				printf("GAME IS PAUSED. P TO UNPAUSE");
				ch = toupper(_getch());
				if (ch == KEY_P) {
					Scene = Game;
				}
				else init = 0;
			}
		}
    }
}
