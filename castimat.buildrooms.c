/*
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
	struct Room* outboundRoomConnection[6]; 
};

/*********************************************************************
*                 initRoomName(char**)
* @param roomName
* @return: void
* Description: Allocate memory for each room name then initialize 
* 	the room names
*********************************************************************/
void initRoomName(char ** roomName){
	int i;
	for (i = 0; i < 10; ++i) {
		roomName[i] = malloc(20 * sizeof(char*));
	}

	// Place name of rooms in arrayi
	strcpy(roomName[0], "Whacky Room");
	strcpy(roomName[1], "Jumpy Room");
	strcpy(roomName[2], "Coffee Room");
	strcpy(roomName[3], "Where Am I Room");
	strcpy(roomName[4], "Rick's Garage");
	strcpy(roomName[5], "This Room");
	strcpy(roomName[6], "Change Room");
	strcpy(roomName[7], "All you can eat Room");
	strcpy(roomName[8], "Hat collection Room");
	strcpy(roomName[9], "Another Room");
}

/*********************************************************************
*                 setRoomName(Room*, char**)
* @param houseRooms                
* @param roomName
* @return: void
* Description: Set each room with a room name and ensure that each name
* 	is unique. The function randomizes the names of each room. 
*********************************************************************/
void setRoomName(struct Room* houseRooms, char **roomName){
	int i;

	for (i = 0; i < 7; ++i) {
		int randomRoomName;
				
		// Initialize 1st room's name and type
		if (i == 0) {
			randomRoomName = rand() % 10;
			
			houseRooms[i].name = roomName[randomRoomName];
		}
		else { // Initialize the final 6 room's names and types and ensure that the name is unique

			bool usedName = false; // Will change to true if room name has been used
			
			// loop through room's name to validate that they are not the same
			do {
				usedName = false; 
				
				randomRoomName = rand() % 10; 
				houseRooms[i].name = roomName[randomRoomName];
				
				// Check whether the room name has been used
				int count;
				for (count = 0; count < i; ++count){
					if (houseRooms[i].name == houseRooms[count].name){
						usedName = true; 
					}
				}
			} while (usedName);
		} 
	}
}

/*********************************************************************
*                 initRoomType(char**)                
* @param roomType
* @return: void
* Description: Initialize the room type to be START_ROOM, MID_ROOM, and
* 	END_ROOM to be used for each room.
*********************************************************************/
void initRoomType(char **roomType){
	int i;
	for (i = 0; i < 3; ++i) {
		roomType[i] = malloc(20 * sizeof(char*));
	}
	
	// Place type of rooms 
	strcpy(roomType[0], "MID_ROOM");
	strcpy(roomType[1], "START_ROOM");
	strcpy(roomType[2], "END_ROOM");
}

/*********************************************************************
*                 setRoomType(Room*, char**)                
* @param houseRooms
* @param roomType
* @return: void
* Description: Have only 1 room to be a START_ROOM type another to be
* 	a END_ROOM type and the rest to be a MID_ROOM type. The function
* 	randomizes which room will be a certain type.
*********************************************************************/
void setRoomType(struct Room* houseRooms, char **roomType){
	int i;
	int startRandom = rand() % 7;
	int endRandom = rand() % 7; 

	// Set a random room as the start room
	houseRooms[startRandom].type = roomType[1];
	
	// Set a random room as the end room
	while (endRandom == startRandom) {
		endRandom = rand() % 7; 
	}
	houseRooms[endRandom].type = roomType[2];
	
	// Set the other rooms to mid rooms
	for (i = 0; i < 7; ++i) {
		if (i != startRandom && i != endRandom)
			houseRooms[i].type = roomType[0];
	}

}

/*********************************************************************
*                 initRoomConnectionCount(Room*)
* @param houseRooms
* @return: void
* Description: Initialize each room to only have 0 connections
**********************************************************************/
void initRoomConnectionCount(struct Room* houseRooms){
	int i;
	for(i = 0; i < 7; ++i) {
		houseRooms[i].outboundConnectionCount = 0; 
	}
}

/*********************************************************************
*                 freeOptions(char**, int)
* @param option
* @param count
* @return: void
* Description: Deallocates memory from char array within the passed
*   structure and the count of items in the structure.
**********************************************************************/
void freeOptions(char **option, int count){
	int i;
	for(i = 0; i < count; ++i){
		free(option[i]);
	}
	free(option);
}

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull(struct Room* houseRooms)  
{
	int i;
	for (i = 0; i < 7; ++i) {
		if (houseRooms[i].outboundConnectionCount < 3)			
			return false; 
	}
   	return true;
}

