#include <stdio.h>

int main(){

int a = 0xCCDDAABB;
int b = a, c = a;
int z;

for (int i = 0; i < sizeof(a); i++){
    printf("byte %d = %x\n", i, (a & 0xFF));
    a = a >> 8;
}

scanf("%d", &z);
for(int i = 0; i < sizeof(a); i++){
    if(i ==2){
        printf("%x", z);
        c = c >> 8;
    }
    else {
        printf("%x", (c & 0xFF));
        c = c >> 8;
    }
}
}
