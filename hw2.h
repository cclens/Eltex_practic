#include <stdio.h>

int main()
{
    int N = 0;
    scanf("%d", &N);
    int arr[N];
    for (int i = N; i >=1; i--){
        arr[i] = i;
        printf("%d", arr[i]);
    }
}