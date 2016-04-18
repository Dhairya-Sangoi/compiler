//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trieDef.h"


trieNode *createTrieNode(char key){
    trieNode *tn = (trieNode *)malloc(sizeof(trieNode));
    tn->key = key;
    tn->dataIndex = -1;
    tn->child = NULL;
    tn->next = NULL;
    return tn;
}

trie *createTrie(){
    trie *t = (trie *)malloc(sizeof(trie));
    t->head = NULL;
    return t;
}

trieNode *searchTrie(trie *t, char *str){
    trieNode *parent = NULL;
    trieNode *temp = t->head;
    while ( (temp!=NULL) && (*str != '\0') ){
        if (temp->key == *str){
            parent = temp;
            temp = temp->child;
            str += 1;
            if (*str == '\0'){
                if (parent->dataIndex != -1){
                    return parent;
                }
                else {
                    return NULL;
                }
            }
        }
        else {
            if (temp->key > *str){
                return NULL;
            }
            temp = temp->next;
        }
    }
    return NULL;
}

void insert(trie *t, char *str, int pos){
    if (t->head == NULL){
        t->head = createTrieNode(*str);
        trieNode *temp = t->head;
        str += 1;
        while (*str != '\0'){
            temp->child = createTrieNode(*str);
            str += 1;
            temp = temp->child;
        }
        temp->dataIndex = pos;
        return;
    }

    trieNode *head = t->head;
    trieNode *temp = t->head;
    trieNode *prev = NULL;
    trieNode *parent = NULL;
    int child = 0, next = 0;
    while ((temp != NULL) && (*str != '\0')){
        child = 0, next = 0;
        if (temp->key == *str){
            parent = temp;
            head = temp->child;
            temp = temp->child;
            str += 1;
            child = 1;
        }
        else {
            if (temp->key < *str){
                prev = temp;
                temp = temp->next;
                next = 1;
            }
            else {
                if (head == temp){
                    if (parent == NULL){
                        trieNode *tn = createTrieNode(*str);
                        tn->next = temp;
                        t->head = tn;
                        temp = tn;
                        //insert remaining
                    }
                    else {
                        parent->child = createTrieNode(*str);
                        parent->child->next = temp;
                        temp = parent->child;
                        //insert remaining
                    }
                }
                else {
                    prev->next = createTrieNode(*str);
                    prev->next->next = temp;
                    temp = prev->next;
                }
                break;

            }
        }
    }
    if (temp == NULL){
        if (child){
            parent->child = createTrieNode(*str);
            parent->child->next = temp;
            temp = parent->child;
        }
        if (next){
            prev->next = createTrieNode(*str);
            prev->next->next = temp;
            temp = prev->next;
        }

    }
    str += 1;
    while (*str != '\0'){
        temp->child = createTrieNode(*str);
        str += 1;
        temp = temp->child;
    }
    temp->dataIndex = pos;
    return;
}

void search(trie *t, char *a){
    trieNode *temp = searchTrie(t,a);
    if (temp == NULL){
        printf("not found :%s\n",a);
    }
    else {
        printf("found :%s->index: %d\n",a, temp->dataIndex);
    }
}

void printNode(trieNode *t){
    if (t != NULL){
        trieNode *temp = t;
        while (temp!= NULL){
            printf("%c ", temp->key);
            temp = temp->next;
        }
        printf("\n");
        temp = t;
        while (temp!=NULL){
            printNode(temp->child);
            temp = temp->next;
        }
    }
    else {
        return;
    }

}

void printTrie(trie *t){
    if (t->head!=NULL){
        printNode(t->head);
    }
    return;
}
