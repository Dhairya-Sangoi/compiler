#ifndef ALLFUNCTIONSHASHTABLE_H_INCLUDED
#define ALLFUNCTIONSHASHTABLE_H_INCLUDED

allFunctionsHashTable *createAllFunctionsHashTable(int size, int multiplier);
void rehashAllFunctionsHashTable(allFunctionsHashTable *ht, int start, int end);
int insertAllFunctionsHashTable(allFunctionsHashNode *temp, allFunctionsHashTable *ht);
int insertEntryAllFunctionsHashTable(allFunctionsHashNode *temp, allFunctionsHashTable *ht);
allFunctionsHashNode *searchEntryAllFunctionsHashTable(char *key, allFunctionsHashTable *ht);
int deleteEntryAllFunctionsHashTable(char *key, allFunctionsHashTable *ht);
void printIONode(IONode *ion);
void printAllFunctionsDataNode(allFunctionsDataNode *afdn, char *fname, recordsHashTable *rht);
void printAllFunctionsHashNode(allFunctionsHashNode *shn, recordsHashTable *rht);
void printAllFunctionsHashTable(allFunctionsHashTable *ht, recordsHashTable *rht);
allFunctionsHashNode *createAllFunctionsHashNode(char *key, scopeHashTable *scope);
IONode *createIONode(char *param, char *paramname);

#endif // ALLFUNCTIONSHASHTABLE_H_INCLUDED
