#include <stdio.h>

int main() {
    int N = 0;
    scanf("%d", &N);
    int arr[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arr[i][j] = 0;
            
            if (i + j >= N - 1){
                arr[i][j] = 1;
            }
            printf("%d", arr[i][j]);
        }
        printf("\n");
    }

    return 0;
}