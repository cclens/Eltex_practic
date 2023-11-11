#include <stdio.h>

int main() {
    int N;
    printf("Введите размер квадратной матрицы: ");
    scanf("%d", &N);

    int matrix[100][100];
    int num = 1;
    int upStart = 0, rtEnd = N - 1;
    int UpStart = 0, UpEnd = N - 1;

    while (num <= N * N) {
        // Вывод верхней строки
        for (int i = UpStart; i <= UpEnd; i++) {
            matrix[upStart][i] = num++;
        }
        upStart++;

        // Вывод правого столбца
        for (int i = upStart; i <= rtEnd; i++) {
            matrix[i][UpEnd] = num++;
        }
        UpEnd--;

        // Вывод нижней строки
        for (int i = UpEnd; i >= UpStart; i--) {
            matrix[rtEnd][i] = num++;
        }
        rtEnd--;

        // Вывод левого столбца
        for (int i = rtEnd; i >= upStart; i--) {
            matrix[i][UpStart] = num++;
        }
        UpStart++;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}