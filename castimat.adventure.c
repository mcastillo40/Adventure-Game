/*
* CS 261 Assignment 4
* Name: Matthew Castillo
* Date: 10/25/17
* Description: 
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

typedef enum {false, true} bool;

struct Connection {
	char conName[30]; 
};

struct Room {
	char* name; 
    char* type;
    int outboundConnectionCount;
    struct Connection diffConnections[6];
};

// Set the first room as the starting room
struct Room start (struct Room* thisRoom){
	int i;
	for (i = 0; i < 7; i++) {
		if(strncmp(thisRoom[i].type, "START_ROOM", 5) == 0){
			return thisRoom[i];
		}
	}
}

// Set the last room as the end room
struct Room end (struct Room* thisRoom){
	int i;
	for (i = 0; i < 7; i++) {
		if(strncmp(thisRoom[i].type, "END_ROOM", 5) == 0){
			return thisRoom[i];
		}
	}
}

void showConnections(struct Room thisRoom){
	int last = thisRoom.outboundConnectionCount - 1;

	int i;
	for (i = 0; i < thisRoom.outboundConnectionCount; ++i){
		if (i == 0)
			printf("POSSIBLE CONNECTIONS: %s,", thisRoom.diffConnections[i].conName);
		else if (i < last - 1)
			printf(" %s,", thisRoom.diffConnections[i].conName);
		else if (i == last)
			printf(" %s.\n", thisRoom.diffConnections[i].conName);
	}
}

bool validChoice(struct Room thisRoom, char* selection){
	int i;
	printf("select: %s\n", selection);
	for (i = 0; i < thisRoom.outboundConnectionCount; ++i){
		if (strncmp(thisRoom.name, selection, 5) == 0)
			return true;
	}

	return false;
}

void startGame(struct Room* house){
	struct Room currentRoom;
	struct Room endRoom; 
	int moves;
	bool correctRoom;

	size_t characters;
	char *buffer;
    size_t bufsize = 40;
	buffer = malloc(bufsize * sizeof(char*));

	currentRoom = start(house); 
	endRoom = end(house);
	
	do {
		
		printf("CURRENT LOCATION: %s\n", currentRoom.name);
		showConnections(currentRoom);	
		printf("WHERE TO? >");
		characters = getline(&buffer,&bufsize,stdin);
		
		while(!validChoice(currentRoom, buffer)){
			printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
			characters = getline(&buffer,&bufsize,stdin);
		}

		//if (validChoice(currentRoom, buffer))	{
			printf("\nyes\n");
		//}
		//else{
		//	printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
		//}

	} while (strncmp(currentRoom.type, "END_ROOM", 5) != 0);
	
}


int getRowCount(char **info, int counter){
	char * pch;
	int rowCount = 1;
	
	// get row count
	pch=strchr(info[counter],'\n');

	while (pch!=NULL)
	{
		if(pch=strchr(pch+1,'\n'))
			rowCount++;
	}	
	
	return rowCount;
}

void getInformation(char **info, int file, char **roomName, char **roomType, struct Connection* connection, int rows){
	char *name;

	// Used to store the different connections
	char connection1[30];
	char connection2[30];
	char connection3[30];
	char connection4[30];
	char connection5[30];
	char connection6[30];
	
	// Store the last position with type
	int lastType = rows - 1; 
	
	// Token to read each line
	char *temp;
	char *token;
	token = strtok(info[file], "\n");
	
	int i;
	for (i = 0; i < rows; ++i) {
		temp = token;
		
		if (i == 0){
			name = temp;
		}
		else if (i == 1){
			sscanf(temp, "CONNECTION 1:  %[^\n]s", connection1);
		}
		else if (i == 2){
			sscanf(temp, "CONNECTION 2:  %[^\n]s", connection2);
		}
		else if (i == 3){
			sscanf(temp, "CONNECTION 3:  %[^\n]s", connection3);
		}
		else if (i == 4 && i != lastType){
			sscanf(temp, "CONNECTION 4:  %[^\n]s", connection4);
		}
		else if (i == 5 && i != lastType){
			sscanf(temp, "CONNECTION 5:  %[^\n]s", connection5);
		}
		else if (i == 6 && i != lastType){
			sscanf(temp, "CONNECTION 6:  %[^\n]s", connection6);
		}	
		else if (i == lastType){
			sscanf(temp, "ROOM TYPE:  %[^\n]s", roomType[file]);
		}

		token = strtok (NULL, "\n");
	}
		
	// Copy name into room's name
	strcpy(roomName[file], name);
		
	// Set each connection accordingly 
	for (i = 0; i < lastType - 1; ++i){
		if (i == 0)
			strcpy(connection[i].conName, connection1);
		if (i == 1)
			strcpy(connection[i].conName, connection2);
		if (i == 2)
			strcpy(connection[i].conName, connection3);
		if (i == 3)
			strcpy(connection[i].conName, connection4);
		if (i == 4)
			strcpy(connection[i].conName, connection5);
		if (i == 5)
			strcpy(connection[i].conName, connection6);
	}
}

// Allocate memory to a char variable
void initChar(char **type, int count, int size){
	int i;
	for (i = 0; i < count; ++i){
		type[i] = malloc(size * sizeof(char*));
	}
}

void print(struct Room* testRoom){
	printf("NAMESS: %s\n", testRoom[1].name);
}

void freeChar(char **option, int count){
    int i;
	for(i = 0; i < count; ++i){
		free(option[i]);
	}
	free(option);
}

int main(int argc, const char** argv)
{
	int  newestDirTime = -1; // Modified timestamp of newest subdir examined
  	char targetDirPrefix[32] = "castimat.rooms."; // Prefix we're looking for
	char newestDirName[256]; // Holds the name of the newest dir that contains prefix
	memset(newestDirName, '\0', sizeof(newestDirName));
	char tempFileName[256];  // Holds the name of a file in the directory
	int file_descriptor;     // Use to open up file
	char readBuffer[300];
	ssize_t nread;
	int connectionCount;
	
	// Allocate memory for info
	char **info;
	info = malloc(7 * sizeof(char*));
	initChar(info, 7, 400);
	
	// Allocate memory for rooms
	struct Room* houseRooms;
	houseRooms = malloc(7 * sizeof(struct Room));
	
	// Allocate memory for connections
	struct Connection* connectNames;
	connectNames = malloc(6 * sizeof(struct Connection));
	memset(connectNames, '\0', sizeof(connectNames));

	// Allocate memory for temp room names
	char **roomNames;
	roomNames = malloc(7 * sizeof(char*));
	initChar(roomNames, 7, 30);

	// Allocate memory for temp room type
	char **roomTypes;
	roomTypes = malloc(7 * sizeof(char*));
	initChar(roomTypes, 7, 40);
	
	// Allocate memory for temp room connections
	char **connectionNames;
	connectionNames = malloc(6 * sizeof(char*));
	initChar(connectionNames, 6, 30);

	int rowCount;

	DIR* dirToCheck;           // Holds the directory we're starting in
	struct dirent *fileInDir;  // Holds the current subdir of the starting dir
	struct stat dirAttributes; // Holds information we'oomTypeve gained about subdir

	dirToCheck = opendir("."); // Open up the directory this program was run in

	if (dirToCheck > 0)        // Make sure the current directory could be opened
	{
		while ((fileInDir = readdir(dirToCheck)) != NULL)           // Check each entry in dir
		{
			if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If entry has prefix
			{
				stat(fileInDir->d_name, &dirAttributes);            // Get attributes of the entry

				if ((int)dirAttributes.st_mtime > newestDirTime)    // If this time is bigger
				{
					newestDirTime = (int)dirAttributes.st_mtime;
				    memset(newestDirName, '\0', sizeof(newestDirName));
				    strcpy(newestDirName, fileInDir->d_name);
				}
			}
		}

		closedir(dirToCheck); // Close the directory we opened
	}
	
	DIR* latestDir;           // Holds the directory we're starting in

	latestDir = opendir(newestDirName);
	int counter = 0;

	if (latestDir > 0){
		while((fileInDir = readdir(latestDir))){
			
			// Ensure that "." and ".." are not opened
			if (!strcmp (fileInDir->d_name, "."))
				continue;
			if (!strcmp (fileInDir->d_name, ".."))    
				continue;
			
			// Initialize fileName so that it may be opened
			char thisFile[80];
			strcpy(thisFile, newestDirName);
			strcat(thisFile, "/");
			strcat(thisFile, fileInDir->d_name);
			
			file_descriptor = open(thisFile, O_RDONLY);
			
			// If file could not be opened then inform user with error and close program
			if (file_descriptor < 0)
			{
				fprintf(stderr, "Could not open %s\n", fileInDir->d_name);
				exit(1);
			}
			
			// Start reading after "ROOM NAME: "
			lseek(file_descriptor, 11, SEEK_SET);	
			
			memset(readBuffer, '\0', sizeof(readBuffer));
			
			// Copy information from the files into a string
			nread = read(file_descriptor, readBuffer, sizeof(readBuffer));
			strcpy(info[counter], readBuffer);

			// Calculate the number of rows in the file
			rowCount = getRowCount(info, counter);
			
			// Calculate the number of connections in the file
			// Subtract the name and type 
			connectionCount = rowCount - 2;
			houseRooms[counter].outboundConnectionCount = connectionCount;
			
			// Place file info in appropriate strings
			getInformation(info, counter, roomNames, roomTypes, connectNames, rowCount);			
			
			houseRooms[counter].name = roomNames[counter];	
			houseRooms[counter].type = roomTypes[counter];

			int t;
			for (t = 0; t < connectionCount; ++t)
				strcpy(houseRooms[counter].diffConnections[t].conName, connectNames[t].conName);
			
			counter++;
			
			// Close file
			close(file_descriptor);
		}
   }

	startGame(houseRooms);

	closedir(latestDir);

	// Free allocated memory for Room structures
	free(houseRooms);
	free(connectNames);
	freeChar(connectionNames, 6);
	freeChar(info, 7);
	freeChar(roomNames, 7);
	freeChar(roomTypes, 7);

	return 0; 
}
