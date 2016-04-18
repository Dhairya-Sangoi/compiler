//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

//create first set for nonterminals and each rule
void createFirstSet(int term, int nonterm, int rule, int firstSetRules[rule+1][term+nonterm], int firstSetTerminals[term+nonterm][term+nonterm], token **allTokens, ruleq **allRules, trie *terminals);

//print first set for nonterminals
void printFirstSetTerminals(int term, int nonterm,int firstSetTerminals[term+nonterm][term+nonterm],token **allTokens);

//create follow set
void createFollowSet(int term, int nonterm, int rule, int followSet[nonterm+term][nonterm+term], int firstSetRules[rule+1][term+nonterm], int firstSetTerminals[term+nonterm][term+nonterm], token **allTokens, ruleq **allRules, trie *terminals, trie *nonterminals);

//print follow set
void printFollowSet(int term, int nonterm,int followSet[term+nonterm][term+nonterm],token **allTokens);

//create parse table
void createParseTable(int term, int nonterm, int rule, int parseTable[nonterm+term][nonterm+term], int followSet[nonterm+term][nonterm+term], int firstSetRules[rule+1][term+nonterm], int firstSetTerminals[term+nonterm][term+nonterm], token **allTokens, trie *terminals);

//parse source code
int parseInputSourceCode(parseTree **p, trie *terminals, trie *nonterminals, token **allTokens, ruleq **allRules, int term, int nonterm, int parseTable[nonterm+term][nonterm+term], tokenlist *tklist, int firstSetTerminals[term+nonterm][term+nonterm], int followSet[nonterm+term][nonterm+term], int fd);


#endif // PARSER_H_INCLUDED
