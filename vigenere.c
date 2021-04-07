#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 95

int c(int x, int k) {
    //printf("x: %d\t", x);
    x -= 32;
    char y = (x+k)%ALPHABET_SIZE;
    y += 32;    
    //printf("f(x): %d\n", y);
    return y;
}

int dc(int y, int k) {
    //printf("x: %d\t", x);
    y -= 32;
    char x =(y-k)%ALPHABET_SIZE;   
    if (x<0){  
        // manejo de casos negativos: en C el operador módulo respeta el signo
        x += ALPHABET_SIZE;
    }
    x += 32;
    //printf("x: %d\n", x);
    return x;
}

void vig_cipher(char* text, char* key){
    for (int i=0; text[i] != '\0'; i++) {
        text[i] = c(text[i], key[i]);
    }
}

void vig_decipher(char* text, char* key){
    for (int i=0; text[i] != '\0'; i++) {
        text[i] = dc(text[i], key[i]);
    }
}


void generateKey(char* text, char* keyword, char* key) {
    int len = strlen(keyword);
    for(int i =0; text[i] != '\0'; i++) {
        key[i] = keyword[i%len];
    }
}

int main() {
    printf("VIGENÈRE CIPHER\n");
    char TEXT[101];
    char* last;

    
    char keyword[9];   

    printf("Enter your message to cipher (100 chars MAX): ");
    if (fgets(TEXT, 101, stdin) != NULL) {
        // la función fgets podría incluir el caracter de newline, en ese caso lo quitamos
        if ((last = strchr(TEXT, '\n')) != NULL) {
            *last = '\0';
        };

        printf("\n Enter the keyword for the cipher (9 chars MAX): ");
        scanf("%s", keyword);
        printf("\nYour keyword '%s'\n", keyword);
        printf("Your message:\n'%s'\n", TEXT);
    };

    char key[101];
    generateKey(TEXT, keyword, key);
    printf("Key: %s\n", key);
    vig_cipher(TEXT, key);
    printf("Ciphered text:\n'%s'\n", TEXT);
    vig_decipher(TEXT, key);
    printf("Deciphered text:\n'%s'\n", TEXT);
    
    return 0; 
}

/*TEST CASES
    1) BBBBBBBB, ericeric
    2) ATTACKATDAWN, lemon 
    3) 7 elefantes, se columpiaban..., G
*/