#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

//TODO
//Implement the below function
//Simulate one particle moving n steps in random directions
//Use a random number generator to decide which way to go at every step
//When the particle stops at a final location, use the memory pointed to by grid to 
//record the number of particles that stop at this final location
//Feel free to declare, implement and use other functions when needed

void one_particle(int ***grid, int n)
{
    int size = (n * 2) + 1;
    int x, y, z;
    x = y = z = size / 2;
    int i;
    for (i=1; i<=n; i++){
        int r = rand() % 6;
        switch (r){
            case 0:
                x--; break;
            case 1:
                x++; break;
            case 2:
                y++; break;
            case 3:
                y--; break;
            case 4:
                z++; break;
            case 5:
                z--; break;
        }
    }
    grid[x][y][z] += 1;
}

//TODO
//Implement the following function
//This function returns the fraction of particles that lie within the distance
//r*n from the origin (including particles exactly r*n away)
//The distance used here is Euclidean distance
//Note: you will not have access to math.h when submitting on Mimir
double euclid(int x, int y, int z)
{
    int i;
    double s = (x * x) + (y * y) + (z * z);
    double guess = s / 2.;
    if (s == 0){
        return 0;
    }
    for (i=0; i<15; i++){
        guess = (guess + (s/guess)) / 2.;
    }
    return guess;
}
double density(int ***grid, int n, double r)
{
    int i, j, k;
    int x, y, z;
    //double rn = r * n;
    double count, total;
    count = total = 0.;
    int size = (n * 2) + 1;
    x = y = z = size / 2;
    for (i=0; i<size; i++){
        for (j=0; j<size; j++){
            for (k=0; k<size; k++){
                double distance = euclid(i - x, j - y, k - z);
                if (distance < (r * n) || (distance - (r * (double)n) < .000000000000001 && distance - (r * (double)n) > -.000000000000001)){
                    count += grid[i][j][k];
                }
                if (distance < 1. + n){
                    total += grid[i][j][k];
                }
            }
        }
    }
    //printf("\n%f -- %f\n", count, total);
    return count / total;
}


//use this function to print results
void print_result(int ***grid, int n)
{
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
    {
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
    }
}

//TODO
//Finish the following function
//See the assignment decription on Piazza for more details
void diffusion(int n, int m)
{
	//fill in a few line of code below
    int ***grid;
    int size = (n * 2) + 1;
    int i, j;

    grid = (int***)calloc(size, sizeof(int**));
    for(i=0; i<size; i++){
        grid[i] = (int**)calloc(size, sizeof(int*));
        for (j=0; j<size; j++){
            grid[i][j] = (int*)calloc(size, sizeof(int));
        }
    }

	for(int i = 1; i<=m; i++) one_particle(grid, n);

	print_result(grid, n);
	//fill in some code below
    for (i=0; i<size; i++){
        for (j=0; j<size; j++){
            free(grid[i][j]);
        }
        free(grid[i]);
    }
    free(grid);


}

int main(int argc, char *argv[])
{
	
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	assert(n >= 1 && n <=50);
	assert(m >= 1 && m <= 1000000);
	srand(12345);
	diffusion(n, m);
	return 0;
}
