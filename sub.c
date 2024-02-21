float sub(float* buffer){
    float temp = 0;
    printf("%s\n", "введите число которое хотите отнять");
    scanf("%f", &temp);
    *buffer -= temp;
    printf("%f\n", *buffer);
    temp = 0;
}
