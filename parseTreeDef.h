//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef PARSETREEDEF_H_INCLUDED
#define PARSETREEDEF_H_INCLUDED

//structure definition for parse tree node
typedef struct parseTreeNode{
    struct parseTreeNode *parent;
    struct parseTreeNode *child;
    struct parseTreeNode *next;
    char *lexemeCurrentNode;
    int lineno;
    char *token;
    float valueIfNumber;
    int isLeafNode;
    char *nodeSymbol;
    int isNumber;
    int index;
    int ruleno;
    int replacedNode;
    int replacedNodesCount;
}parseTreeNode;

//structure definition for parse tree
typedef struct parseTree{
    parseTreeNode *head;
}parseTree;


#endif // PARSETREEDEF_H_INCLUDED
