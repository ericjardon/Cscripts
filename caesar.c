#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int key;
#define ALPHABET_SIZE 95 // upper limit to ascii table: 127-32

/*Valid ASCII codes go from 32 to 126*/
int c(int x) {
    //printf("x: %d\t", x);
    x -= 32;
    char y = (x+key)%ALPHABET_SIZE;
    y += 32;
    //printf("f(x): %d\n", y);
    return y;
}

int dc(int y) {
    //printf("f(x): %d\t", y);
    y -= 32;
    char x =(y-key)%ALPHABET_SIZE;   
    if (x<0){  
        // en C el operador módulo respeta el signo, manejo de casos negativos
        x += ALPHABET_SIZE;
    }
    x += 32;
    //printf("x: %d\n", x);
    return x;
}

void cipher(char* text) {
    for (int i=0; text[i] != '\0'; i++) {
        text[i] = c(text[i]);
    }
}

void decipher(char* text) {
    for (int i=0; text[i] != '\0'; i++) {
        text[i] = dc(text[i]);
    }
}

int main() {
    // menú de consola, provee una llave y un nombre de archivo
    printf("CEASAR CIPHER\n");
    char TEXT[101];
    char* last;

    printf("Enter your message to cipher (100 chars MAX): ");
    if (fgets(TEXT, 101, stdin) != NULL) {
        if ((last = strchr(TEXT, '\n')) != NULL) {
            *last = '\0';       // reassign pointer's value
        };

        printf("\n Enter the integer key for the cipher: ");
        scanf("%d", &key);
        printf("\nYour key %d\n", key);
        printf("Your message:\n'%s'\n", TEXT);
    };
    // fgets function may include the newline character so we remove it


    cipher(TEXT);
    printf("Ciphered text:\n'%s'\n", TEXT);
    decipher(TEXT);
    printf("Deciphered text:\n'%s'\n", TEXT);
}


