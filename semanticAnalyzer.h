#ifndef SEMANTICANALYZER_H_INCLUDED
#define SEMANTICANALYZER_H_INCLUDED


void makeAST(parseTreeNode *head, ruleq **newRules, parseTreeNode *param);
int canBeAdded(int index);
void dfsForScopeTables(ASTNode *head, recordsHashTable *rht, allFunctionsHashTable *afht, scopeHashTable *currentScope, scopeHashTable *globalScope, typeNode **tn, int *recWidth, IONode **ion, scopeHashTable *recFields, char *fname, int *offset, FILE *fp, int *success);
int makeScopeTables(ASTNode *head, recordsHashTable *rht, allFunctionsHashTable *afht, scopeHashTable *globalScope, quadruple *q, int fd, FILE *asmfile);
void dfsForSemanticAnalysis(ASTNode *head, recordsHashTable *rht, allFunctionsHashTable *afht, scopeHashTable *currentScope, scopeHashTable *globalScope, char *fname, char **argtype, whileScopeList *headWhile, whileScopeList *tailWhile, int mylabel, int nextlabel, int grandnextlabel, int *ptrAvailableLabels, int *ptrAvailableTemporary, int jumpIfTrue, int jumpIfFalse, int shouldOutputLabel, int *success, quadruple *q, dynamicArray *da, int *maxoffset, FILE *fp, int *ln);
void makeCode(quadrupleNode *qn, scopeHashTable *currentScope, scopeHashTable *globalScope, recordsHashTable *rht, dynamicArray *usedLabels, reg **registers, int total_registers, int *nextlabel, FILE *fp);
int isNotRecordOpcode(int opcode);


#endif // SEMANTICANALYZER_H_INCLUDED
