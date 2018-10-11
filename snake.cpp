#include <iostream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <ncurses.h>

using namespace std;

#define MAP_HEIGHT 15
#define MAP_WIDTH 30
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4
#define MAX_SNAKE_LENGTH 100

struct Point {
	int x;
	int y;
};

struct Node {
	Point point;
	Node* next;
};

class Snake {
public:
void enqueue(Point);
Point dequeue();
Node* head;
int length;
};

void initializeVariables();
void printMap();
void generateFood();
int moveSnake(Snake* snake);

const int GAME_OVER = -1, MOVED = 0, FOOD_EATEN = 1;

char gameMap[MAP_HEIGHT][MAP_WIDTH];
int direction = DIRECTION_RIGHT;

int main(){

	int ch, result;
	bool running=TRUE;
	Point startPoint = {5,5};
	Node* firstNode = new Node{startPoint, nullptr};
	Snake snake = {firstNode, 1};

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);

	initializeVariables();

	gameMap[startPoint.y][startPoint.x] = 'x';

	printMap();
	generateFood();
	while(running) {
		if( (ch=getch()) != ERR) {
			switch(ch) {
			case 'h': if(direction != DIRECTION_RIGHT)
					direction = DIRECTION_LEFT;
				break;
			case 'l': if(direction != DIRECTION_LEFT)
					direction = DIRECTION_RIGHT;
				break;
			case 'j': if(direction != DIRECTION_UP)
					direction = DIRECTION_DOWN;
				break;
			case 'k': if(direction != DIRECTION_DOWN)
					direction = DIRECTION_UP;
				break;
			}
		}
		result = moveSnake(&snake);
		switch(result) {
		case GAME_OVER: running=FALSE;
		case FOOD_EATEN: generateFood();
		}
		usleep(500000);
	}
	nodelay(stdscr, FALSE);
	clear();
	mvprintw(0,0, "Game over!");
	getch();
	endwin();
	return 0;
}

void generateFood(){
	int randX = rand() % MAP_WIDTH;
	int randY = rand() % MAP_HEIGHT;
	while (gameMap[randY][randX] != ' ') {
		randX = rand() % MAP_WIDTH;
		randY = rand() % MAP_HEIGHT;
	}
	gameMap[randY][randX] = 'o';
	mvaddch(randY, randX, 'o');
}

void initializeVariables(){
	for (int i=1; i<MAP_WIDTH-1; i++)
		for (int j=1; j<MAP_HEIGHT-1; j++)
			gameMap[j][i] = ' ';
	for (int i=0; i<MAP_WIDTH; i++) {
		gameMap[0][i] = '#';
		gameMap[MAP_HEIGHT-1][i] = '#';
	}
	for (int j=1; j<MAP_HEIGHT-1; j++) {
		gameMap[j][0] = '#';
		gameMap[j][MAP_WIDTH-1] = '#';
	}
}

void printMap(){
	for (int i=0; i<MAP_WIDTH; i++) {
		for (int j=0; j<MAP_HEIGHT; j++)
			mvaddch(j, i, gameMap[j][i]);
	}
}

int moveSnake(Snake* snake){
	int xCoord = snake->head->point.x;
	int yCoord = snake->head->point.y;
	switch (direction) {
	case DIRECTION_RIGHT: xCoord++;
		break;
	case DIRECTION_LEFT: xCoord--;
		break;
	case DIRECTION_DOWN: yCoord++;
		break;
	case DIRECTION_UP: yCoord--;
		break;
	}

	if (gameMap[yCoord][xCoord] == '#') return GAME_OVER;
	else {
		Point newHeadPoint = {xCoord, yCoord};
		snake->enqueue(newHeadPoint);
		bool food = FALSE;
		if(gameMap[yCoord][xCoord] == 'o') food=TRUE;
		gameMap[yCoord][xCoord] = 'x';
		mvaddch(yCoord, xCoord, 'x');

		if(!food) {
			Point prevPoint = snake->dequeue();
			gameMap[prevPoint.y][prevPoint.x] = ' ';
			mvaddch(prevPoint.y, prevPoint.x, ' ');
			return MOVED;
		} else {
			return FOOD_EATEN;
		}
	}
}

void Snake::enqueue(Point point)
{
	Node* newhead = new Node{
		point,
		head
	};
	head = newhead;
	length++;
}

Point Snake::dequeue()
{
	Point p = {-1, -1};
	if(!head) return p;
	if(!head->next)
	{
		p = head->point;
		head = nullptr;
		length--;
		return p;
	}

	Node* temphead = head;
	while(temphead->next->next) temphead = temphead->next;

	p = temphead->next->point;
	delete temphead->next;
	temphead->next = nullptr;
	length--;
	return p;
}
