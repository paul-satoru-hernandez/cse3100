#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// search TODO to find the place you need to work on

/*************  macros related to random numbers for MT ****************/
// Use functions in POSIX.1-2001.

// Use the following macro in your thread function before any calls to get random values.
#define RANDOM_INIT(x) (g_random_buffer[0] = 3100, g_random_buffer[1] = (x), g_random_buffer[2] = (x) >> 16)

// RANDOM_INT() returns a random number uniformly distributed between [0, 2^31)
#define RANDOM_INT() (nrand48(g_random_buffer))

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}

void check_pthread_return(int rv, char *msg)
{
    if (rv == 0)
        return;
    errno = rv;
    die(msg);
}

void my_msleep(int r)
{
    // limit to 100ms
    struct timespec req = {0, (r % 100 + 1) * 1000000};

    nanosleep(&req, NULL);
}

#define DEFAULT_NTRIPS 10
#define DEFAULT_NCARS_EAST 5
#define DEFAULT_NCARS_WEST 5
#define DEFAULT_MAX_CROSSING 2

#define CROSS_BRIDGE(car) my_msleep((car)->time_to_cross)

enum
{
    EAST,
    WEST
};

typedef struct bridge_t_tag
{
    int direction;            // current direction
    int n_crossing;           // number of cars on the bridge
    int max_crossing;         // max number of cars on the bridge
    int n_crossed;            // number of cars that have crossed the bridge in current direction
    int max_crossed;          // max number of cars that can cross if a car is waiting in opposite direction
    int eastb_waiting;        // number of cars waiting, eastbound
    int westb_waiting;        // number of cars waiting, westbound
    pthread_mutex_t mutex;    // mutex to protect shared info
    pthread_cond_t cond_east; // condition variable for eastbound cars
    pthread_cond_t cond_west; // condition variable for westbound cars
} bridge_t;

typedef struct car_t_tag
{
    int id;
    int direction;
    int n_trips;
    int seed;
    int time_to_cross;
    bridge_t *bridge;
} car_t;

#define CAR_DIRECTION(car) ((car)->direction ? "westbound" : "eastbound")

#define PRINT_ARRIVING_MSG(car) printf("car %2d, %s, is arriving...\n", (car)->id, CAR_DIRECTION(car))
#define PRINT_WAITING_MSG(car) printf("car %2d, %s, is waiting...\n", (car)->id, CAR_DIRECTION(car))
#define PRINT_GETTINGON_MSG(car) printf("car %2d, %s, is getting on the bridge...\n", (car)->id, CAR_DIRECTION(car))
#define PRINT_LEAVING_MSG(car) printf("car %2d, %s, is leaving ...\n", (car)->id, CAR_DIRECTION(car))

// this function shows what a car does if it does not care about rules
// comments have some requirements/suggestions for multithreaded version
void cross_bridge_single(car_t *car)
{
    /*  A car may wait if it follows the rules.
     *  If it needs to wait, it should wait in a loop and print a message.
    while (....) {
        PRINT_WAITING_MSG(car);
        pthread_cond_wait(...);
    }
    */

    // messages should be printed when mutex is locked.
    PRINT_GETTINGON_MSG(car); // mutex is locked

    CROSS_BRIDGE(car); // drive on the bridge, should be done when mutex is unlocked.

    PRINT_LEAVING_MSG(car); // mutex is locked
}

