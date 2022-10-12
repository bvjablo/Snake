#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <cstdlib>
#include "Node.h"
#include "Queue.h"
#include "Snake.h"

using namespace std;

//This contructor initializes the array to blank characters
Snake::Snake() {
  for (int i=0; i<NUM_ROW; i++) {
    for (int j=0; j<NUM_COL; j++) {
      mainArray[i][j] = ' ';
    }
  }
}

void Snake::setDifficulty() {
  nodelay(stdscr, FALSE);
  printw("Please Select The Game Difficulty: (Chaos Mode Recommended)\nHard:   Type 'H'\nNormal: Type 'N'\nEasy:   Type 'E\nChaos:  Type 'C'\nBonus:  Type 'B'\n");
  refresh();
  char userInput;
  userInput = wgetch(stdscr);
  while ((userInput != 'H') && (userInput != 'h') && (userInput != 'N') && (userInput != 'n') && (userInput != 'E') &&
         (userInput != 'e') && (userInput != 'C') && (userInput != 'c') && (userInput != 'B') && (userInput != 'b')) {
    printw(" is not a valid option\nPlease try again\n");
    refresh();
    userInput = wgetch(stdscr);
  }

  if ((userInput == 'H') || (userInput == 'h')) {
     gamePace = 120000;
     printw("\nHard Mode Selected\n");
  }
  else if ((userInput == 'N') || (userInput == 'n')) {
    gamePace = 150000;
    printw("\nNormal Mode Selected\n");
  }
  else if ((userInput == 'E') || (userInput == 'e')) {
    gamePace = 200000;
    printw("\nEasy Mode Selected\n");
  }
  else if ((userInput == 'C') || (userInput == 'c')) {
    gamePace = 100000;
    chaosMode = true;
    printw("\nChaos Mode Selected.... Good Luck\n");
  }
  else {
    gamePace = 120000;
    bonusMode = true;
    printw("\nBonus Mode Selected\n");
  }
  refresh();
  sleep(2);
}

//This method initialzes the gamescreen and adds the snake's starting postition
void Snake::initializeSnake() {
  initscr();
  keypad(stdscr, true);
  srand(time(0));

  nodelay(stdscr, FALSE);
  setDifficulty();
  clear();

  //add starting snake body
  queue->enqueue(2,7);
  mainArray[2][7] = 'O';
  queue->enqueue(3,7);
  mainArray[3][7]= 'O';
  queue->enqueue(4,7);
  mainArray[4][7]= '^';


  generateFood();                                                                                                                                                                          
  display();                                                                                                                                                                                
  refresh();
}

void Snake::runGame() {
  initializeSnake();
  
  //Runs until invalid move is played
  while (run) {
    direction = wgetch(stdscr);
    //enables real-time input of keyboard
    nodelay(stdscr, true);
    while(setArray()){                                                                                                                                                      
      clear();
      display();
      printw("\nScore: %d\n", score);
      
      refresh();
      //controls speed of snake
      usleep(gamePace);
      keyCheck();
    }
    run = false;
  }
  clear();
  display();
  printw("\nScore: %d\n", score);
  
  printw("You Lose! Press Any Key to return to the terminal.");
  refresh();
  sleep(1);
  nodelay(stdscr, FALSE);
  char temp;
  temp = wgetch(stdscr);
  clear();
  endwin();
}

//This method uses a for loop to repeatedly display the values stored in the array
void Snake::display() {

for (int i=0; i<NUM_ROW; i++) { 
  for (int j=0; j<NUM_COL; j++) {
    if (j==14) {
      printw("X\n");
    }
    else if ((j==0) || (i==0) || (i==14)) {
      printw("X ");
    }
    else {
      printw("%c ", mainArray[i][j]);
    }
  }
 }
}


//Utilizes the order of the snake queue to set the array with the correct values to display
//Returns false if invalid postition is detected
bool Snake::setArray() {
  
  int headRow = queue->getHeadRow();
  int headColumn = queue->getHeadColumn();
  bool isFood = false;

  mainArray[headRow][headColumn] = 'O'; //change head to body segment

  //validate and update snake position
  //left
  if (direction == 260) {
    if ((headColumn == 1) || (mainArray[headRow][headColumn-1] == 'O') || (mainArray[headRow][headColumn-1] == 'X')) {
      return false; //prevent index out of bounds
    }
    if (mainArray[headRow][headColumn-1] == 'F') {
      isFood = true;
    }
    mainArray[headRow][headColumn-1] = '>';
    queue->enqueue(headRow, (headColumn-1));
  }

  //right
  else if (direction == 261) {
    if ((headColumn == 13) || (mainArray[headRow][headColumn+1] == 'O') || (mainArray[headRow][headColumn+1] == 'X')) {
      return false;
    }
    if (mainArray[headRow][headColumn+1] == 'F') {
      isFood = true;
    }
    mainArray[headRow][headColumn+1] = '<';
    queue->enqueue(headRow, (headColumn+1));
  }
  
  //up
  else if (direction == 259) {
    if ((headRow == 1) || (mainArray[headRow-1][headColumn] == 'O') || (mainArray[headRow-1][headColumn] == 'X')) {
      return false;
    }
    if (mainArray[headRow-1][headColumn] == 'F') {
      isFood = true;
    }
    mainArray[headRow-1][headColumn] = 'v';
    queue->enqueue((headRow-1), headColumn);
  }
  
  //down
  else if (direction == 258) {
    if ((headRow == 13) || (mainArray[headRow+1][headColumn] == 'O') || (mainArray[headRow+1][headColumn] == 'X')) {
      return false;
    }
    if (mainArray[headRow+1][headColumn] == 'F') {
      isFood = true;
    }
    mainArray[headRow+1][headColumn] = '^';
    queue->enqueue((headRow+1), headColumn);
  }

  //remove tail of snake unless food is eaten
  if (isFood) {
    score++;
    if (chaosMode) {
      generateWall();
    }
    if (bonusMode) {
      generateFood();
    }
    generateFood();
  }
  else {
    queue->dequeue();
    mainArray[queue->removedR][queue->removedC] = ' ';
  }
  
  return true;
}

//checks user input for new key direction
void Snake::keyCheck() {
  int temp = wgetch(stdscr);
  if (temp != ERR) {
    //260 - left, 261 - right, 259 - down, 258 - up
    if ((temp!=direction) && ((temp==260)||(temp==261)||(temp==259)||(temp==258))) {
      direction = temp;
    }
  }
  else {
    //do nothing
  }
}

void Snake::generateFood() {
  int y, x;
  bool isTaken = true;
  while (isTaken) {
    //only generate between 1 and 13 since 0 and 14 are used for the game borders
    y = rand()%12 +1;
    x = rand()%12 +1;
    if (mainArray[y][x] == ' ') {
      isTaken = false;
    }
  }
  mainArray[y][x] = 'F';
}

void Snake::generateWall() {
  int y, x;
  bool isTaken = true;
  while (isTaken) {
    y = rand()%12 +1;
    x = rand()%12 +1;
    if (mainArray[y][x] == ' ') {
      isTaken = false;
    }
  }
  mainArray[y][x] = 'X';
}

void Snake::reset() {
  clear();
  for (int i=0; i<NUM_ROW; i++) {
    for (int j=0; j<NUM_COL; j++) {
      mainArray[i][j] = ' ';
    }
  }


  while (queue->queueLength > 0) {
    queue->dequeue();
  }
}

