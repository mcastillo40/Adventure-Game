/*
 *
 * CS 344 Assignment 2
 * Name: Matthew Castillo
 * Date: 10/22/17
 * Description: 
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

typedef enum {false, true} bool;

struct Room {
	char* name; 
    char* type;
	int outboundConnectionCount;
	struct Room* outboundRoomConnection; 
};

struct roomType {
	int size;
	char* [][];
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

int main() {rand
	int processID = getpid();   // Obtain process ID number
	char tempID[20];		    // Use temp to convert int to string
	char folderName[80];	    // Folder to hold rooms
	char roomNames[10][50];     // The name of the rooms that may be chosen in the game
	char roomType[3][15];	    // The room types in the game	
	bool startRoomUsed = false; // Used to validate if start room was used
	bool endRoomUsed = false;	// Used to validate if end room was used
	struct Room* houseRooms; 
	houseRooms = malloc(7 * sizeof(struct Room));

	srand(time(NULL));

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

	// Place type of rooms 
	strcpy(roomType[0], "MID_ROOM");
	strcpy(roomType[1], "START_ROOM");
	strcpy(roomType[2], "END_ROOM");

	// Initialize each room with a name and room type
	int i;
	for (i = 0; i < 7; ++i) {
		int randomRoomName;
				
		// Initialize 1st room's name and type
		if (i == 0) {
			randomRoomName = rand() % 10;
			randomRoomType = rand() % 3;
			
			if (randomRoomType == 1)
				startRoomUsed = true;

			if (randomRoomType == 2)
				endRoomUsed = true; 

			houseRooms[i].name = roomNames[randomRoomName];
			houseRooms[i].type = roomType[randomRoomType]; 
		}
		else { // Initialize the final 6 room's names and types and ensure that the name is unique
			
			bool usedName = false; // Will change to true if room name has been used
			bool usedType = false; // Will change to true if room type is either START_ROOM OR END_ROOM
				
			do {
				usedName = false; 
				
				randomRoomName = rand() % 10; 
				houseRooms[i].name = roomNames[randomRoomName];
				
				// Check whether the room name has been used
				// If used change usedName to true
				int count;
				for (count = 0; count < i; ++count){
					if (houseRooms[i].name == houseRooms[count].name){
						usedName = true; 
					}
				}

			} while (usedName);

			do {
				usedType = false;

				randomRoomType = rand() % 3;
				houseRooms[i].type = roomType[randomRoomType];

				if (houseRooms[i].type == roomType[1] || houseRooms[i].type == roomType[2])
					usedType = true; 
				
			} while (usedType);

		} 
	}

	do {
		randomRoomType = rand() % 3;
			
		if (randomRoomType == 1)
			startRoomUsed = true;

		if (randomRoomType == 2)
			endRoomUsed = true; 

		int count;
		for (count = 0; count < 7; ++count)
		{
			houseRooms[count].type = roomType[randomRoomType]; 	
		}

	} while (!startRoomUsed || !endRoomUsed);

	int count = 0; 
	for (count = 0; count < 7; ++count) {
		printf("Room Name %d: %s\n", count, houseRooms[count].name); 
		printf("Room Type %d: %s\n", count, houseRooms[count].type);
	}

	// Convert process ID into string and concatenate into folder name
	sprintf(tempID, "%d", processID);
	strcpy(folderName, "castimat.rooms.");
	//	strcat(folderName, tempID);

	int result = mkdir(folderName, 0755);
	printf("Complete: %d\n", result);
	printf("My process ID : %d\n", processID);
	
	// Delete folder (Create loop to delete all files within folder first openDir() and closeDir())
//	rmdir(folderName);
	free(houseRooms);
	return 0;
}
