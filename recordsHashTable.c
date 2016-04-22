#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "trieDef.h"
#include "trie.h"
#include "rulesDef.h"
#include "rules.h"
#include "helperDef.h"
#include "parseTreeDef.h"
#include "parseTree.h"
#include "stackDef.h"
#include "stack.h"
#include "helper.h"
#include "scopeHashTableDef.h"
#include "recordsHashTableDef.h"
#include "scopeHashTable.h"

#include "recordsHashTable.h" //should be commented

recordsHashTable *createRecordsHashTable(int size, int multiplier){
    recordsHashTable *rht = (recordsHashTable *)malloc(sizeof(recordsHashTable));
    rht->tableSize = size;
    rht->arr = (recordHashNode **)malloc(sizeof(recordHashNode *)*size);
    int i;
    for (i=0;i<size;i++){
        rht->arr[i] = NULL;
    }
    rht->specialValue = (recordHashNode *)rht;
    rht->elements = 0;
    rht->loadfactor = 0;
    rht->multiplier = multiplier;
    return rht;
}

void rehashRecordsHashTable(recordsHashTable *ht, int start, int end){
    if (start == end){
        recordHashNode *temp = ht->arr[start];
        ht->arr[start] = NULL;
        insertEntryRecordsHashTable(temp,ht); //
        return;
    }
    else {
        recordHashNode *temp = ht->arr[start];
        ht->arr[start] = NULL;
        rehashRecordsHashTable(ht,start+1,end);
        insertEntryRecordsHashTable(temp,ht);//
        return;
    }
}

int insertRecordsHashTable(recordHashNode *temp, recordsHashTable *ht){
    int retval = insertEntryRecordsHashTable(temp,ht);
    if (ht->loadfactor > 0.7){
        int prevSize = ht->tableSize;
        ht->tableSize = nextPrime(ht->tableSize);
        ht->elements = 0;
        ht->arr = (recordHashNode **)realloc(ht->arr,sizeof(recordHashNode *)*ht->tableSize);
        int i;
        for (i=prevSize;i<ht->tableSize;i++){
            ht->arr[i] = NULL;
        }
        rehashRecordsHashTable(ht,0,prevSize-1);
        ht->loadfactor = (float)ht->elements / ht->tableSize;
    }
    return retval;
}

int insertEntryRecordsHashTable(recordHashNode *temp, recordsHashTable *ht){
    if (temp == NULL){
        return 0;
    }
    int probe = 0;
    int keysize = strlen(temp->key);
    int i = 0;
    for (i=0;i<keysize;i++){
        probe = probe * (ht->multiplier);
        probe += temp->key[i];
    }
    if (probe < 0) probe = probe * -1;
    probe = probe % ht->tableSize;
    int indexToAdd = -1;
    while (ht->arr[probe]!=NULL ){
        if (ht->arr[probe] != ht->specialValue){
            if (strcmp(temp->key,ht->arr[probe]->key) == 0){
                return 0;
            }
        }
        else {
            if (indexToAdd == -1){
                indexToAdd = probe;
            }
        }
        probe = (probe+1) % ht->tableSize;
    }
    if (indexToAdd != -1){
        probe = indexToAdd;
    }
    ht->arr[probe] = temp;
    ht->elements ++;
    ht->loadfactor = (float)ht->elements/ht->tableSize;
    return 1;
}

recordHashNode *searchEntryRecordsHashTable(char *key, recordsHashTable *ht){
    int probe = 0;
    int keysize = strlen(key);
    int i = 0;
    for (i=0;i<keysize;i++){
        probe = probe * (ht->multiplier);
        probe += key[i];
    }
    if (probe < 0) probe = probe * -1;
    probe = probe % ht->tableSize;
    while (ht->arr[probe]!=NULL){
        if (ht->arr[probe] != ht->specialValue){
            if (strcmp(key,ht->arr[probe]->key) == 0){
                return ht->arr[probe];
            }
            else {
                probe = (probe+1) % ht->tableSize;
            }
        }
        else {
            probe = (probe+1) % ht->tableSize;
        }
    }
    return NULL;
}

int deleteEntryRecordsHashTable(char *key, recordsHashTable *ht){
    int probe = 0;
    int keysize = strlen(key);
    int i = 0;
    for (i=0;i<keysize;i++){
        probe = probe * (ht->multiplier);
        probe += key[i];
    }
    if (probe < 0) probe = probe * -1;
    probe = probe % ht->tableSize;
    while (ht->arr[probe]!=NULL){
        if (ht->arr[probe] != ht->specialValue){
            if (strcmp(key,ht->arr[probe]->key) == 0){
                ht->arr[probe] = ht->specialValue;
                return 1;
                //return ht->arr[probe];
            }
            else {
                probe = (probe+1) % ht->tableSize;
            }
        }
        else {
            probe = (probe+1) % ht->tableSize;
        }
    }
    return 0;
}

recordHashNode *createRecordHashNode(char *key){
    recordHashNode *rhn = (recordHashNode *)malloc(sizeof(recordHashNode));
    rhn->key = key;
    rhn->data = (recordDataNode *)malloc(sizeof(recordDataNode));
    rhn->data->fields = createScopeHashTable(5,7);
    rhn->data->width = 0;
    rhn->data->type = NULL;
    return rhn;
}

typeNode *createTypeNode(int fieldtype, char *fieldid){
    typeNode *tn = (typeNode *)malloc(sizeof(typeNode));
    tn->fieldid = fieldid;
    tn->fieldtype = fieldtype;
    tn->next = NULL;
    return tn;
}

void printTypeNode(typeNode *tn){
    if(tn == NULL){
        return;
    }
    printf("%s:%d ",tn->fieldid, tn->fieldtype);
    printTypeNode(tn->next);
}

void printRecordDataNode(recordDataNode *rdn){
    printf("\nrecords fields:\n");
    //printScopeHashTable(rdn->fields);
    printf("\n");
    printf("width: %d type: ",rdn->width);
    printTypeNode(rdn->type);
}

void printRecordHashNode(recordHashNode *rhn){
    printf("key: %s data: ",rhn->key);
    printRecordDataNode(rhn->data);
    printf("\n");
}

void printRecordHashTable(recordsHashTable *ht){
    if (ht == NULL){
        return;
    }
    int i;
    //printf("\nprinting table\n");
    for (i=0;i<ht->tableSize;i++){
        printf("Entry %-3d:  ",i+1);
        if (ht->arr[i] == NULL || ht->arr[i] == ht->specialValue){
            printf("NULL\n");
        }
        else {
            printRecordHashNode(ht->arr[i]);
        }
    }
    return;
}


