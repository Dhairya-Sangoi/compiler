#ifndef RECORDSHASHTABLEDEF_H_INCLUDED
#define RECORDSHASHTABLEDEF_H_INCLUDED


typedef struct typeNode{
    int fieldtype;
    char *fieldid;
    struct typeNode *next;
}typeNode;

typedef struct recordDataNode{
    int width;
    typeNode *type;
    scopeHashTable *fields;
}recordDataNode;

typedef struct recordHashNode{
    char *key;
    recordDataNode *data;
}recordHashNode;

typedef struct recordsHashTable{
    int tableSize;
    recordHashNode **arr;
    recordHashNode *specialValue;
    int elements;
    int multiplier;
    float loadfactor;
}recordsHashTable;


#endif // RECORDSHASHTABLEDEF_H_INCLUDED
