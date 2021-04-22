#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int busca(char ** palabras, char * palabra, int inicio, int fin, char * quien) {
  for (int i = inicio; i < fin; i++) {
    if (strcmp(palabra, palabras[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

void printMessage(int sigNum) {
  printf("Papá encontré la palabra que buscabas, ahora puedo morir en paz\n");
  return;
}

int main() {
  signal(SIGUSR1, printMessage); // imprime mensaje cuando recibe SIGINT del hijo
  char * palabras[] = {
    "hola",
    "adios",
    "no-se",
    "hola",
    "sofia",
    "hola",
    "aaa",
    "hola"
  };
  int tamanio = sizeof(palabras) / sizeof(char * );
  int mitad = tamanio / 2;
  pid_t hijo_id;
  pid_t padre_id = getpid();
  printf("Padre id: %d\n", padre_id);

  if ((hijo_id = fork()) != 0) {
    printf("Hijo id: %d\n", hijo_id);
    int n = busca(palabras, "sofia", 0, mitad, "padre");
    int status;
    wait( & status);  // espera a que acabe el hijo
    status = WEXITSTATUS(status);
    if (status == 1) {
      printf("Mi hijo encontró la palabra\n");
    }
     else {
      printf("Mi hijo no encontró nada\n");
    }
    if (n==1) {
      // El padre encontró
       printf("El padre encontró la palabra\n");
    }

    else{
       printf("El padre no encontró la palabra\n");
    }

  } else {
    // ejecución del hijo
    int n = busca(palabras, "sofia", mitad, tamanio, "hijo");
    if (n==1) {
      kill(padre_id, SIGUSR1); 
      // matar proceso hijo y envia señal al padre 
    }
    exit(n);
  }
  return 0;
}