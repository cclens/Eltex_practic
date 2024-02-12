#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
};

void add_abbonent(int* count, struct abonent** arr) { // используем двойной указатель
    if (*count >= 100) {
        printf("Справочник переполнен\n");
        return;
    }
    *count += 1; // Увеличение размера массива на 1
    *arr = (struct abonent*) realloc(*arr, (*count) * sizeof(struct abonent)); // пересоздаем массив
    
    if (*arr == NULL) {
        printf("Ошибка выделения памяти\n");
        return;
    }
    
    printf("Введите имя: ");
    scanf("%s", (*arr)[*count - 1].name);
    
    printf("Введите фамилию: ");
    scanf("%s", (*arr)[*count - 1].second_name);
    
    printf("Введите номер телефона: ");
    scanf("%s", (*arr)[*count - 1].tel);
}

void delete_abonent(int* count, struct abonent** arr){
    if (*count < 1){
        return;
    }
    int number = 0; 
    
    printf("введите номер который надо удалить\n");
    scanf("%d", &number);
    
    if ((number < 0 ) || (number > *count)){
        return;
    }

    for (int i = number - 1; i <= *count - 1; i++){
        strcpy((*arr)[i].name, (*arr)[i+1].name);
        strcpy((*arr)[i].second_name, (*arr)[i+1].second_name);
        strcpy((*arr)[i].tel, (*arr)[i+1].tel);
    }
    *count -= 1;
    *arr = (struct abonent*) realloc(*arr, (*count) * sizeof(struct abonent));
}

void search_abonent(int* count, struct abonent** arr){
    char search_name[10];
    int flag = 0;
    printf("%s", "введите имя пользователя которого хотите найти: ");
    scanf("%s", search_name);
    
    for (int j = 0; j <= ((*count) - 1); j++){
        if (( strcmp(search_name, (*arr)[j].name) == 0)){ 
            flag = 1;
            printf("%s%d\n", "абонент найден под номером ", *count);
        }
    }
    if (!flag){
        printf("%s\n", "абонент не найден");
    }
}

void all_abonent(int* count, struct abonent** arr){
    for (int i = 1; i <= *count; i += 1){
        printf ("%d\n", i);
        printf ("%s%s\n", "имя: ", (*arr)[i-1].name);
        printf ("%s%s\n", "фамилия: ", (*arr)[i-1].second_name);
        printf ("%s %s\n", "номер: ", (*arr)[i-1].tel);
        
    }
}

void exit_guide(){
    exit;
}

int main() {
    int choice = 0; 
    struct abonent* arr = (struct abonent*) malloc(0); // создаем пустой динамический массив структур
    if (arr == NULL){
        exit; //
    }
    int count = 0;
    
    while (choice != 5){
        printf("1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по имени\n4) Вывод всех записей\n5) Выход\n");
        scanf("%d", &choice);
        switch (choice){
            case 1:
                add_abbonent(&count, &arr);
                break;
            case 2:
                delete_abonent(&count, &arr);
                break;
            case 3:
                search_abonent(&count, &arr);
                break;
            case 4:
                all_abonent(&count, &arr);
                break;
            case 5:
                exit_guide();
            default:
                continue;
                break;
        }
    }
}
