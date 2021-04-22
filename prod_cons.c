#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define STACKSIZE 20
#define THREADS 4
#define WIDTH 60
#define UNITS 100

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;
char stack[STACKSIZE];
int top = -1;

void printStack(int op, char c)
{
    int lenMsg;
    if (op == 0)
    {
        lenMsg = 8;
    }
    else
    {
        lenMsg = 7;
    }

    for (int i = 0; i < top + 1; i++)
    {
        printf("%c", stack[i]);
    }

    int lines = WIDTH - (top + 1) - lenMsg;

    for (int j = 0; j < lines; j++)
    {
        printf("-");
    }
    if (op == 0)
        printf("PUSH:>%c<", c);
    if (op == 1)
        printf("POP:>%c<", c);

    putchar('\n');
}

// SYNCRONIZED STACK METHODS
int isEmpty()
{
    if (top == -1)
    {
        return 1;
    }
    return 0;
}

int isFull()
{
    if (top == STACKSIZE)
    {
        return 1;
    }
    return 0;
}
/* Métodos push, pop deben estar sincronizados.
    Debemos esperar si no hay nada que poppear, y esperar si no 
    hay espacio para hacer push. 
    Cada vez que cambia el estado del stack con un push o pop exitoso,
    avisamos a todos los threads que están esperando el lock. */
char pop()
{
    char elem;
    while (isEmpty())
    {
        // wait
        pthread_cond_wait(&data_cond, &lock);
    }
    elem = stack[top];
    top = top - 1;
    //msg[5] =  elem;
    printStack(1, elem);
    // avisamos a los demás threads que el stack cambió de status
    pthread_cond_broadcast(&data_cond);
    return elem;
}

int push(char elem)
{
    while (isFull())
    {
        // wait
        pthread_cond_wait(&data_cond, &lock);
    }
    top = top + 1;
    stack[top] = elem;
    printStack(0, elem);
    // avisamos a los demás threads que stack cambió de status
    pthread_cond_broadcast(&data_cond);
    return 0;
}

// We have Producer threads and we have Consumer threads
// Producers push to stack, Consumers pop from stack
void *producer(void *arg)
{
    char c;
    srand(time(0));
    for (int i = 0; i < UNITS; i++)
    {
        c = (char)((rand() % (126 - 33 + 1)) + 33);
        pthread_mutex_lock(&lock);
        push(c);
        //printf("Producer %d => >%c<\n", *(int*)arg, c);
        pthread_mutex_unlock(&lock);

        usleep(rand() % 301);
    }
    pthread_exit(0);
}

void *consumer(void *arg)
{
    char c;
    srand(time(0));
    // Tries to produce up to 20 chars.
    for (int i = 0; i < UNITS; i++)
    {

        pthread_mutex_lock(&lock);
        char c = pop();
        //printf(">%c< => Consumer %d\n",c,*(int*)arg);
        pthread_mutex_unlock(&lock);

        usleep(rand() % 301);
    }
    pthread_exit(0);
}

int main()
{
    // Create 4 threads.
    // run the four threads.
    pthread_t threads[THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

    int p1 = 0;
    int p2 = 1;
    int c1 = 2;
    int c2 = 3;

    int prod1 = pthread_create(&threads[p1], &attr, producer, (void *)&p1);
    int prod2 = pthread_create(&threads[p2], &attr, producer, (void *)&p2);
    int cons1 = pthread_create(&threads[c1], &attr, consumer, (void *)&c1);
    int cons2 = pthread_create(&threads[c2], &attr, consumer, (void *)&c2);

    // Unir los threads al principal
    for (int k = 0; k < THREADS; k++)
    {
        int tmp = pthread_join(threads[k], NULL);
        if (tmp != 0)
        {
            printf("Error %d %c", tmp, tmp);
            printf("Problema para esperar al thread %d\n", k);
            exit(EXIT_FAILURE);
        }
    }
    printf("END ----- Is stack empty? %d\n", isEmpty());

    pthread_mutex_destroy(&lock);
    return 0;
}

/*
TO RUN:
  gcc -o out -pthread prod_cons.c
*/
