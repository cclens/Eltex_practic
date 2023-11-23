#include <stdio.h>
#include <string.h>

int main()
{
    char str[100]; // строка 
    char substr[100]; // подстрока
    fgets(str, 100, stdin);
    fgets(substr, 100, stdin);
    int length = strlen(str); 
    int sublength = strlen(substr);
    
    char* ptr = str;
    char* subptr = substr;
    
    int found = 0; // флаг о нахождении подстроки в строке
    int position = 0; 
    for (int i = 0; i < length; i++){
        if (str[i] == '\n' || str[i] == '\0'){  // "обрезаем" у строк всё лишнее
            break;
        }
        
        if (*ptr == *subptr){
            char* tempPtr = ptr;
            char* tempSubptr = subptr;
            int j;
            for (j = 1; j < sublength; j++){
                if (*tempPtr != *tempSubptr){
                    break;  // Если символы не совпадают, прерываем цикл
                }
                tempPtr++;
                tempSubptr++;
            }
            if (j == sublength){  // Если дошли до конца подстроки, значит подстрока найдена
                found = 1;
                position = i;
                break;
            }
        }
        ptr++;
    }
s
    if (found){
        printf("Подстрока найдена по адресу: %p\n", ptr); // адрес ячейки памяти, с началом подстроки
        printf("Подстрока найдена по адресу: %c\n", *ptr);// символ начала подстроки
        printf("Подстрока найдена по адресу: %d\n", position);// порядок ячейки памятик пачала подстроки 

    } else {
        printf("Подстрока не найдена\n");
        *ptr =  '\0';
        *subptr =  '\0';
    }
    
    return 0;
}