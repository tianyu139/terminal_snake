const int GAME_OVER = -1, MOVED = 0, FOOD_EATEN = 1;
const int MAP_HEIGHT = 15, MAP_WIDTH = 30;
const int DIRECTION_UP = 1, DIRECTION_DOWN = 2, DIRECTION_LEFT = 3, DIRECTION_RIGHT = 4;

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
