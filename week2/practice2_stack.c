//Template
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Node{
	int nData;
	struct Node *next;
}Node;

typedef struct Stack{
	Node *top;
}Stack;

void InitializeStack(Stack* stack);
void Push(Stack *stack, int nData);
int Pop(Stack *stack);


int main(void){
    char operation[5];
    int data;
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    InitializeStack(stack);
    while (1) {
        printf("Enter what to do (pop, push, end) : ");
        scanf("%s", operation);
        
        if(!strcmp(operation, "pop")){
            data = Pop(stack);
            if(data == INT_MAX){
                printf("Empty Stack!\n");
                continue;
            }
            printf("Popped data : %d\n", data);
        }
        else if(!strcmp(operation, "push")){
            scanf("%d", &data);
            Push(stack, data);
        }
        else if(!strcmp(operation, "end")){
            return 0;
        }
        else{
            printf("Invalid input!\n");
        }
    }
	return 0;
}

void InitializeStack(Stack* stack){
    stack = NULL;
}

void Push(Stack *stack, int nData){
    Node *node = (Node*)malloc(sizeof(Node));
    node->nData = nData;
    node->next = stack->top;
    stack->top = node;
}

int Pop(Stack *stack){
    if(stack->top == NULL){
        return(INT_MAX);
    }
    Node *tmpcell = stack->top;
    int data = tmpcell->nData;
    stack->top = tmpcell->next;
    
    free(tmpcell);
    return data;
}