// Returns true if a connection can be added from Room (< 6 outbound connections), false otherwise
bool CanAddConnectionFrom(struct Room x) 
{
	if (x.outboundConnectionCount < 6) {
		return true;
	}
	else {
		return false; 
	}
}

// Returns true if a connection from Room x to Room y already exists, false otherwise
bool ConnectionAlreadyExists(struct Room x, struct Room y)
{
    int i;

	for (i = 0; i < x.outboundConnectionCount; ++i){
		if (x.outboundRoomConnection[i]->name == y.name)
			return true; 
	}
	
	return false;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room* x, int randomRoomA, int randomRoomB) 
{
    int aCount = x[randomRoomA].outboundConnectionCount;
	int bCount = x[randomRoomB].outboundConnectionCount;
	
	x[randomRoomA].outboundRoomConnection[aCount] = &x[randomRoomB];
	x[randomRoomB].outboundRoomConnection[bCount] = &x[randomRoomA];
	
	x[randomRoomA].outboundConnectionCount++;
	x[randomRoomB].outboundConnectionCount++;
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct Room x, struct Room y) 
{
    if (x.name == y.name)
		return true;
	else
		return false; 
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(struct Room* houseRooms)
{
	struct Room A;  
	int randomRoomA;
	struct Room B;
	int randomRoomB;

    while(true)
   	{
		randomRoomA = rand() % 7;
		A = houseRooms[randomRoomA];
		
        if (CanAddConnectionFrom(A))
		    break;
    }

    do
    {
        randomRoomB = rand() % 7;
		B = houseRooms[randomRoomB];
	
    }  while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);
	
	ConnectRoom(houseRooms, randomRoomA, randomRoomB);  // Adds connection between rooms 
}

void PrintRoomOutboundConnections(struct Room* input)
{
	printf("The rooms connected to (%s/%s) count:%d are:\n", input->name, input->type, input->outboundConnectionCount);

    int i;
    for (i = 0; i < input->outboundConnectionCount; i++)
	    printf("  (%s/%s)\n", input->outboundRoomConnection[i]->name, input->outboundRoomConnection[i]->type);

	printf("\n");
	return;
}

int main() {
	int processID = getpid();   // Obtain process ID number
	char tempID[20];		    // Use temp to convert int to string
	char folderName[80];	    // Folder to hold rooms
	bool startRoomUsed = false; // Used to validate if start room was used
	bool endRoomUsed = false;	// Used to validate if end room was used
	
	// Initialize memory for number of rooms
	struct Room* houseRooms; 
	houseRooms = malloc(7 * sizeof(struct Room));
	
	// Initialize memory for roomNames
	char **roomNames; 		
	roomNames = malloc(10 * sizeof(char*));
	initRoomName(roomNames);
	
	// Initialize the types of rooms
	char **roomType;           
	roomType = malloc(3 * sizeof(char*));
	initRoomType(roomType);

 	initRoomConnectionCount(houseRooms);
	
	// Used to randomize room names and types
	srand(time(NULL));
	
	setRoomName(houseRooms, roomNames);
	
	setRoomType(houseRooms, roomType);

	int count = 0; 
	for (count = 0; count < 7; ++count) {
		printf("Room Name %d: %s\n", count, houseRooms[count].name); 
		printf("Room Type %d: %s\n", count, houseRooms[count].type);
		printf("Room Connection %d: %d\n\n", count, houseRooms[count].outboundConnectionCount);
	}

	// Convert process ID into string and concatenate into folder name
	sprintf(tempID, "%d", processID);
	strcpy(folderName, "castimat.rooms.");
	//	strcat(folderName, tempID);

	int result = mkdir(folderName, 0755);
	printf("Complete: %d\n", result);
//	printf("My process ID : %d\n", processID);

	// Create all connections in graph
	while (IsGraphFull(houseRooms) == false)
	{
		  AddRandomConnection(houseRooms);
	}
	
	for (count = 0; count < 7; ++count)
		PrintRoomOutboundConnections(&houseRooms[count]);

	// Delete folder (Create loop to delete all files within folder first openDir() and closeDir())
	//	rmdir(folderName);
	
	// Free allocated memory
	free(houseRooms);
	freeOptions(roomNames, 10);
	freeOptions(roomType, 3);
	
	return 0;
}
