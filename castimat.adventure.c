/*
* CS 344 Assignment 
* Name: Matthew Castillo
* Date: 10/25/17
* Description: Activate the Adventure Game where the user
* selects which room to go into until they find the exit.
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
#include <pthread.h>

#define NUM_THREAD	2

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

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_lock(myMutex);

// Create file to hold the time
char* createTime(){
	ssize_t nwritten, nread;
	char timeFile[] = "currentTime.txt";
	int file_descriptor;
	char buffer[255];
	time_t currentTime;
	struct tm *localTime1;
	char newBuffer[50];
	
	currentTime = time (NULL);
	
	// get local time
	localTime1 = localtime (&currentTime);
	
	// write time in format: 1:03pm, Tuesday, September 13, 2016
	strftime (buffer, 255, "%I:%M %p, %A, %B %d, %Y\n", localTime1);
	
	file_descriptor = open(timeFile, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if (file_descriptor == -1)	{
		printf("Hull breach - open() failed on \"%s\"\n", timeFile);
		perror("In main()");
		exit(1);
	}

	nwritten = write(file_descriptor, buffer, strlen(buffer) * sizeof(char));
	
	memset(newBuffer, '\0', sizeof(newBuffer));
	lseek(file_descriptor, 0, SEEK_SET);
	nread = read(file_descriptor, newBuffer, sizeof(newBuffer));

	printf("\n%s\n", newBuffer);

}



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

// Return the room that the user selected to enter
struct Room newRoom (struct Room* thisRoom, char* selection){
	int i;
	for (i = 0; i < 7; i++) {
		if(strncmp(thisRoom[i].name, selection, 5) == 0){
			return thisRoom[i];
		}
	}
}

// Show the other rooms that the user is able to enter from the room they are in
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

// Return true if the name of the room is equal to what the user enetered 
bool validChoice(struct Room thisRoom, char* selection){
	int i;
	
	for (i = 0; i < thisRoom.outboundConnectionCount; ++i){
		if (strncmp(thisRoom.diffConnections[i].conName , selection, 5) == 0){
			return true;
		}
	}
	return false;
}

// Return the time if the user choose time else return that the user 
// entered the wrong information
void isTime(char *selection){
	printf("THIS is selection: %s\n",selection);	
	
	

	if(strncmp(selection, "time", 5)){
		createTime();
	}
	else{
		printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
	}
 
}

// Begin the game and continue until the user find the end of the roomnwritten = write(file_descriptor, roomInfo[i], strlen(roomInfo[i]) * sizeof(char));
// Will keep track of valid moves and if user would like to know the time
void startGame(struct Room* house){
	struct Room currentRoom;
	struct Room endRoom; 
	int moves = 0;
	bool correctRoom = false;
	char pathToVictory[1000];
	strcpy(pathToVictory, "");
	size_t characters;
	char *buffer;
    size_t bufsize = 40;
	buffer = malloc(bufsize * sizeof(char*));

	currentRoom = start(house); 
	endRoom = end(house);
	
	// Loop through the game until the user finds the exit
	do {
			// Loop through question if the user does not enter a 
			// correct response or if they ask for the time
		do {
			
			// Display current position and room choices
			printf("CURRENT LOCATION: %s\n", currentRoom.name);
			showConnections(currentRoom);	
			printf("WHERE TO? >");
			characters = getline(&buffer,&bufsize,stdin);
			
			correctRoom = validChoice(currentRoom, buffer);
			
			if (!correctRoom)
				isTime(buffer);

		} while(!correctRoom);
	    	
		moves += 1; // Only increment move if correct room was chosen

		printf("\n");

		// Set new room to the room that was chosen
		currentRoom = newRoom(house, buffer);
		
		// Keep track of the path that the user has chosen
		strcat(pathToVictory, currentRoom.name);
		strcat(pathToVictory, "\n");

	} while (strncmp(currentRoom.type, "END_ROOM", 5) != 0);

	// Inform user that they finished the game
	// display the number of moves and their path
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", moves);
	printf("%s", pathToVictory);

	free(buffer);
}

// Returns the number of row that are in the file
int getRowCount(char **info, int counter){
	char * sample;
	int rowCount = 1;
	
	// get row count
	sample=strchr(info[counter],'\n');

	while (sample != NULL)	{
		if(sample = strchr(sample + 1,'\n'))
			rowCount++;
	}	
	
	return rowCount;
}

// Get the necessary information for the room from within the file
// Search through each row and get the info
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
	
	// Token to read each linenwritten = write(file_descriptor, roomInfo[i], strlen(roomInfo[i]) * sizeof(char));
	char *temp;
	char *token;
	token = strtok(info[file], "\n");
	
	// Go through the number of row that are in the file
	// select the valid informationprintf("\nHUH? I DON'T UNDEnwritten = write(file_descriptor, roomInfo[i], strlen(roomInfo[i]) * sizeof(char));RSTAND THAT ROOM. TRY AGAIN.\n\n");
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

// Free memory from char
void freeChar(char **option, int count){
    int i;
	for(i = 0; i < count; ++i){
		free(option[i]);
	}
	free(option);
}

int main()
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
	
	// Allocate memory to hold information from read file  
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
			
			// Set the rooms name and type
			houseRooms[counter].name = roomNames[counter];	
			houseRooms[counter].type = roomTypes[counter];
			
			// Set the rooms connections
			int t;
			for (t = 0; t < connectionCount; ++t)
				strcpy(houseRooms[counter].diffConnections[t].conName, connectNames[t].conName);
			
			counter++;
		
			
			// Close file
			close(file_descriptor);
		}
   } 
   
 	// Start the game
		startGame(houseRooms);
	
	// Close the directory
	closedir(latestDir);

	// Free allocated memory 
	free(houseRooms);
	free(connectNames);
	freeChar(connectionNames, 6);
	freeChar(info, 7);
	freeChar(roomNames, 7);
	freeChar(roomTypes, 7);

	return 0; 
}
