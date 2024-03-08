#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
void current_directory(int* count, char **filenames, int* index){
   DIR *dir = opendir(filenames[*index]); //открываем директорию в текущем рабочем каталоге[index]
    if (chdir(filenames[*index]) != 0) {// обработка ошибок
        perror("chdir");
        closedir(dir);
        *index = 0;
        return;
    }
    if (dir == NULL){ // обработка ошибок
        perror(".");
        return;
    }
   
    for (int j = 0; j < *count; j++){// очищаем массив filenames, перед тем как заного его заполнить
        free(filenames[j]);
    }
    *count = 0;

    struct dirent *cur_dir;
    while ((cur_dir = readdir(dir)) != NULL){ // заполнение
    filenames[*count] = (char*)malloc((strlen(cur_dir->d_name) + 1) * sizeof(char));
        strcpy(filenames[*count], (cur_dir -> d_name));
        *count += 1;
    }
    closedir(dir);   
}
