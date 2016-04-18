//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef TRIEDEF_H_INCLUDED
#define TRIEDEF_H_INCLUDED

//trie node definition
typedef struct trieNode{
    char key;
    struct trieNode *child;
    struct trieNode *next;
    int dataIndex;
}trieNode;

//trie definition 
typedef struct trie{
    trieNode *head;
}trie;


#endif // TRIEDEF_H_INCLUDED
