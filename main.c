#include<stdio.h>
#include<stdlib.h>
#include<curses.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>
#include<sys/stat.h>
#ifdef _WIN32
	#include<windows.h>
#else
	#include<unistd.h>
#endif


char fileHighScore[14];

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

void scoreFileTest()
{
	FILE *highScore;
	int exists;
	#ifdef _WIN32
		struct _stat fileTest;
		exists = _stat(fileHighScore, &fileTest);
	#else
		struct stat fileTest;
		exists = stat(fileHighScore, &fileTest);
	#endif
	if (exists < 0)
	{
		highScore = fopen(fileHighScore, "w");
	}
}

void viewHighScore()
{
	FILE *highScore;
	char printHighScore[120];
	scoreFileTest();
	highScore = fopen(fileHighScore, "r");
	clrscr();
	printw("Simple Figures high score log:\n");
	while (fgets(printHighScore, 120, highScore) != NULL)
	{
		printw("%s", printHighScore);
		#ifdef _WIN32
			Sleep(70);
		#else
			usleep(70000);
		#endif
		if (!strcmp(printHighScore, "\n"))
			refresh();
	}
	fclose(highScore);
	pressContinue();
}

struct player playGame()
{
	struct player gamePlayer;
	FILE *highScore;
	int loopVar;
	int randomVar = 0;
	double value1 = 0, value2 = 0;
	double answer = 0, playerAnswer = 0;
	char operator;
	char gameMessage[64];
	char flt1[32], flt2[32];
	char highScoreText[64];
        time_t gameStart;
        time_t gameStop;
	srand(time(NULL));
	gamePlayer.score = 0;
	clrscr();
	printw("\nSimple Figures: New Game!\n");
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
		randomVar = rand() % 4;
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
		printw("\n%.0f %c %.0f = ?\nAnswer:\n", value1, operator, value2);
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
	/*gamePlayer.time = difftime(gameStop, gameStart);*/
	gamePlayer.time = gameStop - gameStart;
	scoreFileTest();
	sprintf(highScoreText, "\n%s:\nDifficulty: %i\nScore: %i\nTime: %.0lf\n", gamePlayer.name, gamePlayer.difficulty, gamePlayer.score, gamePlayer.time);
	highScore = fopen(fileHighScore, "a");
	fprintf(highScore, highScoreText);
	fclose(highScore);
	return gamePlayer;
}

int main()
{
	struct player mainPlayer;
	bool mainLoop = true;
	char userInput;
	strcpy(fileHighScore, "Highscore.txt");
	initscr();
	scrollok(stdscr, TRUE);
	while (mainLoop)
	{
		clrscr();
		printw("############################\nWelcome to Simple Figures!\n############################\n");
		printw("Options:\nP:\tPlay Game\nV:\tView Score Log\nQ:\tQuit\n");
		refresh();
		userInput = getch();
		if (userInput == 'p' || userInput == 'P')
		{
			mainPlayer = playGame();
			printw("\n%s played %i questions with a difficulty of %i and obtained a score of %i in %.0f seconds!", mainPlayer.name, mainPlayer.questions, mainPlayer.difficulty, mainPlayer.score, mainPlayer.time);
			pressContinue();
		}
		else if (userInput == 'v' || userInput == 'V')
		{
			viewHighScore();
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
