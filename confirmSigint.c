#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
/* Programa para que cuando el usuario realice un Ctrl-C, se escriba el mensaje, “Digite Ctrl-c nuevamente para terminar”. Si en 5 segundos el usuario no realiza un nuevo Ctrl-c, la aplicación continuará. En caso contrario, la aplicación terminará.*/

void control_c(int);

void cancel(int signum)
{
    printf("\nCtrl-C cancelado\n");
    signal(SIGINT, control_c);
    return;
}

void control_c(int signum)
{
    signal(SIGALRM, cancel);
    // Primer control_c
    // lo que se corre al expirar la alarma.
    printf("\nDigite Ctrl-C nuevamente para terminar\n");
    /* inicia el temporizador por 5 segundos */
    alarm(5);
    signal(SIGINT, SIG_DFL); // regresa el comportamiento normal de Ctrl-C

    return;
}

int main()
{
    // Declarar el manejador de señal
    signal(SIGINT, control_c);

    while (1)
    {
        printf("Procesando cosas\n");
        sleep(1);
    }

    return EXIT_SUCCESS;
}