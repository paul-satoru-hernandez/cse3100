#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{

	//TODO
	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3$ as up, right, down and left respectively.

	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.
    int size = (n * 2) + 1;
    double spaces = (size - 2) * (size - 2);
    double count = 0.;
    int posx, posy;
    posx = posy = size-n-1;

    int walk[size][size];
    int i, j;
    for (i=0; i<size; i++)
    {
        for (j=0; j<size; j++)
        {   
            walk[i][j] = 0;
        }
    }
    walk[posx][posy] = 1;

    while (posx != 0 && posx != (n * 2) && posy != 0 && posy != (n * 2))
    {
        int r = rand() % 4;
        if (r == 0) {
            posy += 1;
        }
        if (r == 1) {
            posx += 1;
        }
        if (r == 2) {
            posy -= 1;
        }
        if (r == 3){
            posx -= 1;
        }

        if (posx == 0 || posx == (n * 2) || posy == 0 || posy == (n * 2)) break;
        else walk[posx][posy] = 1;
    }

    for (i=0; i<size; i++)
    {
        for (j=0; j<size; j++)
        {
            if (walk[i][j] == 1) count += 1.;
            //printf("%d", walk[i][j]);
        }
        //printf("\n");
    }
    return count / spaces;
}

//Do not change the code below
int main()
{
	int trials = 1000;

	srand(12345);
	for(int n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(int i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}

