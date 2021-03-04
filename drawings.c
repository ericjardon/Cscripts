#include <stdio.h>
#include <stdlib.h>

int drawDiamond(int);
int drawBowtie(int);

int main(void) {
    int user = 1;
    while (user==1) {
        printf("Choose your drawing.\n");
        printf("type 'a' for Diamond, 'b' for Bowtie\n");
        char type;
        scanf("%c",&type);
        printf("Type the drawing height (odd integer): ");
        int floors;
        scanf("%d", &floors);
        putchar('\n');
        if (type=='a') {
            drawDiamond(floors);
        }
        if (type == 'b') {
            drawBowtie(floors);
        }
        putchar('\n');
        printf("Would you like to go again? 1 for yes, 0 for no: ");
        scanf("%d", &user);
    }
    printf("\nBye!\n");
    return 0;
}

int drawDiamond(int height) {
  int a = 1;
  int inc = 2;  
  if (height%2){
    for(int i=0; i<height; i++) {   // per row
      if (a==height){
        inc = -inc;
      }
      int blanks = (height-a) / 2;
      int c;
      for(c=0; c<blanks; c++) { // first blanks
        printf(" ");
      }
      for(c=0; c<a; c++) {
        printf("*");
      }
      for (c=0; c<blanks; c++) {
        printf(" ");
      }
      putchar('\n');
      a+=inc;
    }
  }
  return 0;
}

int drawBowtie(int height) {
  if (height%2) {
    int a = 1;
    int inc = 1;
    for (int i=0; i<height; i++) {
      int c;
      int blanks = height - a*2;  
      if (blanks<0) {
        for (c=0; c<height; c++) {
          printf("*");
        }
        inc = -inc;
      } else {
        for(c=0; c<a; c++) {
          printf("*");
        }
        for(c=0; c<blanks; c++) {
          printf(" ");
        }
        for(c=0; c<a; c++) {
          printf("*");
        }
      }
      putchar('\n');
      a+=inc;
    }
  }
  return 0;
}

