#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include "header.h"
#define A1 "A1"
#define A2 "A2"

/* INVERTED INDEX GENERATOR */

typedef struct record1 {
  char word[50];    // word key
  int ptr;      // first position in A2 (head of linked list)
} record1;

typedef struct record2 {
  int line; // line number in original file
  int next; // next position within A2
} record2;

// DYNAMIC ARRAY FOR record1 STRUCTS
typedef struct {
  record1* array;  
  size_t used;
  size_t size;
} Array;

void initArray(Array* a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(record1));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, record1 element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(record1));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

void chunkMap(int id, FILE* chunk) {

    printf("chunkMap called by %d\n", id);
    char sid[2];
    sprintf(sid, "%d", id);
    
    char name1[4] = A1;
    strcat(name1, sid);
    char name2[4] = A2;
    strcat(name2, sid);
    
}