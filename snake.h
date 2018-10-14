const int GAME_OVER = -1, MOVED = 0, FOOD_EATEN = 1, BONUS_FOOD_EATEN = 2;
const int MAP_HEIGHT = 15, MAP_WIDTH = 30;
const int DIRECTION_UP = 1, DIRECTION_DOWN = 2, DIRECTION_LEFT = 3, DIRECTION_RIGHT = 4;
const char WALL_CHAR='#', SNAKE_CHAR='x', FOOD_CHAR='o', BONUS_FOOD_CHAR='S';
const int BONUS_CHANCE = 20; // Out of 100

struct Point {
	int y;
	int x;
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
void updateBonusFood();
int moveSnake(Snake* snake);
Point getEmptySpot();
