//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stackDef.h"


parsedNode *createParsedNode(int index){
    parsedNode *temp = (parsedNode *)malloc(sizeof(parsedNode));
    temp->index = index;
    temp->next = NULL;
    return temp;
}

parsedStack *createParsedStack(){
    parsedStack *temp = (parsedStack *)malloc(sizeof(parsedStack));
    temp->head = NULL;
    return temp;
}

void pushStack(parsedStack *stack, int index){
    if (stack->head == NULL){
        stack->head = createParsedNode(index);
        return;
    }
    else {
        parsedNode *temp = createParsedNode(index);
        temp->next = stack->head;
        stack->head = temp;
        return;
    }

}

int peekStack(parsedStack *stack){
    if (stack->head == NULL){
        return -1;
    }
    else {
        return stack->head->index;
    }
}

int popStack(parsedStack *stack){
    if (stack->head == NULL){
        return -1;
    }
    else {
        parsedNode *temp = stack->head;
        stack->head = temp->next;
        int ind = temp->index;
        free(temp);
        return ind;
    }
}


