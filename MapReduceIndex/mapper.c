#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include "header.h"
#define A1 "A1"
#define A2 "A2"

/* INVERTED INDEX GENERATOR */

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


int printFile1(FILE * a1, int id);
int printFile2(FILE * a2, int id);

int compareUser(const void *v1, const void *v2)
{
  // comparación para ordenamiento de registros 1
  const struct record1* r1 = v1;
  const struct record1* r2 = v2;
  return strcmp(r1->word, r2->word);
}

int printFileOne(FILE * a1, int id) {
  printf("-----A1 ----\n");
  int i=0;
  while (fseek(a1, sizeof(record1)*i, SEEK_SET) == 0){
    record1 r1;
    fread(&r1, sizeof(record1), 1, a1);
    if (feof(a1)) {
      // Indica si tratamos de escribir más allá de EOF
      break;
    }
    printf("%s %d\n",r1.word, r1.ptr);
    i++;
  }
  printf("----END A1----\n");
  return 0;
}


int printFile2(FILE* a2, int id) {
  printf("-----BEGIN A2 ----\n");
  int i=0;
  while (fseek(a2, sizeof(record2)*i, SEEK_SET) == 0){
    record2 r2;
    fread(&r2, sizeof(record2), 1, a2);
    if (feof(a2)) {
      // Indica si tratamos de escribir más allá de EOF
      break;
    }
    printf("%d: %d next: %d\n",i, r2.line, r2.next);
    i++;
  }
  printf("----END A2----\n");
  return 0;
}

int traverseArchivo1(FILE* ar, char* target) {
  int i = 0;
  while (fseek(ar, sizeof(record1)*i, SEEK_SET) == 0){
    record1 r1;
    fread(&r1, sizeof(record1), 1, ar);
    if(strcmp(r1.word, target)==0){
      return i;
    }
    if (feof(ar)) {
      // Indica si tratamos de escribir más allá de EOF
      break;
    }
    i++;
  }
  return -1;  // not found
}

// MAIN METHOD
void chunkMap(int id, FILE* chunk) {
  char sid[2];
  sprintf(sid, "%d", id+1);   // int -> string
  
  char chunk_txt[11] = "chunk"; // chunkX.txt
  strcat(chunk_txt, sid); strcat(chunk_txt, ".txt");

  printf("Mapping chunk: %s\n", chunk_txt);

  FILE* toread;
  toread = fopen(chunk_txt, "r");

  char name1[4] = A1; // A1X
  char name2[4] = A2; // A2X

  strcat(name1, sid);
  strcat(name2, sid);
  FILE* archivo1 = fopen(name1, "wb+");
  FILE* archivo2 = fopen(name2, "wb+");
  // START PROCESSING

  int posA2 = 0;
  Array a;
  initArray(&a, 50);  // initially 50 elements
  
  // Lectura línea por línea
  char* line;
  size_t size = sizeof(char)*100;
  line = (char *) malloc(size);
  int line_index; // first token of a line

  while( getline(&line, &size, toread) != -1 ) {
    const char s[2] = " ";
    char *token;
    // First token is line number
    token = strtok(line, s);
    line_index = atoi(token);

    token = strtok(NULL, s); // Start word by word
    while( token != NULL ) {
      char clean[50];
      memset(clean,'\0',sizeof(clean));   // reset `clean`

      int j = 0;
      for (int i=0; token[i]!='\0'; i++) {
          // convertir mayusculas a minusculas
          if(token[i]>='A' && token[i]<='Z'){
            token[i] = token[i]+32;
          }
          // si es una letra, copiala a clean[j]
          if(token[i]>= 'a' && token[i]<='z'){
            clean[j]=token[i];
            j++;
            continue;
          } else {    // if next is non-alpahebtic and we've at least 1 char, stop
            if (j>=1) break;
          }
      }
    
      if (strcmp("", clean)!=0) {
        int positionA1  = traverseArchivo1(archivo1, clean); 
        if (positionA1 == -1) {
          // if string not empty, write to archivo1
          record1 r1;
          strcpy(r1.word, clean);
          r1.ptr = posA2;  
          fwrite(&r1, sizeof(record1), 1, archivo1);
          insertArray(&a, r1);
          // TO CREATE archivo2 ENTRIES:
          // 1. Take current line (line_index)
          // 2. take current position counter posA2
          // 3. write into archivo2 starting from posA2 <line_index,-1>
          record2 r2;
          r2.line = line_index;
          r2.next = -1;
          fseek(archivo2, sizeof(record2)*posA2, SEEK_SET);
          fwrite(&r2, sizeof(record2), 1, archivo2);
          
          // 4. increment posA2.
          posA2++;

        } else { // Duplicate word at positionA1 in archivo1
            
            // Access entry at positionA1 in archivo 1
            fseek(archivo1, sizeof(record1)*positionA1, SEEK_SET); 
            record1 r1;     
            fread(&r1, sizeof(record1), 1, archivo1);
            // Move to its pointer in A2
            fseek(archivo2, sizeof(record2)*r1.ptr, SEEK_SET);
            record2 r2;
            fread(&r2, sizeof(record2), 1, archivo2); // read in record r2

            // traverse linked list in A2
            int prevR2 = r1.ptr;      
            while (r2.next != -1) {   
              prevR2 = r2.next;  // last valid pointer in R2
              //printf("Lista invertida next: %d", r2.next);
              fseek(archivo2, sizeof(record2)*r2.next, SEEK_SET);
              fread(&r2, sizeof(record2), 1, archivo2); // reasignas r2
            } // stop when r2.next == -1;
            
            r2.next = posA2; // update `next` pointer in A2
            fseek(archivo2, sizeof(record2)*prevR2, SEEK_SET);
            fwrite(&r2, sizeof(record2), 1, archivo2);
            // Add new r2 entry
            record2 newR2;
            newR2.line = line_index;
            newR2.next = -1;
            fseek(archivo2, sizeof(record2)*posA2, SEEK_SET);
            fwrite(&newR2, sizeof(record2), 1, archivo2);
            posA2++;
        }
      }
     
      token = strtok(NULL, s); // next token

    } // end while for every line

  }
  //qsort(a.array, a.used, sizeof(record1), compareUser); 
  fseek(archivo1, 0, SEEK_SET);
  
  // write into A1
  for(int i=0; i<a.used; i++) {
    fwrite(&(a.array[i]), sizeof(record1), 1, archivo1);
  }

  // TEST print chunk 1 txt files
  if (0) {
    printFileOne(archivo1, 0);
    putchar('\n');
    printFile2(archivo2, 0);
  }

  // END PROCESSING
  fclose(archivo1);
  fclose(archivo2);
  freeArray(&a);
  fclose(toread);

}
