#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include<sys/wait.h>
#include <sys/types.h>
#include "header.h"

typedef struct {
  int* array;  
  size_t used;
  size_t size;
} IntArray;

void initIntsArray(IntArray* a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertInts(IntArray *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void freeInts(IntArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

typedef struct record3 {
  char word[50];
  IntArray ocurrences; // ¿qué tamaño?
} record3;

typedef struct {
  record3* array;  
  size_t used;
  size_t size;
} Array3;

// For qsort
int compareRecords(const void *v1, const void *v2) {
  const struct record3* a = v1;
  const struct record3* b = v2;
  return strcmp(a->word, b->word);
}

void initArray3(Array3* a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(record3));
  a->used = 0;
  a->size = initialSize;
}

void insertArray3(Array3 *a, record3 element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(record3));
  }
  a->array[a->used++] = element;
}

void freeArray3(Array3 *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

int posA2;

void positions (int r1ptr, FILE * archivo2, IntArray *arr_ptr){
  record2 r2;
  // Posicionarnos en r1ptr
  fseek(archivo2, sizeof(record2)*r1ptr, SEEK_SET);
  fread(&r2, sizeof(record2), 1, archivo2);
  insertInts(arr_ptr, r2.line);
  while(r2.next != -1){
    fseek(archivo2, sizeof(record2)*r2.next, SEEK_SET);
    fread(&r2, sizeof(record2), 1, archivo2);
    insertInts(arr_ptr, r2.line);
  }
}

int printToBinary(char word[50], IntArray* arr, FILE* b1, FILE* b2) {
// 1. Create and print record1 into B1
  record1 r1;
  strcpy(r1.word, word);
  r1.ptr = posA2;
  fwrite(&r1, sizeof(record1), 1, b1);
  
  // 2. Print consecutive line entries into B2 except the last
  int i;
  for (i=0; i<arr->used-1; i++) {
    record2 r2;
    r2.line = arr->array[i];  // line number
    r2.next = posA2+1;        // next position is current posA2+1
    fseek(b2, sizeof(record2)*posA2, SEEK_SET); // is it necessary?
    fwrite(&r2, sizeof(record2), 1, b2);
    posA2++;
  }

  record2 r2;
  r2.line = arr->array[i];
  r2.next = -1;
  fseek(b2, sizeof(record2)*posA2, SEEK_SET); // is it necessary?
  fwrite(&r2, sizeof(record2), 1, b2);
  posA2++;
  return 0; 
}


// Prints word and its occurrences to a debugging .txt
void printRecord(char word[50], IntArray* arr, FILE* finalTest){
  fprintf(finalTest,"%s ", word);
 
  for(int i=0; i<arr->used; i++){
    // imprime cada ocurrencia
    if (i == arr->used -1) {
      fprintf(finalTest,"%d\n",arr->array[i]);  
    } else {
      fprintf(finalTest,"%d, ",arr->array[i]);    
    }
  }
}

// Read every pair of A1, A2, generate an array of {word: positions} structs. Sort and then spit into final A1 and A2.

// Receives sorted r3 array, accumulates same-word occurrences, prints to txt
void finalReduce(Array3* records){
  FILE * b1 = fopen("B1", "wb+");
  FILE * b2 = fopen("B2", "wb+");

  char currWord[50]; 
  IntArray currArray;
  char nextWord[50];
  IntArray nextArray;

  strcpy(currWord, records->array[0].word);
  currArray = records->array[0].ocurrences;

  posA2=0; // solo necesitamos 1

  int i;
  for(i=1; i<records->used; i++) {
    strcpy(nextWord, records->array[i].word);
    nextArray = records->array[i].ocurrences;
    
    if (strcmp(currWord, nextWord) == 0){
      for(int j=0; j < nextArray.used; j++){
        insertInts(&currArray, nextArray.array[j]);
      }
    }
    else{
      // despachar la información que tienes acumulada,
      printToBinary(currWord, &currArray, b1, b2);
      // actualizar palabraActual y arreglo Actual
      strcpy(currWord, nextWord);
      currArray = nextArray; 
    }
  }

  printToBinary(currWord, &currArray, b1, b2);
  
  fclose(b1);
  fclose(b2);

  }


int reduceFiles() {
  printf("Merging mapper outputs...\n");
  
  FILE *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7, *ptr8;
  int k = 0;
  ptr1 = fopen("A11", "rb");
  ptr2 = fopen("A12", "rb");
  ptr3 = fopen("A13", "rb");
  ptr4 = fopen("A14", "rb");
  ptr5 = fopen("A21", "rb");
  ptr6 = fopen("A22", "rb");
  ptr7 = fopen("A23", "rb");
  ptr8 = fopen("A24", "rb");

  FILE* pointers1[4];     // A1
  FILE* pointers2[4];     // A2
  
  pointers1[0] = ptr1;
  pointers1[1] = ptr2;
  pointers1[2] = ptr3;
  pointers1[3] = ptr4;
  pointers2[0] = ptr5;
  pointers2[1] = ptr6;
  pointers2[2] = ptr7;
  pointers2[3] = ptr8;
  
  Array3 * records = malloc(sizeof(Array3));
  initArray3(records, 50);
  
  // Para cada par de archivos A1 y A2
  for (int p=0; p<4; p++) {
    // Para cada entrada de A1
    int i = 0;
    while (fseek(pointers1[p], sizeof(record1)*i, SEEK_SET) == 0){        
      record1 r1;
      fread(&r1, sizeof(record1), 1, pointers1[p]);
      
      if (feof(pointers1[p])) {
        break;
      }

      
      record3* r3 = malloc(sizeof(record3));
      strcpy(r3->word, r1.word);
      initIntsArray(&r3->ocurrences, 5);
      positions(r1.ptr, pointers2[p], &r3->ocurrences); 
      insertArray3(records, *r3);
      i++;
    }
      // para cada entrada de A1, llamar a positions. 
  }

  qsort(records->array, records->used, sizeof(record3), compareRecords);  // sort words alphabetically
  
  // Delete helper files
  system("rm A11 A21 A12 A22 A13 A23 A14 A24");

  // Actual reducing:
  printf("Reducing files...\n");
  finalReduce(records); // crea últimos A1 y A2
  
  // Close file pointers
  for(k=0; k<NUM_PROC; k++) {
    fclose(pointers1[k]);
    fclose(pointers2[k]);
  }
  
  printf("Final inverted index completed\n");
  return 0;
}