//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef PARSETREE_H_INCLUDED
#define PARSETREE_H_INCLUDED

//create parse tree node
parseTreeNode *createParseTreeNode(parseTreeNode *parent, char *lCN, int lineno, char *token, int isNumber, int iLN, char *nodeSymbol, int index, int ruleno);

//create parse tree
parseTree *createParseTree();

//add child to parse tree
void addChild(parseTreeNode *parent, parseTreeNode *child);

//print parse tree node
void printParseTreeNode(parseTreeNode *ptn, FILE *fp);

//dfs on parse tree
void dfsParseTree(parseTreeNode *ptn, FILE *fp);

void dfsp(parseTreeNode *ptn, int *cnt);


#endif // PARSETREE_H_INCLUDED
