#include <iostream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <ncurses.h>
#include "snake.h"

using namespace std;

char gameMap[MAP_HEIGHT][MAP_WIDTH];
int direction = DIRECTION_RIGHT;
Point bonusFoodPoint;
int bonusFoodTime = -1;

int main(){

	int ch, result;
	bool running=TRUE;
	Point startPoint = {5,5};
	Node* firstNode = new Node{startPoint, nullptr};
	Snake snake = {firstNode, 1};

	initscr();
	if(has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);

	initializeVariables();

	printMap();
	gameMap[startPoint.y][startPoint.x] = SNAKE_CHAR;
	mvaddch(startPoint.y, startPoint.x, SNAKE_CHAR);
	generateFood();
	while(running) {
		if( (ch=getch()) != ERR) {
			// VIM key bindings
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
		usleep(500000);
		result = moveSnake(&snake);
		updateBonusFood();
		switch(result) {
		case GAME_OVER: running=FALSE;
		break;
		case FOOD_EATEN: generateFood();
		break;
		}
	}
	// ToDo: Delete snake entirely
	nodelay(stdscr, FALSE);
	clear();
	mvprintw(0,0, "Game over!");
	getch();
	endwin();
	return 0;
}

void generateFood(){

	if ( bonusFoodTime == -1 && rand() % 100 <= BONUS_CHANCE) {
		bonusFoodPoint = getEmptySpot();
		bonusFoodTime = 20;
		gameMap[bonusFoodPoint.y][bonusFoodPoint.x] = BONUS_FOOD_CHAR;
		attron(COLOR_PAIR(3));
		mvaddch(bonusFoodPoint.y, bonusFoodPoint.x, BONUS_FOOD_CHAR);
		attroff(COLOR_PAIR(3));
	}
	Point foodPoint = getEmptySpot();
	int randY = foodPoint.y, randX = foodPoint.x;
	gameMap[randY][randX] = FOOD_CHAR;
	attron(COLOR_PAIR(2));
	mvaddch(randY, randX, FOOD_CHAR);
	attroff(COLOR_PAIR(2));
}

void updateBonusFood(){
	if(bonusFoodTime == -1)
		return;
	if (--bonusFoodTime == -1){
		gameMap[bonusFoodPoint.y][bonusFoodPoint.x] = ' ';
		mvaddch(bonusFoodPoint.y, bonusFoodPoint.x, ' ');
		bonusFoodPoint = {-1, -1};

	}
}

Point getEmptySpot(){
	int randX = rand() % MAP_WIDTH;
	int randY = rand() % MAP_HEIGHT;
	while (gameMap[randY][randX] != ' ') {
		randX = rand() % MAP_WIDTH;
		randY = rand() % MAP_HEIGHT;
	}
	Point p = {randY, randX};
	return p;
}

void initializeVariables(){
	for (int i=1; i<MAP_WIDTH-1; i++)
		for (int j=1; j<MAP_HEIGHT-1; j++)
			gameMap[j][i] = ' ';
	for (int i=0; i<MAP_WIDTH; i++) {
		gameMap[0][i] = WALL_CHAR;
		gameMap[MAP_HEIGHT-1][i] = WALL_CHAR;
	}
	for (int j=1; j<MAP_HEIGHT-1; j++) {
		gameMap[j][0] = WALL_CHAR;
		gameMap[j][MAP_WIDTH-1] = WALL_CHAR;
	}
}

void printMap(){
	attron(COLOR_PAIR(1));
	for (int i=0; i<MAP_WIDTH; i++) {
		for (int j=0; j<MAP_HEIGHT; j++)
			mvaddch(j, i, gameMap[j][i]);
	}
	attroff(COLOR_PAIR(1));
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

	if (gameMap[yCoord][xCoord] == WALL_CHAR || gameMap[yCoord][xCoord] == SNAKE_CHAR) return GAME_OVER;
	else {
		Point newHeadPoint = {yCoord, xCoord};
		snake->enqueue(newHeadPoint);
		int foodStatus = MOVED;
		if(gameMap[yCoord][xCoord] == FOOD_CHAR) foodStatus=FOOD_EATEN;
		else if(gameMap[yCoord][xCoord] == BONUS_FOOD_CHAR) foodStatus=BONUS_FOOD_EATEN;
		gameMap[yCoord][xCoord] = SNAKE_CHAR;
		mvaddch(yCoord, xCoord, SNAKE_CHAR);

		if(foodStatus == MOVED) {
			Point prevPoint = snake->dequeue();
			gameMap[prevPoint.y][prevPoint.x] = ' ';
			mvaddch(prevPoint.y, prevPoint.x, ' ');
		}
		return foodStatus;
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
		delete(head);
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
