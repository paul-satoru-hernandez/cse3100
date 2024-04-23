#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//TODO
void lastDigitCount(unsigned *a, unsigned n, unsigned freq[10])
{
	//fill in your code
    int i;
    for (i=0; i<10; i++){
        freq[i] = 0;
    }
    for (i=0; i<n; i++){
        int digit = a[i] % 10;
        switch (digit){
            case 0:
                freq[0]++; break;
            case 1:
                freq[1]++; break;
            case 2:
                freq[2]++; break;
            case 3:
                freq[3]++; break;
            case 4:
                freq[4]++; break;
            case 5:
                freq[5]++; break;
            case 6:
                freq[6]++; break;
            case 7:
                freq[7]++; break;
            case 8:
                freq[8]++; break;
            case 9:
                freq[9]++; break;
        }
    }
}

//Do not change the main function
int main(int argc, char *argv[])
{
	assert(argc == 2);
	unsigned n = atoi(argv[1]);

	assert(n>=1 && n<=10000);
	unsigned a[n], i, s = 0;

	for(i=0; i<n; i++)
	{
		s += 2*i+1;
		a[i] = s;
	}
	unsigned freq[10]; 
	lastDigitCount(a, n, freq);
	for(i=0; i<10; i++)
		printf("%d\t%d\n", i, freq[i]);
	return 0;
}