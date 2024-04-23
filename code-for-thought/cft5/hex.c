//In this assignment, we write code to convert decimal integers into hexadecimal numbers
//We pratice using arrays in this assignment
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//convert the decimal integer d to hexadecimal, the result is stored in hex[]
void dec_hex(int d, char hex[])
{
	char digits[] ={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
					'C', 'D', 'E', 'F'};

	int k = 0;
	//Fill in your code below
	//It should not be hard to obtain the last digit of a hex number
	//Then what?
	//If we are getting the digits in a reverse order, what should we do in the end?
    while (d > 0)
    {
        int digit = d % 16;
        hex[k] = digits[digit];
        d /= 16;
        k++;
    }
    int i;
    for (i=0; i<k/2; i++)
    {
        int temp;
        temp = hex[i];
        hex[i] = hex[k-1-i];
        hex[k-1-i] = temp;
    }
	//Make sure the last character is a zero so that we can print the string correctly
	hex[k] = '\0';
}

// Do not change the code below
int main()
{
	int d;
	char hex[80];
	
	printf("Enter a positive integer : ");
	scanf("%d", &d); 
	dec_hex(d, hex);
	printf("%s\n", hex);
	return 0;
}
