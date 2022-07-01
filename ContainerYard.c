#include <stdio.h>
#include <conio.h>
#define _CRT_SECURE_NO_WARNINGS

#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3


/*
Uses integers to initialise floor format
*/
void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
	int i, j;

	//Nested for loop works through 2D array to set boundaries and vacant spaces
	for (i = 0; i < NUM_COLS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			floor[j][i] = VACANT;
			if (i == 0 || i == (NUM_COLS - 1) || j == 0 || j == (NUM_ROWS - 1)) {
				floor[j][i] = BOUNDARY;
			}
		}
	}
	//If statements place entry and exit based on function input
	if (entryboundary == 'R') {
		floor[index][NUM_COLS - 1] = ENTRY;
		floor[index][0] = EXIT;
	}
	else if (entryboundary == 'L') {
		floor[index][0] = ENTRY;
		floor[index][NUM_COLS - 1] = EXIT;
	}
	else if (entryboundary == 'T') {
		floor[0][index] = ENTRY;
		floor[NUM_ROWS - 1][index] = EXIT;
	}
	else if (entryboundary == 'B') {
		floor[NUM_ROWS - 1][index] = ENTRY;
		floor[0][index] = EXIT;
	}
}

/*
Replaces integers in initialised floor layout with corresponding characters
*/
void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
	int i, j;
	//Initialises new character array
	char floor2[NUM_ROWS][NUM_COLS];

	//Nested for loop prints out each element of new character array with
	//Characters corresponding to numbers in initial 2D array
	for (i = 0; i < NUM_COLS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			if (floor[j][i] == 0) {
				floor2[j][i] = ' ';
			}
			else if (floor[j][i] == -1) {
				floor2[j][i] = '@';
			}
			else if (floor[j][i] == -2) {
				floor2[j][i] = 'U';
			}
			else if (floor[j][i] == -3) {
				floor2[j][i] = 'X';
			}
			else {
				floor2[j][i] = (floor[j][i] + 64);
			}
		}
	}

	for (i = 0; i < NUM_COLS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			printf("%c", floor2[i][j]);
		}
		printf("\n");
	}
	
}

void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction)
{
	int i, j;

	//Converts position into row and column coordinates
	int ColPos = position % NUM_COLS;
	int RowPos = position / NUM_ROWS;

	int collision = 0;
	int MaxNum = 0;

	//Searches array for highest number which corresponds to the number of the last container added
	for (i = 0; i < NUM_COLS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			if (floor[j][i] > MaxNum) {
				MaxNum = floor[j][i];
			}
		}
	}

	//Proceeds if direction is horizontal
	if (!direction) {
		//Checks for collision
		for (i = ColPos; i < (ColPos + size); i++) {
			if (i < 8) {
				if (floor[RowPos][i] != 0)
				{
					collision = 1;
				}
			}
		}
		//If there's no collision, container is added
		if (!collision)
		{
			for (i = ColPos; i < (ColPos + size); i++){
				if (i < 8) {
					floor[RowPos][i] = MaxNum + 1;
				}
			}
		}
	}
	
	//Proceeds if direction is vertical
	if (direction) {
		//Checks for collision
		for (i = RowPos; i < (RowPos + size); i++) {
			if (floor[i][ColPos] != 0)
			{
				collision = 1;
			}
		}
		//If there's no collision, container is added
		if (!collision)
		{
			for (i = RowPos; i < (RowPos + size); i++)
			{
				floor[i][ColPos] = MaxNum + 1;
			}
		}
	}
}

//Calculates floor area availabe
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width)
{
	double free = 0;
	int i, j;

	//calculates the area of each grid cell
	double cellArea = length * width;

	//Nested for loop works through array elements
	//If cell is empty, cell area is added to total free area
	for (i = 0; i < NUM_COLS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			if (floor[i][j] == 0) {
				free += cellArea;
			}

		}
	}
	return free;
}

int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd)
{
	int i, j;;
	int block1 = 0, block2 = 0;

	//Nested for loop works through 2D array
	for (i = 1; i < NUM_COLS; i++) {
		for (j = 1; j < NUM_ROWS; j++) {

			//Checks if array element is the start of a container
			if (floor[j][i] == (move - 64) && floor[j - 1][i] != (move - 64) && floor[j][i - 1] != (move - 64)) {
				//If strat of a container is found, coordinates are recorded
				*rowStart = j;
				*colStart = i;
				//Determines if container is blocked at the start based on its orientation
				if (floor[j + 1][i] == (move - 64) && floor[j - 1][i] != 0) {
					block1 = 1;
				}
				else if (floor[j][i + 1] == (move - 64) && floor[j][i - 1] != 0) {
					block1 = 1;
				}
			}

			//Checks if array element is the end of a container
			else if (floor[j][i] == (move - 64) && floor[j + 1][i] != (move - 64) && floor[j][i + 1] != (move - 64)) {
				//If end of a container is found, coordinates are recorded
				*rowEnd = j;
				*colEnd = i;
				//Determines if container is blocked at the end based on its orientation
				if (floor[j - 1][i] == (move - 64) && (floor[j + 1][i] != 0)) {
					block2 = 1;
				}
				else if (floor[j][i - 1] == (move - 64) && (floor[j][i + 1] != 0)) {
					block2 = 1;
				}
			}
		}
	}
	//Determines if container is completely bloced or if it can be moved
	return !(block1 && block2);
}

