#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include<sys/wait.h>
#include "header.h"

#define PRUEBA "prueba.txt"
#define ALICE "alice.txt"
#define NUM_PROC 4

//0. Recibe archivo completo y lee línea por línea, llevando un contador de línea. Cada línea se manda en un struct {num_linea, texto} a cada uno de los 4 archivos (chunks), uniformemente.
//1. Cada mapper recibe su chunk de structs, lee cada uno, tomando nota de los números de línea. Actúa como un inverted index normal y genera sus propios A1 (palabras) y A2 (ocurrencias).
//3. Desde un programa, recorremos cada uno de los archivos invertidos (A1 y A2 en conjunto) de los mappers, se leen secuencialmente para generar un arreglo ordenado de structs de {palabra, [ocurrencias]}
// 4. ordenamos el arreglo con quicksort, lo leemos para reducirlo y escupimos A1 y A2 reducidos final.


// Function to split input file into 4 chunks
int divideFile(FILE* file_ptr) {
  FILE *ptr1, *ptr2, *ptr3, *ptr4;
  
  ptr1 = fopen("chunk1.txt", "w+");
  ptr2 = fopen("chunk2.txt", "w+");
  ptr3 = fopen("chunk3.txt", "w+");
  ptr4 = fopen("chunk4.txt", "w+");

  char* line;
  size_t line_size = sizeof(char) * 100; // bytes
  line = (char * ) malloc(line_size);

  int flag = 0;
  const char sep[2] = " ";

  // Read every line
  while(getline(&line, &line_size, file_ptr) != -1) {
    char *token;
    token = strtok(line, sep); // tokenize the line

    // Copy the token into word, removing non-alphabetic
    while (token != NULL) {
      char word[50]; //
      memset(word,'\0',sizeof(word));

      int j = 0; // index for clean word

      for (int i=0;token[i]!='\0'; i++) {
        if(token[i]>='A' && token[i]<='Z') {
          token[i] += 32; // to lowercase
        }
        if(token[i]>='a' && token[i]<='z') {
          word[j] = token[i];
          j++;
        } else if (j>=1) break; // end of word
      }

      // If word is non-empty
      if (strcmp("", word) != 0) {
        
        if (flag ==0) {
          // Write string to A
          fprintf(ptr1, "%s\n", word);
          flag = 1;
        }

        else if (flag==1) {
          // Write string to B
          fprintf(ptr2, "%s\n", word);
          flag = 2;
        } 

        else if (flag==2) {
          // Write string to C
          fprintf(ptr3, "%s\n", word);
          flag = 3;
        } 
        else if (flag==3) {
          // Write string to D
          fprintf(ptr4, "%s\n", word);
          flag = 0;
        }
      }
 
      token = strtok(NULL, sep); // next token
    } // end of while per token

  } // end of while per line
  fclose(ptr1);
  fclose(ptr2);
  fclose(ptr3);
  fclose(ptr4);
  fclose(file_ptr);
  
  return 0;
}

int main(int argc, char* argv[]) {
  //int n = atoi(argv[1]);
  FILE* file_ptr;
  file_ptr = fopen(PRUEBA, "r");

  divideFile(file_ptr); 

  int n = NUM_PROC;
  printf("Typed n:%d\n", n);

  pid_t child_pid, wpid;
  int status = 0;

  //Father code (before child processes start)
  printf("Padre PID: %d\n", getpid());

  FILE* pointers[NUM_PROC];
  pointers[0] = fopen("chunk1.txt", "r");
  pointers[1] = fopen("chunk2.txt", "r");
  pointers[2] = fopen("chunk3.txt", "r");
  pointers[3] = fopen("chunk4.txt", "r");

  for (int id=0; id<NUM_PROC; id++) {
      if ((child_pid = fork()) == 0) {
          printf("Hijo %d PID: %d\n", id, getpid());

          chunkMap(id, pointers[id]);

          fclose(pointers[id]);
          exit(0);
      }
  }

  while ((wpid = wait(&status)) > 0);
  printf("All done\n");
  return 0;
}

  // Separate the file into four different parts
  // Do two consecutive forks, to create 4 processes
  // Assign each process to a file pointer
  // each process calls to mapper() and produces a txt file
  // after all processes are done, we paste the txts together
  // The pasted txt is sorted and sent to a reducer
  // Reducer traverses the list 

//https://experiencia21.tec.mx/courses/110335/assignments/4029500?module_item_id=6914950

/*

1.-Realizarán una investigación de lo que es el paradigma MapReduce, sus orígenes, sus usos y los patrones de diseño bajo este modelo de programación en especial la manera en que los archivos invertidos pueden ser creados bajo este modelo(30 puntos)

2.-Realizarán un programa en lenguaje C que implemente un archivo invertido de la misma manera que el examen de medio término pero en esta ocasión bajo el modelo de programación MapReduce. (45 puntos)

3.-Realizaran un programa que dando una palabra indicará las posiciones dónde se encuentra dicha palabra dentro del texto. Es forzoso que utilice el o los archivos generados en el punto anterior. (25 puntos)

*/

  // https://stackoverflow.com/questions/5460421/how-do-you-write-a-c-program-to-execute-another-program

  // https://stackoverflow.com/questions/19461744/how-to-make-parent-wait-for-all-child-processes-to-finish