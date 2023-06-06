/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *parameters);
void *consumer (void *parameters);

/* Global variables */
int semid = 0;                      /* to store id of semaphore set */
int queue_size = 0;                 /* to store size of queue */
int total_job_cnt = 0;              /* to store total jobs created */
int total_consume_cnt = 0;          /* to store total jobs consumed */
int max_job_cnt = 0;                /* to store producer's maximum job count */

int main (int argc, char **argv)
{
  srand(time(NULL));    /* seed for random functions */
  int arg[4];

  /* Make sure command line arguments are sufficient and correct */
  if (argc != 5) {
    cerr << "Please insert 4 arguments when running the file." << endl;
    return INSUFFICIENT_ARG;
  }
  if ((arg[0] = check_arg(argv[1])) <= 0) {
    cerr << "Queue size has to be a positive integer." << endl;
    return INVALID_INPUT;
  }
  if ((arg[1] = check_arg(argv[2])) <= 0) {
    cerr << "Producer's job count has to be a positive integer." << endl;
    return INVALID_INPUT;
  }
  if ((arg[2] = check_arg(argv[3])) <= 0) {
    cerr << "No. of producers has to be a positive integer." << endl;
    return INVALID_INPUT;
  }
  if ((arg[3] = check_arg(argv[4])) <= 0) {
    cerr << "No. of consumers has to be a positive integer." << endl;
    return INVALID_INPUT;
  }

  /* Extract command line arguments for the following variables */
  queue_size = arg[0];             
  max_job_cnt = arg[1];        
  int no_of_prod = arg[2];          /* obtain number of producers */ 
  int no_of_cons = arg[3];          /* obtain number of consumers */

  /* Declare the queue based on queue size input */
  job circularqueue[queue_size];

  /* Create a set of 3 semaphores */
  if ((semid = sem_create(SEM_KEY, 3)) < 0) {
    perror("Error when creating semaphore");
    return SEM_CREATE_FAIL;
  };
  /* Initialize binary semaphore for crital section */
  if (sem_init(semid, 0, 1) < 0) {                  
    perror("Error when initialising 1st semaphore");
    return SEM_INIT_FAIL;
  };
  /* Initialize semaphore indicating number of spaces available in queue */
  if (sem_init(semid, 1, queue_size) < 0) {
    perror("Error when initialising 2nd semaphore");
    return SEM_INIT_FAIL;
  }
  /* Initialize semaphore indicating number of jobs available in queue */
  if (sem_init(semid, 2, 0) < 0 ) {
    perror("Error when initialising 3rd semaphore");
    return SEM_INIT_FAIL;
  }

  /* Declare thread identifiers for producers and consumers */
  pthread_t producerid[no_of_prod];
  pthread_t consumerid[no_of_cons];    
  
  /* Declare and assign value to thread arguments */
  pthread_arg1 prod_pthread_arg[no_of_prod];
  pthread_arg2 cons_pthread_arg[no_of_cons];

  /* Assign producer id and queue pointer to producer thread arguments */    
  for (int i=0; i<no_of_prod; i++) {          
    prod_pthread_arg[i].producer_id = i+1;  
    prod_pthread_arg[i].queue = circularqueue;   
  }
  /* Assign consumer id and queue pointer to consumer thread arguments */ 
  for (int j=0; j<no_of_cons; j++) {          
    cons_pthread_arg[j].consumer_id = j+1;  
    cons_pthread_arg[j].queue = circularqueue; 
  }

  /* Create threads for each producer */
  for (int i=0; i<no_of_prod; i++) {          
    if ((pthread_create (&producerid[i], NULL, producer, 
      (void *) &prod_pthread_arg[i])) != 0) {
        perror("Error when initializing producer's thread");
        return PTHREAD_CREATE_FAIL;
    };
  };
  /* Create threads for each consumer */
  for (int j=0; j<no_of_cons; j++) {
    if ((pthread_create (&consumerid[j], NULL, consumer, 
      (void *) &cons_pthread_arg[j])) != 0) {
        perror("Error when initializing consumer's thread");
        return PTHREAD_CREATE_FAIL;
    }
  };
  
  /* Make main function wait for all producer threads to terminate  */
  for (int i=0; i<no_of_prod; i++) {
    if ((pthread_join (producerid[i], NULL)) != 0) {
      printf("Error when joining producer thread. Error no: %i\n", errno);
      return PTHREAD_JOIN_FAIL;
    };  
  };
   /* Make main function wait for all consumer threads to terminate  */
  for (int j=0; j<no_of_cons; j++) {
    if ((pthread_join (consumerid[j], NULL)) != 0) {
      printf("Error when joining producer thread. Error no: %i\n", errno);
      return PTHREAD_JOIN_FAIL;
    };  
  };

  /* Closes the semaphore */
  if (sem_close(semid) < 0) {
      perror("Error when closing semaphore");
      return SEM_CLOSE_FAIL;
  }
  return 0;
}

