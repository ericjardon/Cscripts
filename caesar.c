#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int key;                 // variable global de la llave ingresada por usuario
#define ALPHABET_SIZE 95 // número de códigos ASCII válidos de caracteres. Del 32 al 126

/* Códigos ASCII válidos van del 32 al 126. 
    Antes de encriptar o desencriptar, debemos 'normalizar'
    el código ASCII al quitar el desfase de 32.
*/

// Función para encriptar un solo codigo ascii. 
int c(int x) {
    //printf("x: %d\t", x);
    x -= 32;
    char y = (x+key)%ALPHABET_SIZE;
    y += 32;    
    //printf("f(x): %d\n", y);
    return y;
}

// Funcion para desencriptar un solo codigo ascii.
int dc(int y) {
    //printf("f(x): %d\t", y);
    y -= 32;
    char x =(y-key)%ALPHABET_SIZE;   
    if (x<0){  
        // manejo de casos negativos: en C el operador módulo respeta el signo
        x += ALPHABET_SIZE;
    }
    x += 32;
    //printf("x: %d\n", x);
    return x;
}

// Función para recorrer el texto y encriptarlo
void cipher(char* text) {
    for (int i=0; text[i] != '\0'; i++) {
        text[i] = c(text[i]);
    }
}

// Función para recorrer el texto y desencriptarlo
void decipher(char* text) {
    for (int i=0; text[i] != '\0'; i++) {
        text[i] = dc(text[i]);
    }
}

// Menú de consola, recibe un mensaje y una llave (número entero).
int main() {
    printf("CEASAR CIPHER\n");
    char TEXT[101];
    char* last;

    printf("Enter your message to cipher (100 chars MAX): ");
    if (fgets(TEXT, 101, stdin) != NULL) {
        // la función fgets podría incluir el caracter de newline, en ese caso lo quitamos
        if ((last = strchr(TEXT, '\n')) != NULL) {
            *last = '\0';       // reassign pointer's value
        };

        printf("\n Enter the integer key for the cipher: ");
        scanf("%d", &key);
        while (key%ALPHABET_SIZE==0) {
            printf("This key is invalid. Please choose a different key: ");
            scanf("%d", &key);

        }
        printf("\nYour key %d\n", key);
        printf("Your message:\n'%s'\n", TEXT);
    };
    cipher(TEXT);
    printf("Ciphered text:\n'%s'\n", TEXT);
    decipher(TEXT);
    printf("Deciphered text:\n'%s'\n", TEXT);
    
    return 0; 
}


/* TESTCASES 
    1) DOG, 1
    2) ERIC JARDON CHAO!, 32
    3) Erase una vez una chica, en 1993 el mundo era plano?, 400
*/