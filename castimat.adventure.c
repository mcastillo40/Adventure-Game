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

struct Room {
	char* name; 
    char* type;
    int outboundConnectionCount;
    struct Room* outboundRoomConnection[6]; 
};

void setRoomName(struct Room* houseRoom, char *roomName, int roomNum){
	houseRoom[roomNum].name = roomName;
}

void setRoomType(struct Room* houseRoom, char *roomType, int roomNum){
	houseRoom[roomNum].type = roomType;
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
	
	rowCount -= 2; 
	
	return rowCount;
}

void cutName(char **info, int file, struct Room thisRoom){

}

void setRoom(struct Room* thisRoom, char **info, int counter){
		char namer[30] = "";
		int ret;
//		char **connections;
//		connections = malloc(rowCount * sizeof(char*));
//		int i;
//		for (i = 0; i < rowCount; ++i){
//			connections[i] = malloc(25 * sizeof(char*));
//		}
		
//		for (i = 0; i < rowCount; ++i)

		ret = sscanf(info[counter] , " %[^\n]s", &namer);
		thisRoom[counter].name = namer;

//		int ret = sscanf(info[0], "%s%*[\n]", &name);
		printf("Name: %s\n", namer);
		
//		int j;
//		for(j = 0; j < rowCount; ++j){
//			free(connections[j]);
//		}
//		free(connections);
}

void print(struct Room* testRoom){
	printf("NAMESS: %s\n", testRoom[1].name);
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

	char **info;
	info = malloc(7 * sizeof(char*));
	int num;
	for(num = 0; num < 7; ++num)
		info[num] = malloc(300 * sizeof(char*));

	struct Room* houseRooms;
	houseRooms = malloc(7 * sizeof(struct Room));
	for (num = 0; num < 7; ++num){
		houseRooms[num].name = "";
		houseRooms[num].type = "";
	}
	
	int rowCount;

	DIR* dirToCheck;           // Holds the directory we're starting in
	struct dirent *fileInDir;  // Holds the current subdir of the starting dir
	struct stat dirAttributes; // Holds information we've gained about subdir

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

			counter++;
			
			// Close file
			close(file_descriptor);
		}
//		printf("name: %s\n", houseRooms[2].name);
   }
	char name[25];
	int ret;
	int count; 
	char *pch;
	for (count = 0; count < 7; count++){
		printf("%s\n", info[count]);
		
		houseRooms[count].outboundConnectionCount = getRowCount(info, count);

		ret = sscanf(info[count] , " %[^\n]s", name);
		houseRooms[count].name = name;
		
		pch=strchr(info[count],':');
		while (pch != NULL) {
			printf ("found at %d\n",pch-info[count]+1);
		    pch=strchr(pch+1,':');
		}

		printf("name: %s\n", houseRooms[count].name);
		int t;
		for (t = 0; t < houseRooms[count].outboundConnectionCount; ++t){
//			printf("num: %d\n", t);
		}

		//cutName(info, count, houseRooms[count]);
		//printf("name: %s\n", houseRooms[count].name);
	}


//	print(houseRooms);
//	printf("name: %s\n", houseRooms[2].name);

	closedir(latestDir);

	// Free allocated memory for Room structures
	free(houseRooms);
	
	int j;
	for(j = 0; j < 7; ++j){
		free(info[j]);
	}
	free(info);

	return 0; 
}
