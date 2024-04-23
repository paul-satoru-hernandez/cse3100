//write an integer to a pipe
void write_int(int pd, int value)
{
	write(pd, &value, sizeof(int));
}
//read an integer from a pipe
//the function returns the number of bytes read
int read_int(int pd, int *value)
{
	return read(pd, value , sizeof(int));
}
//random increment when cannot make 100
int random_inc(int cur)
{
	if(100 - cur > 10) return rand() % 10 + 1;
	else return 100 - cur; 	
}
//optimal increment
int optimal_inc(int cur)
{
	//TODO
	//fill in code below
        //int old = random_inc(cur);

	if(100 - cur <= 10)
		return 100 - cur;
        int old = random_inc(cur);
	return 11 - old;
}

void run_game(int seed)
{
	srand(seed);
        int pd1[2];
        //pipe creation
        if(pipe(pd1) < 0)
        {
                perror("Error.");
                exit(-1);
        }

        int pd2[2];
        //pipe creation
        if(pipe(pd2) < 0)
        {
                perror("Error.");
                exit(-1);
        }
        pid_t pid = fork();
        if(pid == 0)
        {
                //TODO
                //fill in code below
                //note child is the player1
                close(pd1[0]);
                close(pd2[1]);


		write_int(pd1[1], 1);

                int cur;
		//TODO
		//complete the following line of code
                while(read_int(pd2[0] , &cur)!=0)
                {
                        printf("player2: %d\n", cur);
			//TODO
			//fill in code below
                        printf("%d\n",cur);
                        int inc = optimal_inc(cur);
                        cur = cur + inc;
                        write_int(pd1[1], cur);
		}
		//TODO
		//fill in code below
                close(pd1[1]);
                close(pd2[0]);
                exit(0);
        }
	//TODO
	//fill in code below
	//note parent is player2
        close(pd1[1]);
        close(pd2[0]);

        int cur;
        read_int(pd1[0], &cur);
        while(cur < 100)
        {
                printf("player1: %d\n", cur);
		//TODO
		//fill in code below
                //cur += random_inc(cur);
                int inc = random_inc(cur);
                cur = cur + inc;
                write_int(pd2[1], cur);
                read_int(pd1[0], &cur);
        }
        printf("player1: %d\n", cur);
	//TODO
        close(pd1[0]);
        close(pd2[1]);

	//fill in code below
        waitpid(pid, NULL, 0);
}       
