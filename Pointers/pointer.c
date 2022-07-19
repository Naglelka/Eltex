#include <stdio.h>


int main(){

    int num = 270533154;
    int byteToChange;
    int numToChange;
    char *ptr;

    ptr = (char *) &num;

    for(int i = 0; i < sizeof(num); i++)
        {
            printf("%d byte = %x\n", i, *ptr);
            ptr++;
        }

    ptr = (char *) &num;

    printf("Enter byte number to replace:");
    scanf("%d", &byteToChange);
    printf("Enter a new byte value:");
    scanf("%d", &numToChange);

    ptr += byteToChange - 1;
    *ptr = numToChange;

    ptr = (char *) &num;

    for(int i = 0; i < sizeof(num); i++)
        {
            printf("%d byte = %x\n", i, *ptr);
            ptr++;
        }
}