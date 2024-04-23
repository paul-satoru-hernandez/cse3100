// Only this line of comment is provided 
#include <stdio.h>

int main(void)
{
    double input = 0, average = 0, count = 0, total = 0;
    while (scanf("%lf", &input) == 1)
    {
        count += 1;
        total += input;
        average = total / count;
        printf("Total=%f Average=%f\n", total, average);
    }

    return 0;
}