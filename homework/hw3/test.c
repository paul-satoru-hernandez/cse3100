#include <stdio.h>
#include <stdlib.h>

int test(int ***grid, int n){
	int i, j, k;
	int size = (n * 2) + 1;
	int count = 0;
	for (i=0; i<size; i++){
		for (j=0; j<size; j++){
			for (k=0; k<size; k++){
				if (grid[i][j][k] == 0){
					count++;
				}			
			}
		}
	}
	return count;
}


int main(void)
{
	int n = 2;
	int size = (n * 2) + 1;
	int i, j;
	
	int*** grid = (int***)calloc(size, sizeof(int**));
	for (i=0; i < size; i++){
		grid[i] = (int**)calloc(size, sizeof(int*));
		for (j=0; j<size; j++){
			grid[i][j] = (int*)calloc(size, sizeof(int));
		}
	}

	int count = test(grid, n);

	printf("%d \n", count);


	for (i=0; i<size; i++){
		for (j=0; j<size; j++){
			free(grid[i][j]);
		}
		free(grid[i]);
	}
	free(grid);
}

