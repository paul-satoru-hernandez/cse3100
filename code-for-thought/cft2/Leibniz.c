// In this code for thought problem, we try to find and fix a bug in the following code.

// The following code is trying to calculate a sum that will converge to PI(3.141592....)
// when the number of items goes to infinity.

// This following code is based on the fact that 
// that sum of items 8/((4*i +1)*(4*i +3)) for i = 0, 1, 2, ..., up to infinity is PI.

// Find the bug, think through why the bug occurs.
// Fix the bug and submit your code.

// To know more detail about the sum, google the following 
// Leibniz formula for π

#include <stdio.h>

int main()
{
	float sum;
    // i and n were stored as int's which prevents the final from being a float as we are modifying i and n to get the sum
	float i, n= 10000;
	
	// Calculating the sum using a for loop
	sum = 0;
	for(i=0; i<=n; i++)
	{
		sum += 8/(4*i + 1)/(4*i + 3);
	}
	//print out the sum. 
	//Note how we set the format to only two digits after the decimal point 
	printf("The sum is %.2f\n", sum); 
	return 0;
}
