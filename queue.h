#pragma once
//
// queue.h
// Written by: Aaron Barlow
// 03/4/2016
//
// Header file for queue structures and prototypes.
//

// Node structure to point to other nodes. Each node contains data.
struct Node {
  Node *next, *prev;
  void *data;
};

// Queue structure pointing to a node.
struct Queue {
  Node *first, *last;
};

// Functions for queue
Queue *newQueue();
void insert(Queue *q, void *data);
void *remove(Queue *q);
bool queue_empty( Queue *q );