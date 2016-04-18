#ifndef SEMANTICANALYZERDEF_H_INCLUDED
#define SEMANTICANALYZERDEF_H_INCLUDED

typedef parseTreeNode ASTNode;
typedef parseTree AST;

typedef struct whileScopeList{
    scopeHashTable *wsht;
    int isChanged;
    struct whileScopeList *next;
}whileScopeList;

#endif // SEMANTICANALYZERDEF_H_INCLUDED
