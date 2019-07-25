/*
 * Tuan Tran
 * 7/17/19
 * adventure.c file
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for random number
#include <sys/types.h>  // source https://www.systutorials.com/241721/how-to-get-the-running-process-pid-in-c-c/
#include <unistd.h> // https://www.systutorials.com/241721/how-to-get-the-running-process-pid-in-c-c/
#include <sys/stat.h> //For stat
#include <dirent.h> // source https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/


//Source: https://stackoverflow.com/questions/1921539/using-boolean-values-in-c
typedef int bool;
#define true 1
#define false 0

enum ROOMTYPE
{
    START_ROOM, 
    MID_ROOM,
    END_ROOM
};

struct Room {
    char * name;            //Room Name; Max 8 char long; only upper and lowercase allowed
    enum ROOMTYPE type;     //Room Type: START_ROOM, END_ROOM, MID_ROOM
    struct Room * connections[6]; // Must have at least 3 outbound and at most 6 outbound
    int numConnections;     //Count of Number of Room Connections
};

//Global Variables
struct Room roomArray[7];       //Array to hold 7 out of the 10 rooms
char newDirName[256];
int gameOver = 0; //If you find the end_room then game is over and gameOver == 1 
int startRoom; 
int currentRoom;
int endRoom;

//Hard Coded Room Names
char * ROOM_NAMES[10] = 
{
    // 10 names bases of cities in the Bay Area
    "Atherton",
    "Belmont", 
    "Berkley",
    "Fremont", 
    "Menlo", 
    "PaloAlto", 
    "Redwood", 
    "SanFran",
    "SanJose", 
    "SanMateo"
};

//Return a the room name
char getRoomName(char * fname)
{
    char result[50];
    char fileName[50];
    char r[2];
    FILE * fPtr;
    strcpy(fileName, fname);
    fPtr = fopen(fileName, "r");  //source https://www.programiz.com/c-programming/examples/read-file

    // fgets(roomName, 10, fPtr);
    fscanf(fPtr, "%s %s %s", r, r ,result);

    fclose(fPtr);

    return * result;
}

void getDirectory() 
{
    int newestDirTime = -1; // Modified timestamp of newest subdir examined
    char targetDirPrefix[32] = "trant6.rooms."; // Prefix we're looking for
    char newestDirName[256]; // Holds the name of the newest dir that contains prefix
    memset(newestDirName, '\0', sizeof(newestDirName));
    DIR* dirToCheck; // Holds the directory we're starting in
    struct dirent *fileInDir; // Holds the current subdir of the starting dir
    struct stat dirAttributes; // Holds information we've gained about subdir

    dirToCheck = opendir("."); // Open up the directory this program was run in

    if (dirToCheck > 0) // Make sure the current directory could be opened
    {
        while ((fileInDir = readdir(dirToCheck)) != NULL) // Check each entry in dir
        {
            if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If entry has prefix
            {
                // printf("Found the prefex: %s\n", fileInDir->d_name);
                stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

                if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
                {
                newestDirTime = (int)dirAttributes.st_mtime;
                memset(newestDirName, '\0', sizeof(newestDirName));
                strcpy(newestDirName, fileInDir->d_name);
                // printf("Newer subdir: %s, new time: %d\n",
                        // fileInDir->d_name, newestDirTime);
                }
             }
        }
    }

    closedir(dirToCheck); // Close the directory we opened
    strcpy(newDirName, newestDirName);
    return;
}

void interface(int currentRoom)
{
    printf("CURRENT LOCATION: %s\n", roomArray[currentRoom].name);
    printf("POSSIBLE CONNECTIONS: ");
}

int main ()
{

    getDirectory();         //Finds new Directory
    // printf("%s\n", newDirName); //Test Print of newDirName

    //Grab all the data from the room files

    //Get all the room names
    int i;
    char * fileName;
    char * result;
    char roomNum[2];
    size_t bufsize = 32;
    fileName = (char *)malloc(32 * sizeof(char));
    result = (char *)malloc(32 * sizeof(char));

    strcpy(fileName, newDirName);
    strcat(fileName, "/room");

    for(i=0; i < 7; i++)
    {
        FILE * fPtr;        //file pointer
        char temp[20];      //temp var to hold room name
        char * tempRN;
        strcpy(temp, fileName);     //copy base filename 
        sprintf(roomNum, "%d", i);  //turn i into string
        strcat(temp, roomNum);      //concat roomnumber to temp var holding filename
        fPtr = fopen(temp, "r");  //read from file
        fgets(result, 20 , fPtr);  //get from first line
        fclose(fPtr);
        tempRN = strtok(result, "\n");  //Take off any newlines

        tempRN = strndup(result+11, 10);    // cut off the string to get room name; source https://www.linuxquestions.org/questions/programming-9/extract-substring-from-string-in-c-432620/
        
        roomArray[i].name = tempRN;
        // printf("%s\n", tempRN);  //test print
    }

    //Get Room Connections
    int count = 0;
    int j;
    for(i=0; i < 7; i++)
    {
        FILE * fPtr;        //file pointer
        
        char temp[20];      //temp var to hold room connections
        char * tempRN;
        char line[256];
        strcpy(temp, fileName);     //copy base filename 
        sprintf(roomNum, "%d", i);  //turn i into string
        strcat(temp, roomNum);      //concat roomnumber to temp var holding filename
        fPtr = fopen(temp, "r");  //read from file

        if (fPtr == 0)
        { 
            while (fgets(line, sizeof(line), fPtr) && (strncmp(line, "ROOM TYPE:", 10) != 0))
            {

            }
            count++;
        }


        fclose(fPtr);
        // printf("count: %d\n", count);  //test print
        // printf("%s\n", result);  //test print
        
    }

     for(i=0; i < 7; i++)
     {
        //  printf("Rooms: %s\n", roomArray[i].name);
     }

    currentRoom = 0;
    interface(currentRoom);
    return 0;
}


