#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define SIZE_X (30 * 2 + 1)
#define SIZE_Y (20 * 2 + 1)

// right, left, down, up    one row/col
const int translations[] = {1, -1, SIZE_X, -SIZE_X};

void display_maze(int maze[SIZE_Y][SIZE_X]);
void clear_screen_and_delay(int mills);
void create_maze(int maze[SIZE_Y][SIZE_X], int algorithm, int animate);

void dfs_recursive(int maze[SIZE_Y][SIZE_X], int currentSquare, int animate);

int check_square(int maze[SIZE_Y][SIZE_X], int currentSquare, int translation);
int check_neighbors(int maze[SIZE_Y][SIZE_X], int currentSquare);
void fill_square(int maze[SIZE_Y][SIZE_X], int currentSquare);
int get_square(int maze[SIZE_Y][SIZE_X], int currentSquare);
int choose_dir(float weights[4]);


int main(int argc, char *argv[]) {

    int maze[SIZE_Y][SIZE_X];

    srand(time(NULL));

	create_maze(maze, 1, 0);

    display_maze(maze);

    return 0;
}

void dfs_recursive(int maze[SIZE_Y][SIZE_X], int currentSquare, int animate) {    

	if (animate) {
		display_maze(maze);
		clear_screen_and_delay(50);
	}
    // checks for non-vistited neighbor
    // if it doesn't find one, then it backtracks until it does
    if (!check_neighbors(maze, currentSquare)) return;

    while (check_neighbors(maze, currentSquare)) {

		// right, left, up, down
		// adds up to 1
		float weights[4] = {0.25, 0.25, 0.25, 0.25};

        int dir = choose_dir(weights);

        int trans = translations[dir];
		
		if (!check_square(maze, currentSquare, trans*2)) continue;
        
		fill_square(maze, currentSquare+trans);
	   	fill_square(maze, currentSquare+trans*2);

        dfs_recursive(maze, currentSquare+trans*2, animate);
    }
}


// Returns 0 if the current square has a unvisited neighbor
int check_neighbors(int maze[SIZE_Y][SIZE_X], int currentSquare) {
	for (int i = 0; i < 4; i++) {
        int translation = translations[i]*2;
        if (check_square(maze, currentSquare, translation)) return 1;
    }
    return 0;
}

// returns 1 if not visted, 0 if visited
int check_square(int maze[SIZE_Y][SIZE_X], int currentSquare, int translation) {
    // God is dead. God remains dead. And we have killed him.
    // How shall we comfort ourselves, the murderers of all murderers?
    // What was holiest and mightiest of all that the world has yet owned has bled to death under our knives:
    // who will wipe this blood off us? 
    // What water is there for us to clean ourselves?
    // What festivals of atonement, what sacred games shall we have to invent?
    // Is not the greatness of this deed too great for us?
    // Must we ourselves not become gods simply to appear worthy of it?
    //
    // In other words, I apologize to whoever must read this code
    return ((currentSquare % SIZE_X + translation % SIZE_X) > -1 && 
            (currentSquare % SIZE_X + translation % SIZE_X) < SIZE_X &&
            (currentSquare / SIZE_X + translation / SIZE_X) > -1 &&
            (currentSquare / SIZE_X + translation / SIZE_X) < SIZE_Y &&
            get_square(maze, currentSquare+translation));
}

void fill_square(int maze[SIZE_Y][SIZE_X], int currentSquare) {
    maze[currentSquare / SIZE_X][currentSquare % SIZE_X] = 0;
}

int get_square(int maze[SIZE_Y][SIZE_X], int currentSquare) {
    return maze[currentSquare / SIZE_X][currentSquare % SIZE_X];
}

int choose_dir(float weights[4]) {

	float choice = (float)(rand() % 100);

	if (choice < weights[0]*100) return 0;
	if (choice < (weights[0]+weights[1])*100) return 1;
	if (choice < (weights[0]+weights[1]+weights[2])*100) return 2;
	if (choice < (weights[0]+weights[1]+weights[2]+weights[3])*100) return 3;
}


void display_maze(int maze[SIZE_Y][SIZE_X]) {
	for (int i = 0; i < SIZE_Y; i++)
	{
		for (int j = 0; j < SIZE_X; j++) 
		{
			if (maze[i][j])
			{
				// The solid block unicode string
				printf("\u2588\u2588");
			} else {
				printf("  ");
			} 
		}
		printf("\n");
	}
}

// takes in 2d array and creates maze based on algorithm
void create_maze(int maze[SIZE_Y][SIZE_X], int algorithm, int animate) {

    // Fills the maze with walls
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            maze[i][j] = 1;
        }        
    }

    // Gets random square on left side of maze
    int start = (rand() % (SIZE_Y / 2))*2 + 1;
    start *= SIZE_X; start++;


	switch (algorithm)
	{
		case 1:
			dfs_recursive(maze,start, animate);
			break;
		default:
			printf("Not a valid choice\n");
	}
}

void clear_screen_and_delay(int mills) {
	// ANSI escape code to clear screen
	printf("\e[1;1H\e[2J");
	
	// This is technically deprecated but it works so I'm keeping it in
	usleep(mills*1000);
}
