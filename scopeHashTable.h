#ifndef SCOPEHASHTABLE_H_INCLUDED
#define SCOPEHASHTABLE_H_INCLUDED

#define INT_SIZE 4
#define REAL_SIZE 8

scopeHashTable *createScopeHashTable(int size, int multiplier);
void rehashScopeHashTable(scopeHashTable *ht, int start, int end);
int insertScopeHashTable(scopeHashNode *temp, scopeHashTable *ht);
int insertEntryScopeHashTable(scopeHashNode *temp, scopeHashTable *ht);
scopeHashNode *searchEntryScopeHashTable(char *key, scopeHashTable *ht);
int deleteEntryScopeHashTable(char *key, scopeHashTable *ht);
void printScopeDataNode(scopeDataNode *sdn, char *fname, recordsHashTable *rht);
void printScopeHashNode(scopeHashNode *shn, char *fname, recordsHashTable *rht);
void printScopeHashTable(scopeHashTable *ht, char *fname, recordsHashTable *rht);
scopeHashNode *createScopeHashNode(char *key, char *typeName, parseTreeNode *head, int *offset);

#endif // SCOPEHASHTABLE_H_INCLUDED
