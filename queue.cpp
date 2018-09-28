//
// queue.cpp
// Written by: Aaron Barlow
// 03/4/2016
//
// General queue code library that allows the intialization, insertion, and
// removal of the last element in queue, and whether or not the queue is empty
//

#include <iostream>
#include <cstdlib>
#include "queue.h"
using namespace std;

//
// newQueue
// Initialize a new queue and set first and last
// pointers to null before returning the queue.
//
Queue *newQueue() {
  Queue *q = new Queue();
  q->last = q->first = NULL;
  return q;
}

//
// insert
// Inserts into the back node while setting the next and prev
// pointers to point to the appropriate queue.
//
void insert(Queue *q, void *data) {
  Node *new_node = new Node();
  new_node->data = data;
  new_node->next = new_node->prev = NULL;

  if(q->last == NULL) {
    q->first = new_node;
  } else {
    new_node->prev = q->last;
    q->last->next = new_node;
  }

  q->last = new_node;
}

//
// remove
// Remove from the front node while setting the next and prev
// pointers to point to the appropriate queue. Then return
// a pointer to the removed node.
//
void *remove(Queue *q) {
  if(q->first == NULL) return NULL;
  void *removed_node = q->first->data;
  q->first->prev = NULL;

  if(q->first->next == NULL) {
    q->first = NULL;
    q->last = NULL;
  } else {
    q->first = q->first->next;
  }

  return removed_node;
}

//
// queue_empty
// Returns whether or not the queue is empty by checking
// whether the last or first pointers point to anything.
//
bool queue_empty(Queue *q) {
  return q->last == NULL && q->first == NULL ? true : false;
}
