//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

//create trie node
trieNode *createTrieNode(char key);

//create trie
trie *createTrie();

//search trie
trieNode *searchTrie(trie *t, char *str);

//insert into trie
void insert(trie *t, char *str, int pos);

//search trie
void search(trie *t, char *a);

////print trie node
void printNode(trieNode *t);

//print trie
void printTrie(trie *t);

#endif // TRIE_H_INCLUDED
