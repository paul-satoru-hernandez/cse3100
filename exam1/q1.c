#include <stdio.h>

//TODO
int power(int a, int b){
    int pow = 1;
    for (int i = 0; i < b; ++i){
        pow *= a;
    }
    return pow;
}

int digits(int a){
    int i = 0;
    while (a > 0){
        i++;
        a /= 10;
    }
    return i;
}

int palindrome(unsigned long n)
{
    int pal = 0;
    int even = 0;
    int i = (digits(n) - 1) / 2;
    // if (digits(n)==1){
    //     return 0;
    // }

    if (digits(n) % 2 == 0){
        even = 1;
    }
	if (n < 0){
        return 0;
    }
    while (pal <= n){
        pal += (n % 10) * power (10, i);
        i--;
        if (pal == n){
            return 1;
        }
        else if (even == 1){
            n /= 10;
            if (pal == n){
                return 1;
            }
        }else{
            n /= 10;
        }
    }
    return 0;
}

//TODO
unsigned long pCube()
{
    int done = 0;
    unsigned long int p = 1;
    while (done != 1){
        if (palindrome(p)==0 && palindrome(p*p*p)==1){
            done = 1;
        }
        else{
            p++;
        }
    }
    return p;
}
//Do not change the main function
int main()
{
	printf("%lu\n", pCube());
	return 0;
}