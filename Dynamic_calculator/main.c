#include <stdlib.h>
#include <stdio.h>

int main()
{
    float* buffer = (float*)malloc(sizeof(float));
    int choice = 0;
    while (choice != 5){
        printf("%s\n", "выберите \n) Сложение\n) Вычитание\n) Умножение\n) Деление\n) Выход" );
        int sucessfulInput = scanf ("%5d", &choice);

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
