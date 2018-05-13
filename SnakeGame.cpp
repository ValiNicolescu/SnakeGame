#include <iostream>
#include <conio.h>
#include <windows.h>


#include "Menu.h"
using namespace std;

#define MAX_LEVEL (5)
#define OBST_LEN  (5)
#define BODY_PER_LEVEL (10)
#define SNAKE_LEN ((BODY_PER_LEVEL) * (MAX_LEVEL))

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY,score;
int tailX[SNAKE_LEN], tailY[SNAKE_LEN];
int obstX[MAX_LEVEL*OBST_LEN], obstY[MAX_LEVEL*OBST_LEN];
int crtLevel = 1;
int nTail;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

TMenuStruct mainMenu[] = {
	{'1', "Joc Nou"},
	{'2',"Scoruri"},
	{'q',"Iesire"},
	{0,  NULL}
};

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void GenOneObstacle(int idx);
bool isClear(int l, int c);

void gotoXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void DrawMap()
{
	gotoXY(0, 0);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	for (int j = 0; j < height; j++)
	{
		cout << "#";
		for (int i = 0; i < width; i++)
		{
			cout << " ";
		}
		cout << "#" << endl;

	}

	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;


}
void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
	DrawMap();
}

void Draw()
{
	gotoXY(x + 1, y + 1);
	cout << "0";
	if (nTail > 0)
	{
		gotoXY(tailX[0] + 1, tailY[0] + 1);
		cout << "o";
	}
	gotoXY(tailX[nTail] + 1, tailY[nTail] + 1);
	cout << " ";

	gotoXY(fruitX + 1, fruitY + 1);
	cout << "F";


	for (int i = 0; score >= 10 && i < crtLevel * OBST_LEN; i++)
	{
		gotoXY(obstX[i]+1, obstY[i]+1);
		cout << "@";
	}

	gotoXY(0, height + 2);
	cout << "Score: " << score << " | Level:" << crtLevel;

	gotoXY(79, 24);

}
void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}
void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	if (nTail != 0)
	{
		tailX[nTail] = prevX;
		tailY[nTail] = prevY;
	}
	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default:
		break;
	}
	//if (x > width || x < 0 || y > height || y < 0)
		//gameOver = true;
	if (x >= width) x = 0; else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height - 1;
	for (int i = 0; !gameOver && i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	for ( int i = 0; !gameOver && i < crtLevel * OBST_LEN; i++)

		if (obstX[i] == x && obstY[i] == y)
			gameOver = true;


	if (x == fruitX && y == fruitY)
	{
		int new_x, new_y;

		score += 10;
		do {
			new_x = rand() % width;
			new_y = rand() % height;
		} while (!isClear(new_y, new_x));
		fruitX = new_x;
		fruitY = new_y;
		nTail++;

		if (score % 100 == 0)
			crtLevel++;

		if (score == 10 || score % 100 == 0)
			GenOneObstacle((crtLevel - 1) * OBST_LEN);

	}

}

bool isClear(int l, int c)
{
	// sa nu fie in afara
	if (l < 0 || c < 0 || l >= height || c >= width)
		return false;
	// sa nu fie fruct
	if (l == fruitY && c == fruitX)
		return  false;

	// sa nu fie sarpe
	if (l == y && c == x)
		return false;

	for (int i = 0; i < nTail; i++)
		if (l == tailY[i] && c == tailX[i])
			return false;

	// sa nu fie obstacol
	for (int i = 0; i < crtLevel * OBST_LEN; i++)
		if (l == obstY[i] && c == obstX[i])
			return false;

	return true;
}
void GenOneObstacle(int idx)
{
	const int nv = 8;
	const int  vy[nv] = { -1,-1,-1,0,1,1,1,0 };
	const int vx[nv] = { -1,0,1,1,1,0,-1,-1 };
	int new_x, new_y;
	do {
		new_x = OBST_LEN + rand() % (width - 2 * OBST_LEN);
		new_y = OBST_LEN + rand() % (height - 2 * OBST_LEN);
	} while (!isClear(new_y, new_x));
	obstX[idx] = new_x;
	obstY[idx] = new_y;

	for (int i = idx + 1; i < idx + OBST_LEN; i++)
	{
		do {
			int v = rand() % nv;
			new_x = obstX[i - 1] + vx[v];
			new_y = obstY[i - 1] + vy[v];
		} while (!isClear(new_y, new_x));
		obstX[i] = new_x;
		obstY[i] = new_y;
	}
	 

}

void DrawMenu(struct menu_struct* myMenu)
{
	struct menu_struct* opt;
	
	if (myMenu == NULL)
		return;

	opt = myMenu;

	while (opt->text) {
		cout << opt->opt << " - " << opt->text << endl;
		opt++;
	}
	
}
int menu(struct menu_struct*  myMenu)
{
	if (myMenu == NULL)
		return -1;

	system("cls");

	DrawMenu(myMenu);
	while (1) {
		char c = _getch();
		
		for (struct menu_struct* opt = myMenu; opt->text != NULL; opt++)
			if (opt->opt == c)
				return c;
	}
}
int main()
{
	SYSTEMTIME st;
	char c;
	GetSystemTime(&st);

	srand(st.wMilliseconds);

	system("cls");
	while (1) {
		c = menu(mainMenu);
		switch (c) {
		case 'q':
			return 0;
		case '1':
			gotoXY(0, 0);
			Setup();
			while (!gameOver)
			{
				Draw();
				Input();
				Logic();
				Sleep(150);
			}

			while (!_kbhit()) {
				gotoXY(6, 10);
				cout << "GAME OVER" << endl;
				Sleep(500);
				gotoXY(6, 10);
				cout << "         " << endl;
				Sleep(200);
			}
			_getch();
			break;
		}
	}
	return 0;
}
