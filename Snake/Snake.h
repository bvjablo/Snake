#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include "Queue.h"
using namespace std;


class Snake {
public:
  Snake();
  void display();
  bool setArray();
  void keyCheck();
  void generateFood();
  void generateWall();
  void initializeSnake();
  void runGame();
  void setDifficulty();
  void reset();


private: 
  int static const NUM_ROW = 15;
  int static const NUM_COL = 15;
  char mainArray[NUM_ROW][NUM_COL];
  bool run = true;
  Queue* queue = new Queue;
  int direction;
  int score = 0;
  int gamePace = 0;
  bool chaosMode = false;
  bool bonusMode = false;
};
#endif
