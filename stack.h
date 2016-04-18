//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

//create stack node
parsedNode *createParsedNode(int index);

//create stack
parsedStack *createParsedStack();

//push node on stack
void pushStack(parsedStack *stack, int index);

//peek top node of stack
int peekStack(parsedStack *stack);

//pop from stack
int popStack(parsedStack *stack);


#endif // STACK_H_INCLUDED
