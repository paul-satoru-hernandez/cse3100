#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n;

	printf("n = ");
	scanf("%d", &n);

	int m = n;
	//TODO
	//add code below
    int total = 0;

    while (n != 1 && n != 4)
    {
        while (n > 0)
        {
            int digit = n % 10;
            total += digit * digit;
            n /= 10;
        }
        n = total;
        total = 0;
        printf("%d\n", n);
    }
	

	if(n==1) printf("%d is a happy number.\n", m);
	else printf("%d is NOT a happy number.\n", m);
	return 0;
}
