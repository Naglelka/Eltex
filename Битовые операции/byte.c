#include <stdio.h>

int main(){

int a = 0xCCDDAABB;
int b = a, c = a;

for (int i = 0; i < sizeof(a); i++){
    printf("byte %d = %x\n", i, (a & 0xFF));
    a = a >> 8;
}

c^= 0x11 << 16;
printf("before = %x, after = %x\n", b, c);

return 0;
}