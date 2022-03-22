#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200

int main(void){
	char** pp_data = NULL;
	int n;
	int length;
    char buff[MAX];
    printf("n : ");
	scanf("%d", &n);
    pp_data = (char**)malloc(sizeof(char*) * n);
    
    printf("Length : ");
    scanf("%d", &length);
    
    printf("words\n");
    for(int i = 0; i < n; i++) {
        pp_data[i] = (char*)malloc(sizeof(char)*length+1);
        scanf("%s",buff);
        // C 표준 함수 사용 시 할당된 영역을 넘어도 데이터를 받는 것을 방지를 위해 버퍼에서 길이 확인
        if(strlen(buff) > length){
            printf("out of range.. re-enter word.. \n");
            i--;
            continue;
        }
        // 버퍼의 문자열을 할당된 메모리 영역에 복사
        strcpy(pp_data[i], buff);
    }
    
    printf("inputs : ");
	for(int i = 0; i < n; i++) {
        printf("%s ", pp_data[i]);
    }
    printf("\n");

	for(int i = 0; i < n; i++) {
        free(pp_data[i]);
    }
    free(pp_data);
	

	return 0;
}
