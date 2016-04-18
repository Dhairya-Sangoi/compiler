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
#include "scopeHashTable.h"
#include "allFunctionsHashTableDef.h"
#include "allFunctionsHashTable.h" //should be commented


allFunctionsHashTable *createAllFunctionsHashTable(int size, int multiplier){
    allFunctionsHashTable *rht = (allFunctionsHashTable *)malloc(sizeof(allFunctionsHashTable));
    rht->tableSize = size;
    rht->arr = (allFunctionsHashNode **)malloc(sizeof(allFunctionsHashNode *)*size);
    int i;
    for (i=0;i<size;i++){
        rht->arr[i] = NULL;
    }
    rht->specialValue = (allFunctionsHashNode *)rht;
    rht->elements = 0;
    rht->loadfactor = 0;
    rht->multiplier = multiplier;
    return rht;
}

void rehashAllFunctionsHashTable(allFunctionsHashTable *ht, int start, int end){
    if (start == end){
        allFunctionsHashNode *temp = ht->arr[start];
        ht->arr[start] = NULL;
        insertEntryAllFunctionsHashTable(temp,ht); //
        return;
    }
    else {
        allFunctionsHashNode *temp = ht->arr[start];
        ht->arr[start] = NULL;
        rehashAllFunctionsHashTable(ht,start+1,end);
        insertEntryAllFunctionsHashTable(temp,ht);//
        return;
    }
}

int insertAllFunctionsHashTable(allFunctionsHashNode *temp, allFunctionsHashTable *ht){
    int retval = insertEntryAllFunctionsHashTable(temp,ht);
    if (ht->loadfactor > 0.7){
        int prevSize = ht->tableSize;
        ht->tableSize = nextPrime(ht->tableSize);
        ht->elements = 0;
        ht->arr = (allFunctionsHashNode **)realloc(ht->arr,sizeof(allFunctionsHashNode *)*ht->tableSize);
        int i;
        for (i=prevSize;i<ht->tableSize;i++){
            ht->arr[i] = NULL;
        }
        rehashAllFunctionsHashTable(ht,0,prevSize-1);
        ht->loadfactor = (float)ht->elements / ht->tableSize;
    }
    return retval;
}

int insertEntryAllFunctionsHashTable(allFunctionsHashNode *temp, allFunctionsHashTable *ht){
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

allFunctionsHashNode *searchEntryAllFunctionsHashTable(char *key, allFunctionsHashTable *ht){
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

int deleteEntryAllFunctionsHashTable(char *key, allFunctionsHashTable *ht){
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

allFunctionsHashNode *createAllFunctionsHashNode(char *key, scopeHashTable *scope){
    allFunctionsHashNode *afhn = (allFunctionsHashNode *)malloc(sizeof(allFunctionsHashNode));
    afhn->key = key;
    afhn->data = (allFunctionsDataNode *)malloc(sizeof(allFunctionsDataNode));
    afhn->data->scope = scope;
    afhn->data->input_params = NULL;
    afhn->data->output_params = NULL;
    return afhn;
}

IONode *createIONode(char *param){
    IONode *in = (IONode *)malloc(sizeof(IONode));
    in->param = param;
    in->next = NULL;
    return in;
}



void printIONode(IONode *ion){
    if (ion == NULL){
        return;
    }
    printf("%s ",ion->param);
    printIONode(ion->next);
}

void printAllFunctionsDataNode(allFunctionsDataNode *afdn){
    printf("\nscopeHashTable: \n");
    printScopeHashTable(afdn->scope);
    printf("inputparam: ");
    printIONode(afdn->input_params);
    printf("\n");
    printf("outputparam: ");
    printIONode(afdn->output_params);
}

void printAllFunctionsHashNode(allFunctionsHashNode *shn){
    printf("key: %s data: ",shn->key);
    printAllFunctionsDataNode(shn->data);
    printf("\n");
}

void printAllFunctionsHashTable(allFunctionsHashTable *ht){
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
            printAllFunctionsHashNode(ht->arr[i]);
        }
    }
    return;
}
