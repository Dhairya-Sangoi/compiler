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

#include "scopeHashTable.h" //should be commented
#include "recordsHashTable.h"

#include "enum.h"


scopeHashTable *createScopeHashTable(int size, int multiplier){
    scopeHashTable *rht = (scopeHashTable *)malloc(sizeof(scopeHashTable));
    rht->tableSize = size;
    rht->arr = (scopeHashNode **)malloc(sizeof(scopeHashNode *)*size);
    int i;
    for (i=0;i<size;i++){
        rht->arr[i] = NULL;
    }
    rht->specialValue = (scopeHashNode *)rht;
    rht->elements = 0;
    rht->loadfactor = 0;
    rht->multiplier = multiplier;
    rht->offset = 0;
    return rht;
}

void rehashScopeHashTable(scopeHashTable *ht, int start, int end){
    if (start == end){
        scopeHashNode *temp = ht->arr[start];
        ht->arr[start] = NULL;
        insertEntryScopeHashTable(temp,ht); //
        return;
    }
    else {
        scopeHashNode *temp = ht->arr[start];
        ht->arr[start] = NULL;
        rehashScopeHashTable(ht,start+1,end);
        insertEntryScopeHashTable(temp,ht);//
        return;
    }
}

int insertScopeHashTable(scopeHashNode *temp, scopeHashTable *ht){
    int retval = insertEntryScopeHashTable(temp,ht);
    if (ht->loadfactor > 0.7){
        int prevSize = ht->tableSize;
        ht->tableSize = nextPrime(ht->tableSize);
        ht->elements = 0;
        ht->arr = (scopeHashNode **)realloc(ht->arr,sizeof(scopeHashNode *)*ht->tableSize);
        int i;
        for (i=prevSize;i<ht->tableSize;i++){
            ht->arr[i] = NULL;
        }
        rehashScopeHashTable(ht,0,prevSize-1);
        ht->loadfactor = (float)ht->elements / ht->tableSize;
    }
    //printf("ts:%d\n", ht->tableSize);
    return retval;
}

int insertEntryScopeHashTable(scopeHashNode *temp, scopeHashTable *ht){
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

scopeHashNode *searchEntryScopeHashTable(char *key, scopeHashTable *ht){
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

int deleteEntryScopeHashTable(char *key, scopeHashTable *ht){
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

scopeHashNode *createScopeHashNode(char *key, char *typeName, parseTreeNode *head, int *offset){
    scopeHashNode *shn = (scopeHashNode *)malloc(sizeof(scopeHashNode));
    shn->key = key;
    shn->data = (scopeDataNode *)malloc(sizeof(scopeDataNode));
    shn->data->typeName = typeName;
    shn->data->head = head;
    shn->data->isChanged = 0;
    if (*offset >= 0){
        shn->data->offset = *offset;
        //*offset = *offset + ( (head->index == TK_INT) ? INT_SIZE : REAL_SIZE) ;
    }
    else {
        shn->data->offset = 0;
    }
    shn->data->regIfPresent = -1;
    return shn;
}


void printScopeDataNode(scopeDataNode *sdn, char *fname, recordsHashTable *rht){
    if (strcmp(sdn->typeName,"int") == 0 || strcmp(sdn->typeName,"real") == 0){
        printf("Type: %s",sdn->typeName);
    }
    else {
        recordHashNode *rhn = searchEntryRecordsHashTable(sdn->typeName, rht);
        printf("Type:");
        if (rhn != NULL){
            typeNode *temp = rhn->data->type;
            while (temp->next != NULL){
                if (temp->fieldtype == TK_INT){
                    printf(" int *");
                }
                else {
                    printf(" real *");   
                }
                temp = temp->next;
            }

                if (temp->fieldtype == TK_INT){
                    printf(" int");
                }
                else {
                    printf(" real");   
                }
                //temp = temp->next;

        }
    }
    printf(" Scope: %s",fname);
    printf(" Offset: %d",sdn->offset);
    //printf(" regIfPresent: %d", sdn->regIfPresent);
}

void printScopeHashNode(scopeHashNode *shn, char *fname, recordsHashTable *rht){
    printf("Lexeme: %s ",shn->key);
    printScopeDataNode(shn->data,fname,rht);
    printf("\n");
}

void printScopeHashTable(scopeHashTable *ht, char *fname, recordsHashTable *rht){
    if (ht == NULL){
        return;
    }
    int i;
    //printf("\nprinting table\n");
    for (i=0;i<ht->tableSize;i++){
        //printf("Entry %-3d:  ",i+1);
        if (ht->arr[i] == NULL || ht->arr[i] == ht->specialValue){
            //printf("NULL\n");
        }
        else {
            printScopeHashNode(ht->arr[i], fname, rht);
        }
    }
    return;
}

