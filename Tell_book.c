#include <stdio.h>
#include <string.h>

struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
};

void addAbonent(struct abonent* arr, int* count) {
    if (*count >= 100) {
        printf("Справочник переполнен\n");
        return;
    }
    
    printf("Введите имя: ");
    scanf("%s", arr[*count].name);
    
    printf("Введите фамилию: ");
    scanf("%s", arr[*count].second_name);
    
    printf("Введите номер телефона: ");
    scanf("%s", arr[*count].tel);
    
    (*count)++;
}

void deleteAbonent(struct abonent* arr, int* count, char* name) {
    int i;
    for (i = 0; i < *count; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            memset(&arr[i], 0, sizeof(struct abonent));
            printf("Абонент удален\n");
            return;
        }
    }
    
    printf("Абонент не найден\n");
}

void searchAbonent(struct abonent* arr, int count, char* name) {
    int i;
    int found = 0;
    for (i = 0; i < count; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            printf("Имя: %s\n", arr[i].name);
            printf("Фамилия: %s\n", arr[i].second_name);
            printf("Телефон: %s\n", arr[i].tel);
            found = 1;
        }
    }
    
    if (!found) {
        printf("Абонент не найден\n");
    }
}

void printAllAbonents(struct abonent* arr, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (strlen(arr[i].name) > 0) {
            printf("Имя: %s\n", arr[i].name);
            printf("Фамилия: %s\n", arr[i].second_name);
            printf("Телефон: %s\n", arr[i].tel);
            printf("\n");
        }
    }
}

int main() {
    int choice = 0;
    struct abonent arr[100];
    int count = 0;
    
    while (choice != 5) {
        printf("1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по имени\n4) Вывод всех записей\n5) Выход\n");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addAbonent(arr, &count);
                break;
            case 2:
                {
                    char name[10];
                    printf("Введите имя абонента для удаления: ");
                    scanf("%s", name);
                    deleteAbonent(arr, &count, name);
                }
                break;
            case 3:
                {
                    char name[10];
                    printf("Введите имя абонента для поиска: ");
                    scanf("%s", name);
                    searchAbonent(arr, count, name);
                }
                break;
            case 4:
                printAllAbonents(arr, count);
                break;
            case 5:
                break;
            default:
                printf("Некорректный выбор\n");
                break;
        }
    }
    
    return 0;
}