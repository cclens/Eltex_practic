#include <stdio.h>

int main() {
    int a = 0;
    int b = 0;
    printf("Введите число: ");
    scanf("%d", &a); 
    printf("Введите число, на которое вы хотите изменить третий байт: ");
    scanf("%d", &b);
    
    unsigned char* ptr = (unsigned char*)&a;
    ptr += 2;
    
    *ptr = (unsigned char)b;
    
    printf("%d\n", a);
    
    return 0;
}
