#ifndef SCOPEHASHTABLEDEF_H_INCLUDED
#define SCOPEHASHTABLEDEF_H_INCLUDED

typedef struct scopeDataNode{
    char *typeName;
    parseTreeNode *head;
    int isChanged;
    int offset;
    int regIfPresent;
}scopeDataNode;

typedef struct scopeHashNode{
    char *key;
    scopeDataNode *data;
}scopeHashNode;

typedef struct scopeHashTable{
    int tableSize;
    scopeHashNode **arr;
    scopeHashNode *specialValue;
    int elements;
    int multiplier;
    float loadfactor;
    int offset;
}scopeHashTable;


#endif // SCOPEHASHTABLEDEF_H_INCLUDED
