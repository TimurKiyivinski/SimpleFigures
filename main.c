#include<stdio.h>
#include<ncurses.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>

struct player
{
	char name[65];
	int difficulty;
	int questions;
	int score;
	double time;
};

/*	Press to continue*/
void pressContinue()
{
	printw("\nPress any key to continue.\n");
	refresh();
	getch();
}

/*	Clears the screen*/
void clrscr()
{
	clear();
	refresh();
}

struct player playGame()
{
	struct player gamePlayer;
	int loopVar;
	int randomVar = 0;
	double value1 = 0, value2 = 0;
	double answer = 0, playerAnswer = 0;
	char operator;
	char gameMessage[64];
	char flt1[32], flt2[32];
    time_t gameStart;
    time_t gameStop;
	srand(time(NULL));
	gamePlayer.score = 0;
	clrscr();
	printw("Please input your name:\n");
	refresh();
	getstr(gamePlayer.name);
	while (true)
	{
		printw("Please input the maximum value:\n");
		refresh();
		scanw(" %i", &gamePlayer.difficulty);
		if (gamePlayer.difficulty < 1)
			printw("Too simple!\n");
		else if (gamePlayer.difficulty % 10 != 0)
			printw("Must be a multiple of 10!\n");
		else
			break;
	}
	while (true)
	{
		printw("Please input the number of questions:\n");
		refresh();
		scanw(" %i", &gamePlayer.questions);
		if (gamePlayer.questions < 10)
			printw("You need to answer at least 10 questions!\n");
		else
			break;
	}
	strcpy(gameMessage, "Question number 1: ");
	time(&gameStart);
	for (loopVar = 0; loopVar < gamePlayer.questions; loopVar++)
	{
		clrscr();
		printw("Simple Figures!");
		printw("\n%s", gameMessage);
		//printw("\nQuestion number %i: ", loopVar + 1);
		//randomVar = rand() % 4;
		randomVar = 3;
		value1 = rand() % gamePlayer.difficulty + 1;
		value2 = rand() % gamePlayer.difficulty + 1;
		if (randomVar == 0)
		{
			/*	Addition*/
			operator = '+';
			answer = value1 + value2;
		}
		else if (randomVar == 1)
		{
			/*	Substraction*/
			operator = '-';
			answer = value1 - value2;
		}
		else if (randomVar == 2)
		{
			/*	Multiplication*/
			operator = 'x';
			answer = value1 * value2;
		}
		else
		{
			/*	Division*/
			operator = '/';
			answer = value1 / value2;
		}
		answer = floorf(answer * 100 + 0.5) / 100;
		printw("\n%.0f %c %.0f = ?\nAnswer: %lf\n", value1, operator, value2, answer);
		scanw(" %lf", &playerAnswer);
		playerAnswer = floorf(playerAnswer * 100 + 0.5) / 100;
		if (playerAnswer == answer)
		{
			gamePlayer.score++;
			sprintf(gameMessage, "Question number %i", loopVar + 2);
		}
		else
			strcpy(gameMessage, "Incorrect answer!");
	}
	time(&gameStop);
	gamePlayer.time = difftime(gameStop, gameStart);
	return gamePlayer;
}

int main()
{
	struct player mainPlayer;
	bool mainLoop = true;
	char userInput;
	initscr();
	while (mainLoop)
	{
		clrscr();
		printw("########################\nWelcome to Simple Figures!\n########################\n");
		printw("Options:\nP:\tPlay Game\nV:\tView Score Log\nQ:\tQuit\n");
		refresh();
		userInput = getch();
		if (userInput == 'p' || userInput == 'P')
		{
			mainPlayer = playGame();
			printw("\n%s played %i questions with a difficulty of %i and obtained a score of %i in %.0f seconds!", mainPlayer.name, mainPlayer.questions, mainPlayer.difficulty, mainPlayer.score, mainPlayer.time);
			pressContinue();
		}
		else if (userInput == 'q' || userInput == 'Q')
		{
			printw("\nBye! Thanks for playing!");
			pressContinue();
			mainLoop = false;
		}
	}
	endwin();
}
