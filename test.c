#include <stdio.h>

int main(){

    FILE *file = fopen("test.txt", "a+");
    if (file == NULL){
        return 0;
    }
    fprintf (file, "String from file");
    
    fseek(file, 0, SEEK_END);
    int lenght = ftell(file);
    for (int i = 0; i <= lenght; i++){
        fseek(file, -i, SEEK_END);
        putchar(fgetc(file));
    }
    putchar ('\n'); 
    fclose (file);

    
    return 0;  
}
