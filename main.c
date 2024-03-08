#include <stdio.h>
#include "graphics.h"
#include "navigation.h"
#include <stdlib.h>
#include <string.h>

int main(){
    int count = 1;
    int flag = 1;
    int index = 0;   

    char **filenames = (char **)malloc(100 * sizeof(char *));  
    filenames[0] = (char*) malloc(sizeof(char));
    strcpy(filenames[0], ".");// изначально работаем в текущей директории
    current_directory(&count, filenames, &index);
    draw_graphics(&count, filenames,  &flag);
   
    return 0;
}
