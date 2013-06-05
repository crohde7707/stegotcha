#include <stdio.h>
#include <string.h>

int main() {
 
 printf("Hello World!\n");
 int x = 0, i = 0;
 for(i; i < 5; i++) {
     x = x + i;
 }
 printf("Value for x:%d\n", x);
 
 return 0;
}