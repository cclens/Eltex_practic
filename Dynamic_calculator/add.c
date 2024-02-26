#include "common.h"

void add(float** buffer){
      float* tmp = (float*)malloc(sizeof(float));
      printf("%s\n", "введите число которое хотите прибавить");
      scanf("%10f", tmp);
      if (tmp == NULL){
          printf("ошибка выделения памяти\n");
          return;
      }
      
     **buffer += *tmp;
      printf("%f\n", **buffer);
      free(tmp);
  }
