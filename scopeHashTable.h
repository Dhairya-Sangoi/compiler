#ifndef SCOPEHASHTABLE_H_INCLUDED
#define SCOPEHASHTABLE_H_INCLUDED

#define INT_SIZE 2
#define REAL_SIZE 4

scopeHashTable *createScopeHashTable(int size, int multiplier);
void rehashScopeHashTable(scopeHashTable *ht, int start, int end);
int insertScopeHashTable(scopeHashNode *temp, scopeHashTable *ht);
int insertEntryScopeHashTable(scopeHashNode *temp, scopeHashTable *ht);
scopeHashNode *searchEntryScopeHashTable(char *key, scopeHashTable *ht);
int deleteEntryScopeHashTable(char *key, scopeHashTable *ht);
void printScopeDataNode(scopeDataNode *sdn);
void printScopeHashNode(scopeHashNode *shn);
void printScopeHashTable(scopeHashTable *ht);
scopeHashNode *createScopeHashNode(char *key, char *typeName, parseTreeNode *head, int *offset);

#endif // SCOPEHASHTABLE_H_INCLUDED