void *producer (void *parameters) 
{
  /* Convert generic pointer to thread argument pointer */
  pthread_arg1 *param = (pthread_arg1 *) parameters;
  /* Obtain producer id from thread argument */
  int producer_no = param->producer_id;
  /* Initialise job production count for each producer */
  int job_produced = 0;

  while (1) {
    /* Wait to see if there is available queue space for 20 seconds */
    int timedwait_value = sem_timedwait(semid, 1);

    if ((timedwait_value != 0) && (errno == EAGAIN)) {                                           
      printf("Producer(%i): No space in queue to produce.\n", producer_no);
      /* Exit thread if there is no space available after 20 seconds */         
      pthread_exit(0);                                                          
    }

    /* If other error occur when waiting for space available semaphore */
    else if (timedwait_value != 0) {
      printf("Producer(%i): Space available semaphore error.\n", producer_no);
      /* Exit thread after printing error message */
      pthread_exit(0);  
    }

    else {
      /* Lock critical section which access queue and shared variables */
      sem_wait(semid, 0);
      if (errno) {
        perror("Error when waiting for critical section semaphore");
        pthread_exit(0);
      }

      /* Declare current job to be created */
      job current_job;
      /* Get current job id based on overall no. of jobs produced */
      current_job.job_id = ((total_job_cnt) % queue_size);
      /* Randomly generate job duration (1 - 10 seconds) */
      current_job.job_time = 1 + (rand()%10);

      printf("Producer(%i): Job id %i duration %i\n", producer_no, 
        current_job.job_id, 
        current_job.job_time);

      /* Add job into the queue based on the queue id */ 
      param->queue[current_job.job_id] = current_job;

      /* Add to the total jobs produced by all producers */
      total_job_cnt++;

      /* Release critical section*/
      sem_signal(semid, 0);
      if (errno) {
        perror("Error when signalling critical section semaphore");
        pthread_exit(0);
      }

      /* Signal to consumer that job has been added */
      sem_signal(semid, 2);
      if (errno) {
        perror("Error signalling job available semaphore");
        pthread_exit(0);
      }

      /* Increase count of jobs produced for current producer */
      job_produced++;

      /* Determine if producers should continue or stop producing jobs */                                             
      if (job_produced < max_job_cnt) {
        /* Wait randomly for 1 to 5 seconds before next job creation */
        int time_add_job = 1 + rand()%5;
        sleep(time_add_job);
      }
      else {
        printf("Producer(%i): No more jobs to generate.\n", producer_no);

        /* Producer exits thread after finish producing all jobs */
        pthread_exit(0);
      }
    }
  }
}

void *consumer (void *parameters) 
{
  /* Convert generic pointer to thread argument pointer */
  pthread_arg2 *param = (pthread_arg2 *) parameters;
  /* Obtain consumer id from thread argument */
  int consumer_no = param->consumer_id;
  
  while (1) {
    /* Wait to see if there is available jobs for 20 seconds */
    int timedwait_value = sem_timedwait(semid, 2);

    if ((timedwait_value != 0) && (errno == EAGAIN)) {            
      printf("Consumer(%i): No more jobs left.\n", consumer_no); 
      /* Exit thread if there is no jobs available after 20 seconds */        
      pthread_exit(0);                                           
    }

     /* If other error occur when waiting for space available semaphore */
    else if (timedwait_value != 0) {
      printf("Consumer(%i): Space available semaphore error.\n", consumer_no);
       /* Exit thread after printing message */  
      pthread_exit(0);
    }

    else {
      /* Lock critical section which access queue and shared variables */
      sem_wait(semid, 0);
      if (errno) {
        perror("Error waiting critical section semaphore");
        pthread_exit(0);
      }
      
      /* Declare current job to be consumed */
      job current_job;
      /* Get job id based on overall no. of jobs consumed */
      current_job.job_id = ((total_consume_cnt) % queue_size);
      
      /* Get full job details using job id */  
      current_job = param->queue[current_job.job_id];

      /* Add to the total jobs consumed by all consumers */
      total_consume_cnt++;

      /* Release critical section*/
      sem_signal(semid, 0);
      if (errno) {
        perror("Error signalling critical section semaphore");
        pthread_exit(0);
      }

      /* Signal to producer that new space is available */
      sem_signal(semid, 1); 
      if (errno) {
        perror("Error signalling space available semaphore");
        pthread_exit(0);
      }

      /* Simulate job consumption through sleep() using job duration */
      printf("Consumer(%i): Job id %i executing sleep duration %i\n", 
        consumer_no, current_job.job_id, current_job.job_time);

      sleep(current_job.job_time);

      printf("Consumer(%i): Job id %i completed\n", 
        consumer_no, current_job.job_id);
    }
  }
}