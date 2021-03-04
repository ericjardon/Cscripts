int ptrDemo(void) {
  int a = 20;
  int *ptr; /*Declare a pointer with asterisk */
  ptr = &a; /*ptr holds the mmy address of variable a*/
  char *ptr1;  /*another pointer, points to a char value*/
  printf("The value of a is %d \n", a);
  printf("The byte size of an int is %lu \n", sizeof(int));
  printf("The size of an int pointer is %lu. The size of a char pointer is %lu \n", sizeof(ptr), sizeof(ptr1));
  printf("The value that ptr points to is: %d \n", *ptr);
  *ptr = 123;
  printf("The new value of a is %d \n", a);
  printf("The address value of a is %p \n", &a);

  char c = 'e';
  char d = 'h';   /*Possible consecutive allocation*/
  ptr1 = &c;
  printf("Address of c is %p \n", ptr1);
  printf("Value of d is %c \n", *(ptr1-1));
  /*variable h is one memory position before c*/
  return 0;
}
