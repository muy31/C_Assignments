#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITIAL_LENGTH 50

//int rows;
//int cols;

struct point {
	int r;
	int c;
};

struct point * newPoint(int r1, int c1) {
	struct point* nP = malloc(sizeof(struct point));

	nP->r = r1;
	nP->c = c1;
	return nP;
}

struct node {
	char value;
	int neighborLength;
	size_t allocSpace;
	struct point** neighbors;
};

typedef struct node* Node;

Node createNode(char val, struct point** neighbors) {
	Node nd = malloc(sizeof(struct node));

	if (val == 'X' || val == '.') {
		if (neighbors == 0) {
			nd->neighbors = malloc(INITIAL_LENGTH * sizeof(struct point*));
		}
		else {
			nd->neighbors = neighbors;
		}
	}
	else {
		nd->neighbors = 0;
	}

	nd->value = val;
	nd->neighborLength = 0;
	nd->allocSpace = INITIAL_LENGTH;

	return nd;
}

void UpdateEdges(Node ** grid, int r1, int c1, int r2, int c2) {

	if (grid[r1][c1]->value == 'X' || grid[r1][c1]->value == '.') {
		if (grid[r1][c1]->neighborLength + 1 > grid[r1][c1]->allocSpace) {
			grid[r1][c1]->neighbors = realloc(grid[r1][c1]->neighbors, (grid[r1][c1]->neighborLength + 1) * sizeof(struct point*));
			grid[r1][c1]->allocSpace = (grid[r1][c1]->neighborLength + 1);
		}

		grid[r1][c1]->neighbors[grid[r1][c1]->neighborLength] = newPoint(r2, c2);

		grid[r1][c1]->neighborLength++;

	}

	if (grid[r2][c2]->value == 'X' || grid[r2][c2]->value == '.') {
		if (grid[r2][c2]->neighborLength + 1 > grid[r2][c2]->allocSpace) {
			grid[r2][c2]->neighbors = realloc(grid[r2][c2]->neighbors, (grid[r2][c2]->neighborLength + 1) * sizeof(struct point*));
			grid[r2][c2]->allocSpace = (grid[r2][c2]->neighborLength + 1);
		}

		grid[r2][c2]->neighbors[grid[r2][c2]->neighborLength] = newPoint(r1, c1);

		grid[r2][c2]->neighborLength++;
	}
	
}

Node ** nextStep(Node ** grid, int rows, int cols) {

	Node ** nextGrid = malloc(rows * sizeof(Node *));

	for (int r = 0; r < rows; r++) {

		nextGrid[r] = malloc(cols * sizeof(Node));

		for (int c = 0; c < cols; c++) {

			if (grid[r][c]->value != 'X' && grid[r][c]->value != '.') {
				nextGrid[r][c] = createNode(grid[r][c]->value, grid[r][c]->neighbors);
				//free(nextGrid[r][c]->neighbors);
				continue;
			}

			int switchVal = 0;
			for (int index = 0; index < grid[r][c]->neighborLength; index++){
				if (grid[grid[r][c]->neighbors[index]->r][grid[r][c]->neighbors[index]->c]->value == 'X') {
					switchVal++;
				}
				else if (grid[grid[r][c]->neighbors[index]->r][grid[r][c]->neighbors[index]->c]->value == '.') {
					switchVal--;
				}
			}

			if (switchVal > 0) {
				nextGrid[r][c] = createNode('X', grid[r][c]->neighbors);
			}
			else if (switchVal < 0) {
				nextGrid[r][c] = createNode('.', grid[r][c]->neighbors);
			}
			else {
				nextGrid[r][c] = createNode(grid[r][c]->value, grid[r][c]->neighbors);
			}

			//printf("%d %d %p %p\n",r, c, nextGrid[r][c]->neighbors[1], grid[r][c]->neighbors[0]);
			
			//Re-apply characteristics
			nextGrid[r][c]->neighborLength = grid[r][c]->neighborLength;
			nextGrid[r][c]->allocSpace = grid[r][c]->allocSpace;
			//free(nextGrid[r][c]->neighbors);
			nextGrid[r][c]->neighbors = grid[r][c]->neighbors;

			//printf("%p\n", nextGrid[r][c]->neighbors);

		}
	}

	return nextGrid;
}

void printMap(Node** nodes, int rows, int cols) {
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++) {
			printf("%c", nodes[r][c]->value);
		}
		printf("\n");
	}
}

void freeMap(Node** nodes, int rows, int cols) {
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++) {
			free(nodes[r][c]);
		}
		free(nodes[r]);
	}
	free(nodes);
}

void freeMapCompletely(Node** nodes, int rows, int cols) {
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++) {
			if (nodes[r][c]->neighbors != 0) {
				for (size_t i = 0; i < nodes[r][c]->neighborLength; i++) {
					free(nodes[r][c]->neighbors[i]);
				}
				free(nodes[r][c]->neighbors);
			}
			
			free(nodes[r][c]);
		}
		free(nodes[r]);
	}
	free(nodes);
}

int main() {

	int steps;
	int rows;
	int cols;

	scanf("%d%d%d", &steps, &rows, &cols);
	fgetc(stdin);

	Node** grid = malloc(rows * sizeof(Node*));

	//Node grid
	for (size_t r = 0; r < rows; r++) {
		//Initialize row
		grid[r] = malloc((cols) * sizeof(Node));

		//Get strings/char list
		char cl[cols];
		scanf("%[^\n]", cl); fgetc(stdin);

		for (size_t c = 0; c < cols; c++) {
			grid[r][c] = createNode(cl[c], 0);
			//printf("%c ", cl[c]);
		}
		//printf("\n");
	}

	//Edges
	while (1) {
		int r1;
		int c1;
		int r2;
		int c2;

		if (scanf("%d%d%d%d", &r1, &c1, &r2, &c2) < 4){
			//End
			//Start printing steps

			printMap(grid, rows, cols);
			for (int i = 0; i < steps; i++) {
				printf("\n");
				Node ** nextGrid = nextStep(grid, rows, cols);
				freeMap(grid, rows, cols);
				grid = nextGrid;
				printMap(grid, rows, cols);
			}
			
			freeMapCompletely(grid, rows, cols);

			break;
		}
		else {

			UpdateEdges(grid, r1, c1, r2, c2);
			//printf("(%d %d)", grid[r1][c1]->neighbors[0]->r, grid[r1][c1]->neighbors[0]->c);
		}

	}

	return 0;

}

