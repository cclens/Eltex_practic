float division(float* buffer){
    float temp = 0;
    printf("введите число на которое хотите разделить\n");
    scanf("%10f", &temp);
    if (temp == 0){
        printf("на ноль нельзя\n");
        exit(1);
    }
    
    *buffer = (*buffer)/temp;
    printf("%f\n", *buffer);
    temp = 0;
}
