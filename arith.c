#include <stdio.h>
#include<stdlib.h>
#include <string.h>

int solve (char []);

int printSumaResta(int a[], int b[], char res[], int lenA, int lenB, int lenR, int maxLen, char op) {
  int line = lenB+1 > lenR ? lenB+1 : lenR;
  int blanks = maxLen - lenA;
  for (int i=0; i<blanks; i++) {
    printf(" ");
  }
  for (int d=0; d<lenA; d++) {
    printf("%d", a[d]);
  }
  printf("\n");
  blanks = maxLen - (lenB+1);
  for (int i=0; i<blanks; i++) {
    printf(" ");
  }
  putchar(op);
  for (int d=0; d<lenB; d++) {
    printf("%d", b[d]);
  }
  printf("\n");
  for (int i = 0;i<maxLen-line; i++) {
    printf(" ");
  }
  for (int i=0; i<line; i++) {
    printf("-");
  }

  printf("\n");
  blanks = maxLen - lenR;
  for (int i=0; i<blanks; i++) {
    printf(" ");
  }
  int k = lenR-1;
  while (k >= 0 ) {  
    printf("%c", res[k]);
    k--;
  } // res contiene el resultado escrito en reversa
  printf("\n");
  return 0;
}


int suma(int a[], int b[], int lenA, int lenB) {
  //printf("Entra suma\n");
  int maxLen = lenB+1; 

  char res[501];  // arreglo para el resultado
  int carry=0;
  int i = lenA - 1;   // indice op1
  int j = lenB - 1;   // indice op2
  int k = 0; 
  // 99+1
  while (i >= 0 && j >= 0) {
    int s = a[i] + b[j] + carry;
    if(s < 10){
      res[k] = s+'0'; /*convierte de int a char*/
      carry = 0;
    } else {
      res[k] = s%10 + '0';
      carry = 1;
    }
    i--; j--; k++;
  }
  while (i>=0 && carry==1) { // i:0 val:9 carry=1
  // 9 _ 1 --> 0, + 1 carry
    int s = carry + a[i]; // 9 + 1 = 10 --> :
    if (a[i] == 9) {
      res[k] = '0';
      carry = 1;
    } else {
      res[k] = s+'0'; /*convierte de int a char*/
      carry = 0;
    }
    i--; k++;
  }
  while (j>=0 && carry==1) { // i:0 val:9 carry=1
  // 9 _ 1 --> 0, + 1 carry
    int s = carry + b[j]; // 9 + 1 = 10 --> :
    if (b[j] == 9) {
      res[k] = '0';
      carry = 1;
    } else {
      res[k] = s+'0'; /*convierte de int a char*/
      carry = 0;
    }
    j--; k++;
  }
  // copia digitos restantes
  while (i >= 0) {
    res[k] = a[i] + '0';
    i--; k++;
  }
  while (j >= 0) {
    res[k] = b[j] + '0';
    j--; k++;
  }
  // -1
  if ( carry == 1) {
    res[k] = carry + '0';
    k++;
  }
  // k ya tiene la longitud de res?
  int lenR = k;
  //printf("\nLenR (k): %d", lenR);
  maxLen = maxLen > lenR ? maxLen : lenR;
 // printf("\nmaxLen: %d\n", maxLen);
  printSumaResta(a, b, res, lenA, lenB, lenR, maxLen, '+');
  /*while (k>=0) {
    printf("%c", res[k]);
    k--;
  }*/
  //putchar('\n');
  return 0;  
}

int resta(int a[], int b[], int lenA, int lenB) {
  //printf("Entra resta\n");
  int maxLen = lenB+1; /* max horizontal space needed */
  char res[501];
  int carry=0;
  int i = lenA - 1;   // indice op1
  int j = lenB - 1;   // indice op2
  int k = 0; 
  while (i >= 0 && j >= 0) {  // 
    int r = a[i] - b[j] - carry;
    if(r < 0){    // opA < opB+carry
      r = a[i]-carry + 10 - b[j]; 
      res[k] = r+'0'; /*convierte de int a char*/
      carry = 1;
    } else {
      res[k] = r + '0';
      carry = 0;
    }
    i--; j--; k++;
  }

  while (i>=0) {
    int r = a[i] - carry;
    if(r < 0){    // opA < opB+carry
      r = a[i] -carry + 10; 
      res[k] = r+'0'; /*convierte de int a char*/
      carry = 1;
    } else {
      res[k] = r + '0';
      carry = 0;
    }
    i--; k++;
  }

  maxLen = maxLen > k ? maxLen : k;

  int lenR = k;
  k--;    // el último indice valido de res
  while (res[k] == '0' &&  lenR>1) {
    k--;
    lenR--;
  }

  printSumaResta(a, b, res, lenA, lenB, lenR, maxLen, '-');
  return 0;  
}




/////////////////////////main 2//////////////////
int secondLine = 0;
int firstLine = 0;

