#ifndef RECORDSHASHTABLE_H_INCLUDED
#define RECORDSHASHTABLE_H_INCLUDED

recordsHashTable *createRecordsHashTable(int size, int multiplier);
void rehashRecordsHashTable(recordsHashTable *ht, int start, int end);
int insertRecordsHashTable(recordHashNode *temp, recordsHashTable *ht);
int insertEntryRecordsHashTable(recordHashNode *temp, recordsHashTable *ht);
recordHashNode *searchEntryRecordsHashTable(char *key, recordsHashTable *ht);
int deleteEntryRecordsHashTable(char *key, recordsHashTable *ht);
void printTypeNode(typeNode *tn);
void printRecordDataNode(recordDataNode *rdn);
void printRecordHashNode(recordHashNode *rhn);
void printRecordHashTable(recordsHashTable *ht);
recordHashNode *createRecordHashNode(char *key);
typeNode *createTypeNode(int fieldtype, char *fieldid);

#endif // RECORDSHASHTABLE_H_INCLUDED
