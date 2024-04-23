#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");

    int i=0, sum=0;
    while(i < 200)
    {
        sum += i;
        i += 2;
    }

    printf("%d\n", sum);

    return 0;
}
