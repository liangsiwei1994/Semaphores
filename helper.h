/******************************************************************
 * Header file for the helper functions. This file includes the
 * required header files, as well as the function signatures and
 * the semaphore values (which are to be changed as needed).
 ******************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>
# include <ctype.h>
# include <iostream>
# include <time.h>

using namespace std;

# define SEM_KEY 0x94

/* Define Errors */
#define INSUFFICIENT_ARG 1
#define INVALID_INPUT 2
#define SEM_CREATE_FAIL 3
#define SEM_INIT_FAIL 4
#define PTHREAD_CREATE_FAIL 5
#define PTHREAD_JOIN_FAIL 6
#define SEM_CLOSE_FAIL 7

union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
};

int check_arg (char *);
int sem_create (key_t, int);
int sem_init (int, int, int);
void sem_wait (int, short unsigned int);
void sem_signal (int, short unsigned int);
int sem_close (int);
int sem_timedwait (int, short unsigned int);

/* Struct of job in queue */
struct job {           
  int job_id;           /* job id */
  int job_time;         /* corresponding duration */
};

/* Producer thread arguments struct */
struct pthread_arg1 {
int producer_id;        /* id of each producers */
job *queue;             /* queue to store jobs created */
};

/* Consumer thread arguments struct */
struct pthread_arg2 {
int consumer_id;        /* id of each consumers */
job *queue;             /* queue to consume jobs from */
};