#include <stdio.h>
#define N 4

int main(){

int a[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            a[i][j] = (i + j < N-1) ? 0 : 1;
            printf("%2d", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}
