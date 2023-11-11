#include <stdio.h>

void printBinary(int a) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (a >> i) & 1);
    }
    printf("\n");
}

int main() {
    int a = 0;
    scanf("%d", &a);
    
    printBinary(a);
}
