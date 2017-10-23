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
	char* name; 
    char* type;
	int outboundConnectionCount;
	struct Room* outboundRoomConnection; 
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
//	struct Room A;  // Maybe a struct, maybe global arrays of ints
//	struct Room B;

//    while(true)
//   {
//	    A = GetRandomRoom();

//        if (CanAddConnectionFrom(A) == true)
//		    break;
//    }

//    do
//    {
//        B = GetRandomRoom();
//    }
//    while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

    //ConnectRoom(A, B);  // TODO: Add this connection to the real variables, 
    //ConnectRoom(B, A);  //  because this A and B will be destroyed when this function terminates
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
//
//}

// Returns true if Rooms x and y are the same Room, false otherwise
//bool IsSameRoom(Room x, Room y) 
//{
//    return false;
//}

int main() {
	int processID = getpid(); // Obtain process ID number
	char tempID[20];		  // Use temp to convert int to string
	char folderName[80];	  // Folder to hold rooms
	char roomNames[9][50];    // The name of the rooms that may be chosen in the game

	// Place name of rooms in array
	strcpy(roomNames[0], "Whacky Room");
	strcpy(roomNames[1], "Jumpy Room");
	strcpy(roomNames[2], "Coffee Room");
	strcpy(roomNames[3], "Where Am I Room");
	strcpy(roomNames[4], "Rick's Garage");
	strcpy(roomNames[5], "This Room");
	strcpy(roomNames[6], "Change Room");
	strcpy(roomNames[7], "All you can eat Room");
	strcpy(roomNames[8], "Hat collection Room");
	strcpy(roomNames[9], "Another Room");

	printf("room 1: %s\n", roomNames[0]);
	printf("room 2: %s\n", roomNames[1]);
	
	// Convert process ID into string and concatenate into folder name
	sprintf(tempID, "%d", processID);
	strcpy(folderName, "castimat.rooms.");
	//	strcat(folderName, tempID);

	int result = mkdir(folderName, 0755);
	printf("Complete: %d\n", result);
	printf("My process ID : %d\n", processID);

	return 0;
}
