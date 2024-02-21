#include "common.h"

void mul (float** buffer){
      float* tmp = (float*)malloc(sizeof(float));
      printf("%s\n", "введите число на которое хотите умножить");
      scanf("%f", tmp);
      if (tmp == NULL){
          printf("ошибка выделения памяти\n");
          return;
      }
      
     **buffer *= *tmp;
      printf("%f\n", **buffer);
      free(tmp);
  }
