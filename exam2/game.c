#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include "run-game.c"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s seed\n", argv[0]);
		return 0;
	}
	int seed = atoi(argv[1]);
	assert(seed>=1 && seed <=100000);
	run_game(seed);
	return 0;
}
