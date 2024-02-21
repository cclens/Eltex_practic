#include <stdlib.h>
#include <stdio.h>

float buffer = 0;

int main()
{
    int choice = 0; 
    while (choice != 5){
        printf("%s\n", "выберите \n1) Сложение\n2) Вычитание\n3) Умножение\n4) Деление\n5) Выход" );
        int sucessfulInput = scanf ("%15d", &choice);
        
        if (sucessfulInput == 1){
        switch(choice){
        
            case 1:
                add(&buffer);
                break;
            case 2:
                sub(&buffer);
                break;
            case 3:
                mul(&buffer);
                break;
            case 4:
                division(&buffer);
                break;
            case 5:
                exit;
                break;
            }
        }
        else{
        printf("вы ввели не число\n");}
        while (getchar() != '\n'); // Очистить буфер ввода
    }
}
