/*
 * Tuan Tran
 * 7/17/19
 * buildrooms.c file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for random number
#include <sys/types.h>  // source https://www.systutorials.com/241721/how-to-get-the-running-process-pid-in-c-c/
#include <unistd.h> // https://www.systutorials.com/241721/how-to-get-the-running-process-pid-in-c-c/

// typedef int bool;
// enum { false, true }; // create enum for true and false 

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


// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull()  
{
    int i;

    bool result = false;

    for (i = 0; i < 7; i++) //Loop through all 7 of the room Array
    {

        if ( (roomArray[i].numConnections >= 3) && (roomArray[i].numConnections <= 6) )
        {
            result = true;
        }
        else 
        {
            return false;   // Can return false if the graph is not full
        }
    }

    return result;
}

// Returns a random Room, does NOT validate if connection can be added
struct Room * GetRandomRoom()
{
    int i;
    int number = rand() % 7;
    struct Room * A = &roomArray[number];
    return A;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room * x, struct Room * y) 
{
    //Test Print
    // printf("Room %s is now connected to Room %s\n", x->name, y->name);
    int xConnectionIndex = x->numConnections;

    x->connections[xConnectionIndex] = y;
    x->numConnections++;
    // y->numConnections++;

    //Test Print
    // printf("Room %s connections: %d\n", x->name, x->numConnections);
    // printf("Room %s connections: %d\n", y->name, y->numConnections);
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct Room * x, struct Room * y) 
{
//   ...

    if (strcmp(x->name, y->name) == 0)
    {
        return true;
    }
    // else
    // {
    //     return false;
    // }

     return false;
    
}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
bool CanAddConnectionFrom(struct Room * x) 
{
    if (x->numConnections < 6)   //check if x has < 6 outbound connections
    {
        // printf("Room %s ", x.name);                         //Test Print
        // printf("Num Connections: %d\n", x.numConnections);  //Test Print
        return true;
    }
    else
    {
        return false;
    }
    
    
}
// Returns true if a connection from Room x to Room y already exists, false otherwise
bool ConnectionAlreadyExists(struct Room * x, struct Room * y)
{
    int i; 
    bool result = false;
    for (i = 0; i < x->numConnections; i++)
    {
        if (strcmp(x->connections[i]->name, y->name) == 0)
        {
            return true;
        }
    }
    return false;
}



// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection()  
{
  struct Room * A ;  // Maybe a struct, maybe global arrays of ints
  struct Room * B ;

  while(true)
  {
    A = GetRandomRoom();
    // printf("Random Room A: %s\n", A->name);  //Test Print

    if (CanAddConnectionFrom(A))
      break;
  }

  do
  {
    B = GetRandomRoom();
    // printf("Random Room B: %s\n", B->name); // Test Print
  }
  while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

  ConnectRoom(A, B);  // TODO: Add this connection to the real variables, 
  ConnectRoom(B, A);  //  because this A and B will be destroyed when this function terminates
}



int main ()
{
    int i; 
    int j;

    pid_t pid = getpid();   // get pid

    char command [50];
    char stringPID [32];
    char baseFileName[50];
    sprintf(stringPID, "%d", pid);  //turn pid into string
    strcpy(baseFileName, "trant6.rooms."); 
    strcpy(command, "mkdir trant6.rooms."); // copy mkdir command
    strcat(command, stringPID); //concat command and stringPID
    strcat(baseFileName, stringPID); //concat baseFileName and stringPID

    printf("%s\n", command); // Test Print

    //Commented out temporaroy
    system(command);    //execute mkdir command on system


    for (i=0; i < 7; i++)   //Create 7 files
    {
        char command2[50];
        char num[2];
        sprintf(num, "%d", i);  //turn i into string
        strcpy(command2, "touch ");
        strcat(command2, baseFileName); //concat command2 and trant6.rooms.
        strcat(command2, "/room"); //concat command2 and room
        strcat(command2, num); //concat command and number of for loop
        // printf("Command2: %s\n", command2); // Test Print
         //Commented out temporaroy
        system(command2); //Generate the files inside directory
    }
    
    //Initialize the roomArray with values 
    //Initalize 7 Rooms
    for(i=0; i < 7; i++)
    {
        roomArray[i].numConnections = 0; //initalize number of room connections to 0 1st
        roomArray[i].type = 1;  //Initalize all room type to mid room
    }

    //Assign roomArray names
    // Need to randomize as well
    for(i = 0; i < 7; i++)
    {
        roomArray[i].name = ROOM_NAMES[i];
        // roomArray[i].numConnections = 2;        //test outbound connections
        // printf("Room %d: %s\n", i+1, roomArray[i].name);
        // printf("Room connections: %d\n", roomArray[i].numConnections);
    }

    // Create all connections in graph
    while (IsGraphFull() == false)
    {
        AddRandomConnection();
    }

    strcat(baseFileName, "/room"); //concat baseFileName and stringPID

    FILE *fPtr;
    //Write to file
    for(i = 0; i < 7; i++)
    {
        char num[2];
        char fileName[50];
        strcpy(fileName, baseFileName);
        sprintf(num, "%d", i);  //turn pid into string
        strcat(fileName, num); //concat basefileName and number of for loop
        fPtr = fopen(fileName, "a");

        if (fPtr == NULL)   // source https://codeforwin.org/2018/02/c-program-append-data-file.html#logic
        {
            /* Unable to open file hence exit */
            printf("\nUnable to open '%s' file.\n", fileName);
            printf("Please check whether file exists and you have write privilege.\n");
            exit(EXIT_FAILURE);
        }

        // char textForFile[50];
        // strcpy(textForFile, roomArray[i].name);
        // strcat(textForFile, roomArray[i].name);
        // fputs("ROOM NAME\n", fPtr);
        fprintf(fPtr, "ROOM NAME: %s\n", roomArray[i].name);
        
    }

    //Testing
    // for(i = 0; i < 7; i++)
    // {
    //     printf("Room %d: %s\n", i+1, roomArray[i].name);
    //     printf("Room connections: %d\n", roomArray[i].numConnections);
    // }

    // //Testing Room Connections
    // printf("Room 1: %s: Connections\n", roomArray[0].name);
    // for(i = 0; i < roomArray[0].numConnections; i++)
    // { 
    //     printf("%s\n", roomArray[0].connections[i]->name);
    // }

    // printf("Room 2: %s: Connections\n", roomArray[1].name);
    // for(i = 0; i < roomArray[1].numConnections; i++)
    // { 
    //     printf("%s\n", roomArray[1].connections[i]->name);
    // }
    
    // //Test if Atherton and Belmont Connect
    // if(ConnectionAlreadyExists(&roomArray[0], &roomArray[1]))
    // {
    //     printf("Rooms CONNECTED!\n");
    // }
    /*
    // Testing ConnectRoom

    printf("Connecting Room %s and Room %s\n", roomArray[0].name, roomArray[1].name);
    ConnectRoom(&roomArray[0], &roomArray[1]);
    ConnectRoom(&roomArray[1], &roomArray[0]);
    for(i = 0; i < 7; i++)
    {

        printf("Room %d: %s\n", i+1, roomArray[i].name);
        printf("Room connections: %d\n", roomArray[i].numConnections);
    }
     */


    // if (ConnectionAlreadyExists(a, b))
    //     printf("Connection already exists\n");






    //Testing below

    // struct Room randomRoom = GetRandomRoom();           //Testing GetRandomRoom Function
    // printf("Get random room: %s\n", randomRoom.name);   //Testing CanAddConnectionFrom Function Print
    // CanAddConnectionFrom(randomRoom);                   //Testing CanAddConnectionFrom Function


    /* 
    //Test isGraphFull

    if (IsGraphFull())
    {
        printf("Graph Full\n");
    }
    else
    {
        printf("Graph Not FUll\n");
    }
    */

    return 0;
}