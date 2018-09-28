//
// tickets.cpp
// Written by: Aaron Barlow
// 03/4/2016
//
// Simulates a ticket processing system that will read in orders,
// process the orders, and put the orders in a queue
// as the orders are recieved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include "tickets.h"
using namespace std;

int main() {
  // Instantiate order and queue
  Order *order = new Order();
  order->queue = newQueue();

  // Get the file if possible
  ifstream input;
  get_file( input );

  // Confirmation number must start at 1
  order->confirmation_number = 1;
  // Config current time
  order->current_time.tm_hour = 0;
  order->current_time.tm_min  = 0;
  order->current_time.tm_sec  = 0;
  // Config starting number of tickets used
  order->num_tickets_used = 0;
  // Set starting tick_time to compare with current time
  order->tick_time = order->current_time;

  // Continue to simulate processing the tickets until all tickets are processed
  while( order->num_tickets_used != NUM_TICKETS_AVAILABLE &&
      ( !queue_empty(order->queue) || !input.eof() ) ) {

    // Simulate getting orders
    order->get_orders( input );

    // Simulate processing the order
    order->process_order();
  }

  // Delete dynamically allocated memory
  delete order->queue;
  delete order;

  return 0;
}


//
// get_file
// Open the file. If it exists and has content pass back input.
// Otherwise, exit the program with an appropriate error message.
//
void get_file( ifstream& input ) {
  // Read in file
  input.open(FILE_NAME);

  // When file could not be found
  if( input.fail() ) {
    cout << "Input file " << FILE_NAME << " does not exist. \n";
    exit(1);

  // When file is empty
  } else if( input.peek() == EOF ) {
    cout << "Input file " << FILE_NAME << " has no tickets to read. \n";
    exit(1);
  }
}

//
// get_orders
// Continue to read in orders until the tick time
// is greater than the current time. If it is the
// end of input, close the file once.
//
void Order::get_orders( ifstream& input ) {
  while( !input.eof() && mktime(&tick_time) <= mktime(&current_time) ) {

    // Read in the order from the file
    input >> tick_time.tm_sec;

    input >> first_name;
    input >> last_name;
    input >> num_tickets;

    // Copy the vital order objects
    Order *copied_order = new Order();
    copied_order->first_name  = first_name;
    copied_order->last_name   = last_name;
    copied_order->tick_time   = tick_time;
    copied_order->num_tickets = num_tickets;

    // Insert the copied order into the queue
    insert( queue, (void*)copied_order);

    // Close the file once when at the end of file
    if( input.eof() ) input.close();

  }
}

//
// process_order
// Simulate time passing and then ensure queue is not empty.
// When the tick time is less than or equal to the current time,
// read that order in and then print it. If an order will use all
// the available tickets, give the customer as many as possible and
// then inform all customers whose orders could not go through that
// there are no more tickets.
//
void Order::process_order() {
  // Simulate sleep time
  sleep(SLEEP_TIME);
  current_time.tm_sec += SLEEP_TIME;

  // Check if there are any orders to process
  if( queue_empty(queue) ) return;
  Order *queued_order = (Order*)queue->first->data;

  // When an order can be processed
  if( mktime(&queued_order->tick_time) <= mktime(&current_time) ) {

    // If it is possible to process the requested number of tickets
    if( (queued_order->num_tickets + num_tickets_used) < NUM_TICKETS_AVAILABLE ) {
      // Increase the tickets used by the number of tickets requested
      num_tickets_used += queued_order->num_tickets;
      // Print and remove order
      print_order();
      remove(queue);
    } else { // It is not possible to process all of the requested tickets
      int tickets_processed = 0;
      int tickets_not_processed = queued_order->num_tickets;

      // Determine number of tickets that can and can not be processed
      while( num_tickets_used < NUM_TICKETS_AVAILABLE ) {
        num_tickets_used++;
        tickets_processed++;
      }

      // Give the customer as many tickets as possible
      queued_order->num_tickets = tickets_processed;
      print_order();
      // Then inform all customers that orders can not be processed
      queued_order->num_tickets = tickets_not_processed - tickets_processed;
      sold_out();
    }
  }
}

//
// print_order
// Print order's time in 00:00:00 format, last name,
// first name, and number of tickets requested.
//
void Order::print_order() {
  // Ensure there is something in the queue to work with
  if( queue_empty(queue) ) return;

  Order *processed_order = (Order*)queue->first->data;

  // Print the successfull order for the requested number of tickets
  cout << setfill('0') << setw(2) << current_time.tm_hour << ":"
       << setfill('0') << setw(2) << current_time.tm_min << ":"
       << setfill('0') << setw(2) << current_time.tm_sec << " - "
       << "Order " << confirmation_number << ": "
       << processed_order->last_name << ", "
       << processed_order->first_name << " "
       << "(" << processed_order->num_tickets << ") tickets" << endl;

  // Increment confirmation number for next possible reiteration
  confirmation_number++;
}

//
// sold_out
// Print all the orders that could not be processed.
// Remove the order from the queue after printing the order.
// Inform the customer that all future tickets can not be processed
// because there are no more tickets to be processed.
//
void Order::sold_out() {
  while( !queue_empty(queue) ) {
    Order *order_data = (Order*)remove(queue);

  // Print the unsuccessfull order for the requested number of tickets
  cout << setfill('0') << setw(2) << current_time.tm_hour << ":"
       << setfill('0') << setw(2) << current_time.tm_min << ":"
       << setfill('0') << setw(2) << current_time.tm_sec
         << " - Tickets sold out --> Unable to process "
         << order_data->last_name << " "
         << order_data->first_name << " request for ("
         << order_data->num_tickets << ") tickets" << endl;

    // Delete dynamically allocated memory
    delete order_data;

  }

  // No more tickets available
  cout << "Tickets are sold out and any future tickets will not be processed." << endl;
}
