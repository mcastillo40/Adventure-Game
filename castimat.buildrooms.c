/*
 * CS 344 Assignment 2
 * Name: Matthew Castillo
 * Date: 10/22/17
 * Description: The program implements a dynamic array to initialize 
 *  a stack, bag, ordered bag, heap, and iterator
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

typedef enum {false, true} bool;

struct Room {
	int id;
    char* name;
	int connectionCount;
	struct room* outBoundConnections[6];
};

// Create all connections in graph
//while (IsGraphFull() == false)
//{
//	  AddRandomConnection();
//}

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
//bool IsGraphFull()  
//{
//    return true;
//}

// Adds a random, valid outbound connection from a Room to another Room
//void AddRandomConnection()  
//{
//	Room A;  // Maybe a struct, maybe global arrays of ints
//    Room B;

//    while(true)
//    {
//	    A = GetRandomRoom();

//        if (CanAddConnectionFrom(A) == true)
//		    break;
//    }

//    do
//    {
//        B = GetRandomRoom();
//    }
//    while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

//    ConnectRoom(A, B);  // TODO: Add this connection to the real variables, 
//    ConnectRoom(B, A);  //  because this A and B will be destroyed when this function terminates
//}

// Returns a random Room, does NOT validate if connection can be added
//Room GetRandomRoom()
//{
//    return Room;
//}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
//bool CanAddConnectionFrom(Room x) 
//{
//    return true;
//}

// Returns true if a connection from Room x to Room y already exists, false otherwise
//bool ConnectionAlreadyExists(x, y)
//{
//    return false;
//}

// Connects Rooms x and y together, does not check if this connection is valid
//void ConnectRoom(Room x, Room y) 
//{

//}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(const struct Room* x, const struct Room* y) 
{
    return x == y; 
}

int main() { 
	int result = mkdir("testFolder", 0755);
	printf("New: %d\n", result);

	return 0;
}