//Moves container where it needs to be
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked)
{
	int i;
	//Calculates container sizes
	int SizeH = c1 - c0 + 1;
	int SizeV = r1 - r0 + 1;

	//If container is blocked, appropriate value is returned
	if ((isBlocked == 0) && (SizeH > 1)) {
		if ((floor[r0][c0 - 1] == EXIT) || (floor[r0][c1 + 1] == EXIT)) {
			return 2;
		}
		else if ((floor[r0][c0 - 1] == ENTRY) || (floor[r0][c1 + 1] == ENTRY)) {
			return 1;
		}
		else {
			return -1;
		}
	}
	else if ((isBlocked == 0) && (SizeV > 1)) {
		if ((floor[r0 - 1][c0] == EXIT) || (floor[r1 + 1][c0] == EXIT)) {
			return 2;
		}

		else if ((floor[r0 - 1][c0] == ENTRY) || (floor[r1 + 1][c0] == ENTRY)) {
			return 1;
		}

		else {
			return -1;
		}
	}

	//Moves horizontal containers if possible and returns appropriate value

	if (SizeH > 1) {
		//Moves container by moving front letter to the back until it hits a non-vacant space
		//Checks if container can be moved left
		if (floor[r0][c0 - 1] == 0) {
			for (i = c1; i > 1; i--) {
				if (floor[r0][i - SizeH] == 0) {
					floor[r0][i - SizeH] = floor[r0][i];
					floor[r0][i] = 0;
				}
				else if (floor[r0][i - SizeH] != 0) {
					if (floor[r0][i - SizeH] == EXIT) {
						return 2;
					}
					else if (floor[r0][i - SizeH] == ENTRY) {
						return 1;
					}
					else {
						return 0;
					}
				}
			}
		}

		//If container can't move left, and is not blocked, it must move right
		else {
			for (i = c0; i < (NUM_COLS - 2); i++) {
				if (floor[r0][i + SizeH] == 0) {
					floor[r0][i + SizeH] = floor[r0][i];
					floor[r0][i] = 0;
				}
				else if (floor[r0][i + SizeH] != 0) {
					if (floor[r0][i + SizeH] == EXIT) {
						return 2;
					}
					else if (floor[r0][i + SizeH] == ENTRY) {
						return 1;
					}
					else {
						return 0;
					}
				}

			}
		}
	}

	//Vertical Containers
	if (SizeV > 1) {
		//Moves container by moving front letter to the back until it hits a non-vacant space
		//Checks if container can be moved up
		if (floor[r0 - 1][c0] == 0) {
			for (i = r1; i > 1; i--) {
				if (floor[i - SizeV][c0] == 0) {
					floor[i - SizeV][c0] = floor[i][c0];
					floor[i][c0] = 0;
				}
				else if (floor[i - SizeV][c0] != 0) {
					if (floor[i - SizeV][c0] == EXIT) {
						return 2;
					}
					else if (floor[i - SizeV][c0] == ENTRY) {
						return 1;
					}
					else {
						return 0;
					}
				}
			}
		}
		//If container can't move up, and is not blocked, it must move down
		else {
			for (i = r0; i < (NUM_ROWS - 2); i++) {
				if (floor[i + SizeV][c0] == 0) {
					floor[i + SizeV][c0] = floor[i][c0];
					floor[i][c0] = 0;
				}
				else if (floor[i + SizeV][c0] != 0) {
					if (floor[i + SizeV][c0] == EXIT) {
						return 2;
					}
					else if (floor[i + SizeV][c0] == ENTRY) {
						return 1;
					}
					else {
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

/* Function to obtain capital letter as input */
char GetMove(void)
{
	char move;
	printf("\nMove container: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

/* The main Container Yard simulation */
int main(void)
{
	int gameOver = 0;
	int isBlocked = 0;
	int floor[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf("............**********************************............\n");
	printf("............* CONTAINER YARD GAME SIMULATION *............\n");
	printf("............**********************************............\n");

	/* Initialise the yard floor grid and add containers */
	InitialiseFloor(floor, 'R', 3);
	AddContainer(floor, 27, 2, 0);
	AddContainer(floor, 9, 2, 0);
	AddContainer(floor, 17, 3, 0);
	AddContainer(floor, 26, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 36, 2, 1);
	AddContainer(floor, 51, 3, 0);
	AddContainer(floor, 13, 3, 1);
	AddContainer(floor, 22, 2, 1);

	/* Print status */
	printf("ENGGEN131 2021 - C Project\nContainer Yard!  The containers are rushing in!\n");
	printf("In fact, %.2f sq ft of the yard floor is available for containers!\n\n", FloorAreaAvailable(floor, 20.5, 10.3));

	/* Main simulation loop */
	while (gameOver != 2) {
		PrintFloor(floor);
		input = GetMove();
		isBlocked = LocateContainer(floor, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveContainer(floor, rowStart, colStart, rowEnd, colEnd, isBlocked);
	}

	/* A container is ready to exit - the simulation is over */
	PrintFloor(floor);
	printf("\nCongratulations, you've succeeded!");
	printf("\n");
	printf("\nPress any key to exit");
	getch();

	return 0;
}
