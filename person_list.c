#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// https://experiencia21.tec.mx/courses/110335/assignments/3813493
typedef struct person {
  char name[50];
  int age;
  struct person *next; 
} person;

/*Implement CRUD for a list of people ordered alphabetically by their names*/
// x -> y ;  to access the property of a pointer x

void insert(person *per, person** head) {
  /*3 outcomes. insert head, insert middle, insert last*/
  if(*head == NULL) {  
    *head = per;    
  return; 
  }

  if (strcmp(per->name, (*head)->name) < 0) { // per is smaller than head
    per->next = *head;
    // current's next is the head.
    *head = per;
    // current becomes the head.
    return;
  }   // if not, current name is at least larger than head.

  // Traverse the linked list two by two
  person *p1 = *head;   // p1 is a pointer
  person *p2 = p1->next;    // p2 is a pointer

  while (p2 != NULL && strcmp(per->name, p2->name) > 0) {
    // while current's name is larger than p2
    p1 = p2;
    p2 = p2->next;
  } // a) se detiene cuando p2 == null (llegó al final) o b) que per.name < p2.name

  if (p2 == NULL) {
    // llegamos al final
    p1->next = per;
  } else {
    // per <= p2 ,  sandwich entre p1 y p2
    p1->next = per;
    per->next = p2;
  }
 }

// Delete by name. Use free after deletion.
int deleteByName(char name[], person **head) {
    // head is a double pointer, why?

    // Edge Case. Empty list.
    if (*head == NULL) {
        printf("LISTA ESTÁ VACÍA\n");
        return 0;
    }
    
    person* p1 = *head;
    person* p2 = p1->next;
  
    // Case 1. Delete head.
    if (strcmp(name, p1->name)==0) {
        *head = p2;
        free(p1);
        return 1;
    }

    while (p2 != NULL) {
        if (strcmp(name, p2->name)==0) {
            break;
        }
        p1 = p2;
        p2 = p2->next;
    }

    if (p2==NULL) {
        // reached end of list
        printf("PERSONA NO ENCONTRADA\n");
        return 0;
    }

    if (p2->next != NULL) {
        // 2. Delete middle element
        p1->next = p2->next;
        free(p2);
    } else {
        // 3. Delete last element
        p1->next = NULL;
        free(p2);
    }

    return 1;
}

int updateAge(char name[], int age, person **head) {
  person* p = *head;
  while(p != NULL && strcmp(name, p->name) != 0) {
    p = p->next;
  }
  if (p==NULL) {
    printf("PERSONA NO ENCONTRADA\n");
    return 0;
  }
  // else, p->name equals given name
  p->age = age;
  return 1;
}

void printList(person *head) {
  person *temp = head;
  if (temp==NULL) {
    printf("Lista vacía\n");
  }
  while(temp != NULL) {
    if (temp->next != NULL) {
        printf("Name: %s, Age: %d =>\n ", temp->name, temp->age);
    } else {
        printf("Name: %s, Age: %d END\n", temp->name, temp->age);
    }

    temp = temp->next;
  }
}

int demo(void) {
  person * raiz = NULL;  
  person *per;  
  per = (person *) malloc(sizeof(person));  
  strcpy(per->name, "Beto");  
  per->age = 25;  
  per->next = NULL;  
  insert(per, &raiz);  
  per = (person *) malloc(sizeof(person));  
  strcpy(per->name, "Ana");  
  per->age = 20;
  per->next = NULL; 
  insert(per, &raiz); 
  per = (person *) malloc(sizeof(person)); 
  strcpy(per->name, "Zara");    
  per->age = 18;       
  per->next = NULL; 
  insert(per, &raiz);  
  per = (person *) malloc(sizeof(person)); 
  strcpy(per->name, "Antonio");
  per->age = 18;
  per->next = NULL; 
  insert(per, &raiz);
  per = (person *) malloc(sizeof(person));
  strcpy(per->name, "Carla");
  per->age = 15; 
  per->next = NULL; 
  insert(per, &raiz);
  printList(raiz); 
  printf("PRUEBA BORRADO\n");
  char n[50] = {};
  int a;
  printf("Nombre a actualizar: ");
  scanf("%s", n);
  printf("\nEdad nueva: ");
  scanf("%d", &a);
  printf("Actualizando a %s", n);
  updateAge(n, a, &raiz);
  printf("Nombre a borrar: ");
  printList(raiz);
  scanf("%s", n);
  printf("Borrando %s \n", n);
  deleteByName(n, &raiz);
  printList(raiz);
  return 0;
}

int main(void) {
  printf("CRUD: LISTA LIGADA DE PERSONAS");
  printf("\n\n");
  int user = 1;
  char sel;
  person * head = NULL;
  printf("Presiona 'c' para insertar, 'u' para actualizar una edad, 'd' para borrar, 'r' para imprimir la lista, 0 para salir\n");
  printf("Selección: ");

  while (1) {
    scanf("%c", &sel);
    if (sel == '\n') {
      printf("Presiona 'c' para insertar, 'u' para actualizar una edad, 'd' para borrar, 'r' para imprimir la lista, 0 para salir\n");
      printf("Selección: ");
      continue;
    }
    if (sel=='0') {
      break;
    }
    // CREAR
    if (sel=='c'){
      char nombre[50];
      int edad;
      printf("\nEscribe el nombre a insertar: ");
      scanf("%s", nombre);
      printf("\n Escribe la edad: ");
      scanf("%d", &edad);
      person *per = (person *) malloc(sizeof(person));
      strcpy(per->name, nombre);
      per->age = edad;
      per->next = NULL;
      insert(per, &head);
      printf("\nSe insertó 1 nueva persona.\n");
      continue;
    }
    // BORRAR
    if (sel=='d') {
      char nombre[50];
      printf("\nEscribe el nombre a borrar: ");
      scanf("%s", nombre);
      if (deleteByName(nombre, &head)) {
        printf("\nSe borró 1 persona.\n");
      }
      continue;
    }
    if (sel=='u'){
      char nombre[50];
      int edad;
      printf("\nEscribe el nombre a actualizar: ");
      scanf("%s", nombre);
      printf("\n Escribe la nueva edad: ");
      scanf("%d", &edad);
      if (updateAge(nombre, edad, &head)){
        printf("\nSe actualizó 1 persona.\n");
      }
      continue;
    }
    if (sel=='r') {
      printf("IMPRIMIENDO LISTA:\n");
      printList(head);
      continue;
    } else {
      printf("Selección inválida\n");
    }
  }
  printf("Bye\n");
  return 0;
}