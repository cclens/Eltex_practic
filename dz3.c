#include <stdio.h>

void printBinary(int a) {
    int count = 0;
    for (int i = 31; i >= 0; i--) {
        if (((a >> i) & 1)){count += 1;}
        printf("%d", (a >> i) & 1);
    }
    printf("\n");
    printf("%d", count );

}

int main() {
    int a = 0;
    scanf("%d", &a);
    
    printBinary(a);
}
