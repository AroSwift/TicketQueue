#pragma once
//
// tickets.h
// Written by: Aaron Barlow
// 03/4/2016
//
// Header file for constants, order struct,
// and prototypes for ticket processing program.
//

#include <string>
#include "queue.h"
using namespace std;

// Constants
const char FILE_NAME[] = "tickets.dat";
const int NUM_TICKETS_AVAILABLE = 100;
const int SLEEP_TIME = 10;

// Order structure
struct Order {
  Queue *queue;
  tm current_time;
  tm tick_time;
  int confirmation_number;
  int num_tickets;
  int num_tickets_used;
  string first_name;
  string last_name;

  // Methods
  void get_orders( ifstream& input );
  void process_order();
  void print_order();
  void sold_out();
};

void get_file( ifstream& input );
