#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N_ELVES 4


typedef struct car_tag_t {
    int parts;
} car_t;

typedef struct workshop_t_tag {
    //pthread_mutex_t mutex;
    pthread_barrier_t barrier;
    car_t* table[N_ELVES];
} workshop_t;

typedef struct elf_arg_t_tag {
    // is this needed?
    workshop_t* workshop;
    int id;
} elf_arg_t;

void my_msleep(int r)
{
    // limit to 100ms
    struct timespec req = {0, (r % 100 + 1) * 1000000};

    nanosleep(&req, NULL);
}

void random_sleep() {
    unsigned int state = time(NULL) ^ getpid() ^ pthread_self();
    my_msleep(rand_r(&state));
}

car_t* grab_car(int place) {   
    random_sleep();
    car_t* car = malloc(sizeof(car_t));
    car->parts = 0;
    printf("Placing a car in spot %d\n", place);
    return car;
}

void add_part(car_t* car, int place) {
    random_sleep();
    printf("Added a part to the car in spot %d (%d->%d)\n", place, car->parts, car->parts+1);
    car->parts++;
}

void free_car(car_t* car, int place) {
    random_sleep();
    free(car);
    printf("Freed the car in spot %d\n", place);
}

void* elf_function(void* arg) {
    /*
     * TODO
     * The elves sit in a circle and take the following steps:
     * First, each elf grabs a car and places it in the spot corresponding to its id 
     * Next, each elf moves to the next spot around the table. 0->1->2->3->0->...
     * Once the elves have moved, each elf adds a part to the car
     * After all the elves complete this task, they move once again (this must be synchronized!)
     * Eventually, every car will have the necessary number of parts and the elves should send them to santa! (Free them)
     * Afterwords, the elves will stop working
     * Please see the slides in HuskyCT for more info!
     */
    elf_arg_t* elf_arg = arg;
    workshop_t* workshop = elf_arg->workshop;
    int id = elf_arg->id;
    int cur_car = id;

    // TODO
    workshop->table[cur_car] = grab_car(cur_car);
    pthread_barrier_wait(&workshop->barrier);

    while(1) {
        cur_car = (cur_car + 1) % N_ELVES;
        
        if(workshop->table[cur_car]->parts >= N_ELVES) {
            free_car(workshop->table[cur_car], cur_car);
            return NULL;
        }

        add_part(workshop->table[cur_car], cur_car);

        printf("Elf %d is waiting...\n", id);
        pthread_barrier_wait(&workshop->barrier);
    }
    
    
    return NULL;
}

int main() 
{
    workshop_t workshop;
    pthread_t elf_threads[N_ELVES];
    elf_arg_t elf_args[N_ELVES];
    
    // TODO
    // Initialize workshop fields
    // Set up threads and thread arguments
    // Create threads
    // Join threads
    pthread_barrier_init(&workshop.barrier, NULL, N_ELVES);
    for (int i = 0; i < N_ELVES; i++) {
        workshop.table[i] = NULL;
        elf_args[i].id = i;
        elf_args[i].workshop = &workshop;
    }
    for (int i = 0; i < N_ELVES; i++) {
        pthread_create(&elf_threads[i], NULL, elf_function, &elf_args[i]);
    }
    for (int i = 0; i < N_ELVES; i++) {
        pthread_join(elf_threads[i], NULL);
    }
    pthread_barrier_destroy(&workshop.barrier);
    return 0;
}