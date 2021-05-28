#define NUM_PROC 4

#ifndef HEADER_DOT_H    /* This is an "include guard" */
#define HEADER_DOT_H

typedef struct record1 {
  char word[50];    // word key
  int ptr;      // first position in A2 (head of linked list)
} record1;

typedef struct record2 {
  int line; // linea en el archivo .txt
  int next; // sig posicion dentro de A2
} record2;  

void chunkMap(int, FILE *);
int reduceFiles();

#endif