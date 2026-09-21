#include <stdio.h>
#include <string.h>

typedef struct room {
    char buildingName[100]; // fixed size of 100 characters
    int roomNumber;
    char roomName[100];
} room;

int main()
{
    // Create structure variables
    room room1, room2, room3; // ; finishes a command

    // Assign member values
    strcpy(room1.buildingName, "Newton"); // building name
    room1.roomNumber = 109; // room number
    strcpy(room1.roomName, "iiLAB"); // room name

    strcpy(room2.buildingName, "Newton");
    room2.roomNumber = 118;
    strcpy(room2.roomName, "Control-Lab");

    strcpy(room3.buildingName, "Newton");
    room3.roomNumber = 119;
    strcpy(room3.roomName, "SW-Lab");

    room *roomPt; // Create roomPt as a pointer to room

    roomPt = &room3; // Point roomPt to room3

    // Print each room
    printf("%s building, room %d is %s\n",
           room1.buildingName, room1.roomNumber, room1.roomName);

    printf("%s building, room %d is %s\n",
           room2.buildingName, room2.roomNumber, room2.roomName);

    printf("%s building, room %d is %s\n",
           room3.buildingName, room3.roomNumber, room3.roomName);

    printf("%s building, room %d is %s\n",
           roomPt->buildingName, roomPt->roomNumber, roomPt->roomName);

    printf("room3 address=%p, roomPt = %p\n",
           (void *)&room3, (void *)roomPt);

    return 0;
}