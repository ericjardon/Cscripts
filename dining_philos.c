#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Each philosopher is a Thread.
// Each fork is a mutex variable

// A philosopher: thinks, locks left, locks right, eats, unlocks left, unlocks right, and repeats.

#define PHILOSOPHERS 5
#define ITERATIONS 10

typedef struct {
  char name[15];
  int id;
  int count;
  pthread_mutex_t * left;
  pthread_mutex_t * right;
} philosopher;

// Globally accessible pointers
pthread_mutex_t * forks[PHILOSOPHERS];
philosopher * philos[PHILOSOPHERS];

// Left-handed philosopher
void* lefty(void* arg) {
  philosopher* me = philos[(*(int*)arg)];
  srand(time(0));

  for(int j=0; j<ITERATIONS; j++) {
    // pick left fork
    pthread_mutex_lock(me->left);
    usleep(rand()%100);
    // pick right fork
    pthread_mutex_lock(me->right);
    // eat for an ubcertain amount of time
    me->count ++;
    printf("%s is eating...\n", philos[*(int *)arg]->name);
    usleep(rand()%300);
    // drop left fork
    pthread_mutex_unlock(me->left);
    usleep(rand()%100);
    // drop right fork
    pthread_mutex_unlock(me->right);
    // think until hungry
    printf("%s is thinking...\n", philos[*(int*)arg]->name);
    usleep(rand()%400);
  }

  pthread_exit(0);
}

void* righty(void* arg) {
  philosopher* me = philos[(*(int*)arg)];
  srand(time(0));

  for(int j=0; j<ITERATIONS; j++) {
    // pick right fork first
    pthread_mutex_lock(me->right);
    usleep(rand()%100);
    // pick left fork
    pthread_mutex_lock(me->left);
    // eat for an ubcertain amount of time
    me->count ++;
    printf("%s is eating...\n", philos[*(int *)arg]->name);
    usleep(rand()%300);
    // drop left fork
    pthread_mutex_unlock(me->left);
    usleep(rand()%100);
    // drop right fork
    pthread_mutex_unlock(me->right);
    // think until hungry
    printf("%s is thinking...\n", philos[*(int*)arg]->name);
    usleep(rand()%400);
  }

  pthread_exit(0);
}


int main(void) {
  pthread_t threads[PHILOSOPHERS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

  philosopher * a;  
  a = (philosopher *) malloc(sizeof(philosopher));  
  strcpy(a->name, "Aristotle");
  a->id = 0;
  a->count = 0;

  philosopher * b;
  b = (philosopher *) malloc(sizeof(philosopher));  
  strcpy(b->name, "Buddha");
  b->id = 1;
  b->count = 0;

  philosopher * c;
  c = (philosopher *) malloc(sizeof(philosopher));  
  strcpy(c->name, "Confucius");
  c->id = 2;
  c->count = 0;

  philosopher * d;
  d = (philosopher *) malloc(sizeof(philosopher));  
  strcpy(d->name, "Descartes");
  d->id = 3;
  d->count = 0;

  philosopher * e;
  e = (philosopher *) malloc(sizeof(philosopher));  
  strcpy(e->name, "Epicurus");
  e->id = 4;
  e->count = 0;


  philos[0] = a;
  philos[1] = b;
  philos[2] = c;
  philos[3] = d;
  philos[4] = e;

  pthread_mutex_t f1 = PTHREAD_MUTEX_INITIALIZER; 
  pthread_mutex_t f2 = PTHREAD_MUTEX_INITIALIZER; 
  pthread_mutex_t f3 = PTHREAD_MUTEX_INITIALIZER; 
  pthread_mutex_t f4 = PTHREAD_MUTEX_INITIALIZER; 
  pthread_mutex_t f5 = PTHREAD_MUTEX_INITIALIZER;

  forks[0] = &f1;
  forks[1] = &f2;
  forks[2] = &f3;
  forks[3] = &f4;
  forks[4] = &f5;

  for(int i=0; i<PHILOSOPHERS; i++) {
    philos[i]->left = forks[i];
    philos[i]->right = forks[(i+1)%5];
  }

  // create, run the threads
  int thA = pthread_create(&threads[0], &attr, lefty, (void *)&(a->id));
  int thB = pthread_create(&threads[1],  &attr, lefty, (void *)&(b->id));
  int thC = pthread_create(&threads[2],  &attr, lefty, (void *)&(c->id));
  int thD = pthread_create(&threads[3],  &attr, lefty, (void *)&(d->id));
  int thE = pthread_create(&threads[4],  &attr, righty, (void *)&(e->id));

  // there only needs to be at least one righty to avoid deadlock.

  // joins threads
  for (int k = 0; k < PHILOSOPHERS; k++) {
    int tmp = pthread_join(threads[k], NULL);
    if (tmp!= 0) {
      printf("Error %d %c", tmp, tmp);
      printf("Problem encountered waiting for thread no. %d\n", k);
      exit(EXIT_FAILURE);
    }
  }

  putchar('\n');

  // print results and free memory
  for (int i=0; i<PHILOSOPHERS; i++) {
    printf("%s ate %d times\n", philos[i]->name, philos[i]->count);
    free(philos[i]);
  }

  return 0;
}