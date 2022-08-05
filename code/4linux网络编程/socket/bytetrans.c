#include <stdio.h>

#include <stdlib.h>
#include <arpa/inet.h>


int main(){
  unsigned int a = 0x12345678;
  unsigned int b = htonl(a);
  unsigned int c = ntohl(a);

  printf("a = %x, b = %x, c = %x\n", a, b, c);
  
  return 0;
}