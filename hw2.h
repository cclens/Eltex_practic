#include <stdio.h>

int main()
{
    int N = 0;
    scanf("%d", &N);
    int arr[N];
    for (int i = 1; i <= N; i++){
        arr[i] = i;
        printf("%d", arr[i]);
    }
    printf("\n");
    for (int j =0; j < N; j++){
        arr[j] = N -j;
        printf("%d", arr[j]);
    }
}
