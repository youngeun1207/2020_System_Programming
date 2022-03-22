//Template
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

void bubbleSort(int* arr,  int num){
    int tmp;
    for(int i = 0; i < num; i++){
        for(int j = 0; j < num-1-i; j++){
            if(arr[j] > arr[j+1]){
                tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

int main(){
    int n_array[SIZE] = {2,-9, 10, 15, 1, 3, -12, 5, 4, 1};
    bubbleSort(n_array, SIZE);
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", n_array[i]);
    }
    printf("\n");
    return 0;
}
