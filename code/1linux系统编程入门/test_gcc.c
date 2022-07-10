#include <stdio.h>

#define PI 3.14

int add(int a, int b);


int main(){
  int sum = PI + 10;
  #ifdef MYDEFINE
  printf("gcc -D\n");
  #endif
  printf("hello wrold\n");
  add(1, 2);
  return 0;
}