#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

class Queue {
  public:
  void enqueue(int, int);
  void dequeue();
  void display();
  int getHeadRow();
  int getHeadColumn();
  int getTailRow();
  int getTailColumn();
  int removedR;
  int removedC;
  int queueLength = 0;
};

#endif
