#ifndef ALLFUNCTIONSHASHTABLEDEF_H_INCLUDED
#define ALLFUNCTIONSHASHTABLEDEF_H_INCLUDED

typedef struct IONode{
    char *param;
    struct IONode *next;
}IONode;

typedef struct allFunctionsDataNode{
    scopeHashTable *scope;
    IONode *input_params;
    IONode *output_params;
}allFunctionsDataNode;

typedef struct allFunctionsHashNode{
    char *key;
    allFunctionsDataNode *data;
}allFunctionsHashNode;

typedef struct allFunctionsHashTable{
    int tableSize;
    allFunctionsHashNode **arr;
    allFunctionsHashNode *specialValue;
    int elements;
    int multiplier;
    float loadfactor;
}allFunctionsHashTable;


#endif // ALLFUNCTIONSHASHTABLEDEF_H_INCLUDED