int printMulti(int a[], int b[], int lenA, int lenB, int partials[][lenA+1], int* final_res) {
  // determinar lenR de res_final 
  int d=lenA+lenB-1;
  while(d>0 && final_res[d] == 0 ){
      d--;
  }
  // determinar len de primer res parcial
  int *primer_partial = (int*)&partials[0][0];
  int k = lenA; // every partial array len is lenA+1;
  while(primer_partial[k] == -1) {
    k--;
  }
  while (k>0 && primer_partial[k] == 0) {
    k--;
  }
  firstLine = firstLine > k+1 ? firstLine : k+1;

  secondLine = d+1;  // secondLine tiene misma longitud que final
  int maxLen = firstLine > secondLine ? firstLine : secondLine; // espacios horizontales
  // 1) Imprimir a
  int blanks = maxLen - lenA;
  for(int i = 0 ; i<blanks ; i++){
    printf(" ");
  }
  for(int i=0; i<lenA; i++){
    printf("%d", a[i]);
  }
  printf("\n");
    // 2) Imprimir * y b
  blanks = maxLen - (lenB + 1);
   for(int i = 0 ; i<blanks ; i++){
    printf(" ");
  }
  putchar('*');
  for (int i=0; i<lenB; i++) {
    printf("%d", b[i]);
  }
  putchar('\n');
  
  if (lenB>1) {
    // imprimir firstLine
    blanks = maxLen - firstLine;
    for(int i = 0 ; i<blanks ; i++){
      printf(" ");
    }
    for (int i=0; i<firstLine; i++) {
      printf("-");
    }
    putchar('\n');
    // 3) Imprimir todos los resultados parciales
    for (int z=0; z<lenB; z++) {
      int *partial_res = (int*)&partials[z][0];
      int k = lenA; // every partial array len is lenA+1;
      while(partial_res[k] == -1) {
        k--;
      }
      // check leading zeroes
      while(k>0 && partial_res[k]==0) {
        k--;
      }
      // print the valid digits of partial_res
      int digits = k+1;
      blanks = (maxLen - z) - digits;
      for (int i=0; i<blanks; i++) {
        printf(" ");
      }
      while (k>=0) {
        printf("%d", partial_res[k]);
        k--;
      }
      putchar('\n');
    }
  }

  // 4) Imprimir segunda linea y el resultado final
  blanks = maxLen - secondLine;
  for (int i=0; i<blanks; i++) {
    printf(" ");
  }
  for (int i=0; i<secondLine; i++) {
    printf("-");
  }
  putchar('\n');

  blanks = maxLen - (d+1);
  for (int i=0; i<blanks; i++) {
    printf(" ");
  }
  while (d>=0) {
    printf("%d", final_res[d]);
    d--;
  }
  putchar('\n');
  return 0;
}

int multiSimple(int a[], int lenA, int b, int z, int* partial, int* final_res) {
  // zeros es la posicion de este resultado parcial, indica potencia de 10
  // partial es un apuntador al arreglo en la posicion z de partials
  int res[lenA+1];  // arreglo auxiliar tiene a lo más, los dígitos de a + 1
  int carry = 0;
  int i = lenA-1;   // indice para recorrer a[] de atrás para el frente
  int k = 0;    // indice de res parcial

  while( i >= 0 ){
    int product = a[i]*b + carry;
    if(product>9){
      int d = product%10; 
      carry = product/10; 
      res[k] = d;
    } else {
      res[k] = product;
      carry = 0;
    } 
    partial[k] = res[k];
    final_res[z] += res[k];   // add resulting digit to final_res result in zth position
    i--;
    k++;
    z++;  
  }

  // TERMINAN DIGITOS DE a
  while (carry > 0) {
    res[k] = carry%10;  // para que nunca exceda 10
    partial[k] = res[k];
    final_res[z] += res[k];
    carry /= 10;
    k++;
    z++;
  }  

  return 0;
}

int multi(int a[], int lenA, int b[], int lenB) {
  int partials[lenB][lenA+1];  // construct the array for storing partial results
  int final_res[lenA+lenB];    // construct the array for the final result
  memset(final_res,0,sizeof(final_res));    // initialize vals to 0

  firstLine = lenB + 1;
  secondLine = 0;
  for (int i = lenB-1; i>=0; i--) {
    // multiply array a[] for each digit of b, traverse from right to left
    int _b = b[i];
    int z = lenB-1-i; // number of zeroes at the right of result
    int *partial_res = (int*)&partials[z][0];    // current partial result array, length is lenA+1
    memset(partial_res, -1, (lenA+1)*sizeof(int));
    multiSimple(a, lenA, _b, z, partial_res, final_res);
  }

  // Perform necessary carries on the final_res array 
  int carry = 0;
  // int i = 0;
  for(int i=0; i<lenA+lenB; i++) {
    final_res[i] += carry;
    if (final_res[i] < 10){
      carry = 0;
    }
    else{
      carry = final_res[i] / 10;
      final_res[i] %= 10;   // final-res[i] = final_res[i] % 10;   keep the least sig. digit
    }
  }

  printMulti(a, b, lenA, lenB, partials, final_res);
  return 0;
}


//////////////////////TERMINA main2/////////////////////

int performOperation(int op1[], int op2[], int c1, int c2, char oper) {
  switch (oper) {
    case '+':
      suma(op1, op2, c1, c2);
      break;
    case '-':
      resta(op1, op2, c1, c2);
      break;
      case '*':
      multi(op1, c1, op2, c2);
      break;
    default:
      printf("Nulo operador\n");
      break;
  }
  return 0;
}

int solve(char exp[]) {
  int op1[500];
  int op2[500];
  
  int i = 0;
  // Read operand 1
  while 
  (exp[i] != '+' && exp[i] != '-' && exp[i] != '*') {
    op1[i] = exp[i]-'0';  // insert ith digit 
    i++;
  }
  // Read operator sign
  int charCounter1 = i;
  char oper = exp[i];
  i++;
  int charCounter2 = 0;

  // Read operand 2
  while(exp[i] != '\0'){
    op2[charCounter2] = exp[i] - '0';
    i++;
    charCounter2++;
  }
  performOperation(op1, op2, charCounter1, charCounter2, oper);
  return 0;
} 

int main(void) {

  int testcases;
  char exp[1001];
  scanf("%d",&testcases);  /*no. de testcases*/

  for(int t=0; t<testcases; t++){
  scanf("%s", exp);
  solve(exp);
  if (t<testcases-1) printf("\n");
  }
  return 0;  
}