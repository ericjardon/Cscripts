#include <stdio.h>
void reverse();

// Program for reversing a string using LIFO logic.

int main() {
    printf("Enter a sentence: ");
    reverse();
    putchar(\n);
    return 0;
}

void reverse() {
    // Sequentially reads a character from the input and stores it.
    // When user hits enter, last character is \n and everything starts coming back from recursion.
    // last character to be entered is the first to be printed out, like a stack.
    char c;
    scanf("%c", &c);
    if (c != '\n') {
        reverse();
        printf("%c", c);
    }
}

int get_length(char str[]) {
    int i;
    for (i=0; str[i] != '\0'; ++i);
    printf("Length of string is %d", i);
    return i;
}