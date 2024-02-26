float mul(float* buffer){
    float temp = 0;
    printf("введите число на которое хотите умножить\n");
    scanf("%10f", &temp);
    
    *buffer *= temp;
    printf("%f\n", *buffer);
    temp = 0;
}

