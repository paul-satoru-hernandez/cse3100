#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n, i = 0;

	printf("n = ");
	scanf("%d", &n);

	double pi = 0.;
	//TODO
	//add code below 
    
    int e;
    double pow = 1.;
    while (i <= n)
    {
        for (e = 0; e < i; ++e)
        {
            pow *= 16;
        }

        pi += ((4. / ((8. * i) + 1.) - (2. / ((8. * i) + 4.)) - (1. / ((8. * i) + 5.)) - (1. / ((8. * i) + 6.))) * (1. / pow));

        pow = 1.;
        i++;
    }

	printf("PI = %.10f\n", pi);
	return 0;
}
