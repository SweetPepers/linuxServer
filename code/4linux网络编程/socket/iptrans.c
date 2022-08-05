#include <stdio.h>

#include <stdlib.h>
#include <arpa/inet.h>


/*
#include <arpa/inet.h>
// p 点分十进制字符串,  n网络字节序整数
int inet_pton(int af, const char* src, void *dst);
  - parameters
    - af:地址族 AF_INET AF_INET6

const char* inet_ntop(int af, const void *src, char *dst, socklen_t size);
  - size : dst指向位置的大小
  - ret : 值与dst相同

*/

int main(){
  char buf[] = "192.9.9.3";
  unsigned int num = 0;
  inet_pton(AF_INET, buf, &num);
  
  unsigned char *p = (unsigned char *)&num;

  printf("%d %d %d %d\n", *p, *(p+1), *(p+2), *(p+3));

  // 网络字节序转字符串
  char ip[16] = "";
  const char * str = inet_ntop(AF_INET, &num, ip, 16);

  printf("str : %s\n", str);
  printf("ip : %s\n", ip);
  printf("ip == str : %d\n", ip == str);




}