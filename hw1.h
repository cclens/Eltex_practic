#include <stdio.h>

int main() {
    int N = 0;
    scanf("%d", &N);
    int arr[N][N];
    int count = 1;

    // Заполнение и вывод матрицы
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arr[i][j] = count++;
            printf("%4d", arr[i][j]);
        }
        printf("\n");
    }

    return 0;
}