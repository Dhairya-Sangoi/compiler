//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef STACKDEF_H_INCLUDED
#define STACKDEF_H_INCLUDED

//structure definition for elements on the stack
typedef struct parsedNode{
    int index;
    struct parsedNode *next;
}parsedNode;

//structure definition for the stack used for parsing
typedef struct parsedStack{
    parsedNode *head;
}parsedStack;


#endif // STACKDEF_H_INCLUDED
