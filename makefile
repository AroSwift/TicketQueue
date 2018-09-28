CC = g++

all: ticket

ticket: tickets.o queue.o
  $(CC) -o ticket tickets.o queue.o

tickets.o: tickets.cpp tickets.h
  $(CC) -c tickets.cpp

queue.o: queue.cpp queue.h
	$(CC) -c queue.cpp

clean:
  rm -f a.out ticket *.o