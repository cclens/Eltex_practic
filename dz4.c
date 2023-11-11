#include <stdio.h>

void printBinary(int a) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (a >> i) & 1);
    }
    printf("\n");
}

int main() {
    int a = 0;
    int b = 0;
    scanf("%d", &a);
    printBinary(a);
    printf("Выберите, заменить 3-й бит на 0 или 1: ");
    scanf("%d", &b); // 0 or 1
    
    if (b == 1) {
        a = (a | 4 );
    }
    if (b == 0) {
        a &= ~(1 << 2);
    }
    printf("%d\n", a); // измененное число после смены 3-го бита
    printBinary(a); // двоичное представление измененного числа
}
