#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  srand(time(0));   // get a different seed everytime
  int target = rand() % 100; // number is between 0-99
  int tries = 0;
  int guess = -1;
  while (target != guess) {
    printf("Pick a number: \n");
    scanf("%d", &guess);
    tries++;
    if (guess>target) {
      printf("Too big\n");
    } else if (guess < target){
      printf("Too small\n");
    }
  }
  printf("You guessed the number %d in %d attempts", target, tries);

  return 0;
}