void cross_bridge_eastbound(car_t *car)
{
    bridge_t *b = car->bridge;

    pthread_mutex_lock(&b->mutex);

    b->eastb_waiting++;

    // TODO

    while ((b->direction == EAST && b->n_crossed + b->n_crossing >= b->max_crossed && b->westb_waiting > 0) || (b->direction == WEST && b->n_crossed + b->n_crossing < b->max_crossed && b->westb_waiting > 0) || (b->direction == EAST && b->n_crossing >= b->max_crossing) || (b->direction == WEST && b->n_crossing != 0))
    {
        PRINT_WAITING_MSG(car);
        pthread_cond_wait(&b->cond_east, &b->mutex);
    }

    b->direction = EAST;

    PRINT_GETTINGON_MSG(car);
    b->eastb_waiting--;
    b->n_crossing++;

    pthread_mutex_unlock(&b->mutex);

    CROSS_BRIDGE(car);

    pthread_mutex_lock(&b->mutex);

    PRINT_LEAVING_MSG(car);
    b->n_crossing--;
    b->n_crossed++;

    if (((b->n_crossed + b->n_crossing >= b->max_crossed) || (b->eastb_waiting == 0 && b->direction == EAST && b->westb_waiting > 0)) && (b->direction == EAST && b->n_crossing == 0))
    {
        b->direction = WEST;
        b->n_crossed = 0;
        pthread_cond_broadcast(&b->cond_west);
    }
    else
    {
        pthread_cond_broadcast(&b->cond_east);
    }
    pthread_mutex_unlock(&b->mutex);
}

void cross_bridge_westbound(car_t *car)
{
    bridge_t *b = car->bridge;

    // TODO
    pthread_mutex_lock(&b->mutex);
    b->westb_waiting++;

    while ((b->direction == WEST && b->n_crossing >= b->max_crossing) || (b->direction == EAST && b->n_crossing != 0) || (b->direction == EAST && b->n_crossed + b->n_crossing < b->max_crossed && b->eastb_waiting > 0) || (b->direction == WEST && b->n_crossed + b->n_crossing >= b->max_crossed && b->eastb_waiting > 0))
    {
        PRINT_WAITING_MSG(car);
        pthread_cond_wait(&b->cond_west, &b->mutex);
    }
    b->direction = WEST;
    PRINT_GETTINGON_MSG(car);

    b->westb_waiting--;
    b->n_crossing++;

    pthread_mutex_unlock(&b->mutex);
    CROSS_BRIDGE(car);
    pthread_mutex_lock(&b->mutex);

    PRINT_LEAVING_MSG(car);
    b->n_crossing--;
    b->n_crossed++;

    if (((b->n_crossed + b->n_crossing >= b->max_crossed) || (b->westb_waiting == 0 && b->direction == WEST && b->eastb_waiting > 0)) && (b->direction == WEST && b->n_crossing == 0))
    {
        b->direction = EAST;
        b->n_crossed = 0;
        pthread_cond_broadcast(&b->cond_east);
    }
    else
    {
        pthread_cond_broadcast(&b->cond_west);
    }
    pthread_mutex_unlock(&b->mutex);
}

void *thread_car(void *arg_orig)
{
    unsigned short g_random_buffer[3]; // buffer for random numbers

    car_t *car = arg_orig;

    if (car->seed)
    {
        car->seed = (car->seed << 10) + car->id;
        RANDOM_INIT(car->seed);
    }

    for (int i = 0; i < car->n_trips; i++)
    {
        // driving for some time
        if (car->seed)
            my_msleep(RANDOM_INT());
        else
            my_msleep(0);

        PRINT_ARRIVING_MSG(car);

        // need to cross the bridge
        if (car->seed)
            car->time_to_cross = RANDOM_INT();
        if (car->direction)
        {
            cross_bridge_westbound(car);
        }
        else
        {
            cross_bridge_eastbound(car);
        }
    }
    return NULL;
}

void print_help(void);

