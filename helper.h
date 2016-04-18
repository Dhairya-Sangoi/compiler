//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED


int isPrime(int n);

int nextPrime(int n);

//method for printing comment free code
void printCommentFreeCode(tokenlist* head);

//create token corresponding to each terminal and nontermial
token *createToken(char *name, int isTerm, int index, int firstRule);

//print token
void printToken(token *temp);

//print token list
void printTokenList(tokenlist *temp, token **allTokens);

//helper function to populate first set, follow set and token information
void populateDataStructures(char *filename, trie **termptr, trie **nontermptr, token **allTokens, ruleq **allRules, ruleq **newRules, int *t, int *nt, int *rl, char *enumfilename);

//create token list
tokenlist *createTokenList(int index);

//helper method to add rhs of rule as child of lhs of rule in parse tree
parseTreeNode *addNodeToParseTree(parseTreeNode *curr,ruleNode *head,token **allTokens, char *lexname, int ruleno);

//helper method to add rhs of rule on stack
void addTokensToStack(parsedStack *stack,ruleNode *head);

//helper method to print stack
void printStack(parsedNode *head, token **allTokens);

void makeNewRules(ruleq **newRules, ruleq **allRules, int rule, token **allTokens);

#endif // HELPER_H_INCLUDED
