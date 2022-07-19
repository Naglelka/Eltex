#include <stdio.h>

int main(){

int a = 0xCCDDAABB;
int byteToChange;
int numToChange = a;

for (int i = 0; i < sizeof(a); i++){
    printf("byte %d = %x\n", i, (a & 0xFF));
    a = a >> 8;
}

printf("Enter byte number to replace:");
    scanf("%d", &byteToChange);

    switch(byteToChange)
    {
        case 1:
            numToChange^= 0xff000000;
            break;
        case 2:
            numToChange^= 0xff0000;
            break;
        case 3:
            numToChange^= 0xff00;
            break;
        case 4:
            numToChange^= 0xff;
            break;   
    }

    if(byteToChange >= 1 && byteToChange <= 4)
        printf("Change value = %x\n", numToChange);


    return 0;
}
