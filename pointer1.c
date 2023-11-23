#include <stdio.h>

int main()
{
    int a = 0;
    int b = 0;
    printf("введите число ");
    scanf("%d", &a); 
    printf("введите число, на которые вы хотите изменить 3й байт ");
    scanf("%d", &b);
    char* ptr;
    
    ptr = &a ;
    ptr += 2;
    
   
    *ptr = b; 
    
    printf("%d", a); 
    
}