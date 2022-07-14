#include <stdio.h>
#define N 4

int main(){

    int a[N];
    int b = 0;
    int c = a[N];

    for(int i = 0; i < N; i++){
        scanf("%d", &a[i]);
    }

    for(int i = 0; i < N/2; i++){
            b = a[i];
            a[i] = a[N-i-1];
            a[N-i-1] = b;
    }
    for (int i = 0; i < N; i++){
            printf("%d", a[i]);
    }
    return 0;
}