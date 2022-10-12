
#include "Node.h"
#include "Queue.h"
#include <iostream>

using namespace std;

void Queue::enqueue(int R, int C) {
  Node* newNode = new Node;
  newNode->row = R; //implement number input
  newNode->column = C;
  newNode->point = NULL;
  if ((front==NULL)&&(rear==NULL)) {
    front = newNode;
    rear = newNode;
  }
  else {
    rear->point = newNode;
    rear = newNode;
  }
  queueLength++;
}

void Queue::dequeue() {
  if (front==NULL) {
    cout<<"Queue is empty"<<endl;
  }
  else if (front==rear) {
    removedR = front->row;
    removedC = front->column;
    
    delete front;
    front = NULL;
    rear = NULL;
    queueLength--;
  }
  else {
    removedR = front->row;
    removedC = front->column;

    Node* temp = front;
    front = front->point;
    delete temp;
    queueLength--;
  }
}

void Queue::display() {
  Node* temp = front;
  if (temp==NULL) {
    cout<<"null"<<endl;
  }
  while (temp!=NULL) {
    cout<<temp->row<<endl;
    temp = temp->point;
  }
}

int Queue::getHeadRow() {
  if (rear==NULL) {
    cout<<"Queue is empty"<<endl;
    return -1;
  }
  return rear->row;
}

int Queue::getHeadColumn() {
  if (rear==NULL) {
    cout<<"Queue is empty"<<endl;
    return -1;
  }
  return rear->column;
}

int Queue::getTailRow() {
  if (front==NULL) {
    cout<<"Queue is empty"<<endl;
    return -1;
  }
  return front->row;
}

int Queue::getTailColumn() {
  if (front==NULL) {
    cout<<"Queue is empty"<<endl;
    return -1;
  }
  return front->column;
}
