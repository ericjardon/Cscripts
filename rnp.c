#include <stdio.h>
// Implementation of a Stack

int stackSize = 150;
char stack[150];
int top = -1;   

int isEmpty() {
  if (top==-1) {
    return 1;
  }
  return 0;
}

int isFull() {
  if (top==stackSize) {
    return 1;
  }
  return 0;
}

char pop(){
  char sign;

  if(!isEmpty()){
    sign = stack[top];
    top = top - 1;
    return sign;
  } else {
    printf("Empty stack");
  }
}


int push(char sign){
  if(!isFull()) {
      top= top + 1;
      stack[top] = sign;
  } else {
    printf("Full stack");
  }
  return 0;
}


int readExpression(char exp[]) {
  int i = 0;
  while(exp[i] != '\0'){
    if (exp[i] == '+' || exp[i] == '-' || exp[i] =='*' || exp[i]=='/' || exp[i]=='^') {
      push(exp[i]);
    } 
    else if (exp[i] == ')') {
      printf("%c", pop());
    }
    else if (exp[i] != '('){
      printf("%c", exp[i]);
    }
    ++i;
  }

  putchar('\n');
  return 0;
}

int main(void) {
  int i =0;
  int t;
  int MAX_LIMIT = 400;
  char exp[MAX_LIMIT];

  scanf("%d",&t);
  while(i < t){
  scanf("%s", exp);
  //printf("%s\n", exp);
  readExpression(exp);
  ++i;
  }
  return 0;
}



/*
Example Input:
3
(a+(b*c))
((a+b)*(z+x))
((a+t)*((b+(a+c))^(c+d)))
---
Output: 
abc*+
ab+zx+*
at+bac++cd+^*
*/
