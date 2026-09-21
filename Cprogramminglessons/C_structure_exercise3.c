#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int x; // integral number x y and z
	int y;
	int z;
} POSITION_TypeDef;

// void: function does not return to the start
void addPos(POSITION_TypeDef pos0, POSITION_TypeDef pos1, POSITION_TypeDef* posOut);
void subDist(POSITION_TypeDef pos0, POSITION_TypeDef pos1, POSITION_TypeDef* posOut);
void printPos(POSITION_TypeDef Pos);
// each pos0, pos1, posout contains the POSITION_Typedef x y and z

int main()
{
	// Exercise 3 but the result says exercise 2..?
	printf("\nExercise 2\n");
	POSITION_TypeDef Pos[2];
	POSITION_TypeDef PosOut = { 0, }; // all posout xyz become 0

	Pos[0].x = 2; Pos[0].y = 2; Pos[0].z = 2;
	Pos[1].x = 5; Pos[1].y = 5; Pos[1].z = 5; // pos1 and pos2 values

	printf("Result output of 'addPos'\n");
	addPos(Pos[0], Pos[1], &PosOut);
	printPos(PosOut);

	printf("Result output of 'getDist'\n");
	subDist(Pos[0], Pos[1], &PosOut);
	printPos(PosOut); // get the addpos and getdist 

	return 0;
}

void addPos(POSITION_TypeDef pos0, POSITION_TypeDef pos1, POSITION_TypeDef* posOut)
{
	posOut->x = pos0.x + pos1.x;
	posOut->y = pos0.y + pos1.y;
	posOut->z = pos0.z + pos1.z; // add them all together
}

void subDist(POSITION_TypeDef pos0, POSITION_TypeDef pos1, POSITION_TypeDef* posOut)
{
	posOut->x = abs(pos0.x - pos1.x);
	posOut->y = abs(pos0.y - pos1.y);
	posOut->z = abs(pos0.z - pos1.z); // adding
}

void printPos(POSITION_TypeDef Pos)
{
	printf("PosOut result : (x, y, z) = (%d, %d, %d)\n",
		Pos.x, Pos.y, Pos.z);
}