int main(int argc, char *argv[])
{
    int i, n, ncars_west = DEFAULT_NCARS_WEST, ncars_east = DEFAULT_NCARS_EAST;
    int max_crossing = DEFAULT_MAX_CROSSING, max_crossed = DEFAULT_MAX_CROSSING * 2;
    int n_trips = DEFAULT_NTRIPS;
    int seede, seedw;

    seede = seedw = 3100;

    for (i = 1; i < argc; i++)
    {
        if (!strncmp(argv[i], "-w", 2) && isdigit(argv[i][2]))
        {
            ncars_west = atoi(&argv[i][2]);
        }
        else if (!strncmp(argv[i], "-e", 2) && isdigit(argv[i][2]))
        {
            ncars_east = atoi(&argv[i][2]);
        }
        else if (!strncmp(argv[i], "-m", 2) && isdigit(argv[i][2]))
        {
            max_crossing = atoi(&argv[i][2]);
            if (max_crossing == 0)
                die("max number of cars on the bridge must be positive");
        }
        else if (!strncmp(argv[i], "-c", 2) && isdigit(argv[i][2]))
        {
            max_crossed = atoi(&argv[i][2]);
            if (max_crossing == 0)
                die("max number of cars on the bridge must be positive");
        }
        else if (!strncmp(argv[i], "-t", 2) && isdigit(argv[i][2]))
        {
            n_trips = atoi(&argv[i][2]);
            if (n_trips == 0)
                die("number of trips must be positive");
        }
        else if (!strncmp(argv[i], "-se", 3) && isdigit(argv[i][3]))
        {
            seede = atoi(&argv[i][3]);
        }
        else if (!strncmp(argv[i], "-sw", 3) && isdigit(argv[i][3]))
        {
            seedw = atoi(&argv[i][3]);
        }
        else
        {
            print_help();
        }
    }

    printf("Options: -w%d -e%d -m%d -c%d -t%d -se%d -sw%d\n",
           ncars_west, ncars_east, max_crossing, max_crossed, n_trips, seede, seedw);

    int rv;

    bridge_t bridge = {0};
    bridge.max_crossing = max_crossing;
    bridge.max_crossed = max_crossed;

    rv = pthread_mutex_init(&bridge.mutex, NULL);
    check_pthread_return(rv, "pthread_mutex_init");
    rv = pthread_cond_init(&bridge.cond_east, NULL);
    check_pthread_return(rv, "pthread_cond_init");
    rv = pthread_cond_init(&bridge.cond_west, NULL);
    check_pthread_return(rv, "pthread_cond_init");

    n = ncars_west + ncars_east; // number of threads to create
    pthread_t tid[n];
    car_t cars[n];

    for (i = 0; i < n; i++)
    {
        cars[i] = (car_t){i, EAST, n_trips, seede, 0, &bridge};
        if (i >= ncars_east)
        {
            cars[i].direction = WEST;
            cars[i].seed = seedw;
        }
        rv = pthread_create(&tid[i], NULL, thread_car, &cars[i]);
        check_pthread_return(rv, "pthread_create");
    }
    for (i = 0; i < n; i++)
    {
        rv = pthread_join(tid[i], NULL);
        check_pthread_return(rv, "pthread_join");
    }

    rv = pthread_mutex_destroy(&bridge.mutex);
    check_pthread_return(rv, "pthread_mutex_destroy");
    rv = pthread_cond_destroy(&bridge.cond_east);
    check_pthread_return(rv, "pthread_cond_destroy");
    rv = pthread_cond_destroy(&bridge.cond_west);
    check_pthread_return(rv, "pthread_cond_destroy");

    return 0;
}

void print_help(void)
{
    char *helpmsg =
        "Usage: ./bridge [options]\n"
        "-w<N>    number of westbound cars (default:5).\n"
        "-e<N>    number of eastbound cars (default:5).\n"
        "-t<N>    number of trips each car makes (default:10).\n"
        "-m<N>    max number of cars on the bridge (default:2).\n"
        "-c<N>    max number of cars that can cross the bridge in a phase,\n"
        "         if a car in opposite direciton is waiting. (default:4)\n"
        "-se<N>   seed for eastbound cars (default:3100).\n"
        "-sw<N>   seed for westbound cars (default:3100).\n";
    fprintf(stderr, "%s", helpmsg);
    exit(EXIT_FAILURE);
}