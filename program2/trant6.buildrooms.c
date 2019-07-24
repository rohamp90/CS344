/*
 * Tuan Tran
 * 7/17/19
 * buildrooms.c file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char * name;    //Room Name; Max 8 char long; only upper and lowercase allowed
    enum ROOMTYPE type;    //Room Type: START_ROOM, END_ROOM, MID_ROOM
    struct Room * connections[6]; // Must have at least 3 outbound and at most 6 outbound
    int numConnections;
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

    for (i = 0; i < 7; i++)
    {
        if ( (roomArray[i].numConnections >= 3) && (roomArray[i].numConnections <= 6) )
        {
            result = true;
        }
        else 
        {
            result = false;
        }
    }

    return result;
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection()  
{
  struct Room A;  // Maybe a struct, maybe global arrays of ints
  struct Room B;

//   while(true)
//   {
//     A = GetRandomRoom();

//     if (CanAddConnectionFrom(A) == true)
//       break;
//   }

//   do
//   {
//     B = GetRandomRoom();
//   }
//   while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

//   ConnectRoom(A, B);  // TODO: Add this connection to the real variables, 
//   ConnectRoom(B, A);  //  because this A and B will be destroyed when this function terminates
}

// Returns a random Room, does NOT validate if connection can be added
struct Room GetRandomRoom()
{
//   ...
    struct Room A;

    return A;
}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
bool CanAddConnectionFrom(struct Room x) 
{
//   ...

    return true;
}
// Returns true if a connection from Room x to Room y already exists, false otherwise
bool ConnectionAlreadyExists(struct Room x, struct Room y)
{
//   ...

    return true;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room x, struct Room y) 
{
//   ...
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct Room x, struct Room y) 
{
//   ...

    if (strcmp(x.name, y.name) == 1)
    {
        return true;
    }
    // else
    // {
    //     return false;
    // }

     return false;
    
}

int main ()
{



    // Create all connections in graph
    // while (IsGraphFull() == false)
    // {
    //     AddRandomConnection();
    // }

    //Initialize the roomArray with values 
    int i; 
    int j;

    //Initalize 7 Rooms
    for(i=0; i < 7; i++)
    {
        roomArray[i].numConnections = 0; //initalize number of room connections to 0 1st
    }

    //Assign roomArray names
    for(i = 0; i < 7; i++)
    {
        roomArray[i].name = ROOM_NAMES[i];
        printf("Room name: %s\n", roomArray[i].name);
    }


    // if (ConnectionAlreadyExists(a, b))
    //     printf("Connection already exists\n");



    /* 
    //Test isGraphFull

    if (IsGraphFull())
    {
        printf("True\n");
    }
    else
    {
        printf("False\n");
    }
    */

    return 0;
}