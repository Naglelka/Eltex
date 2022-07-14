#include <stdio.h>
#define N 5

int main(){

    int a [N][N];
    int c = 0;
    int d = N;
    int i,j;

    for(int p = 0; p < N/2; p++)
    {
        for(int k = 0; k < (d-1)*4; k++)
        {
            switch (k / (d-1))
                {
                case 0:
                    i = 0;
                    j = k % (d-1);
                    break;
                case 1:
                    i = k % (d-1);
                    j = d-1;
                    break;
                case 2:
                    i = d-1;
                    j = d-1-(k % (d-1));
                    break;
                case 3:
                    i = d-1-(k % (d-1));
                    j = 0;
                    break;
                }
            a[p+i][p+j] = c + k + 1;
        }
        c += (d-1)*4;
        d -= 2;
    }
    if(N % 2 != 0)
        a[N/2][N/2] = N*N;
    
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            printf("%d\t",a[i][j]);
        }
        printf("\n");
    }

}