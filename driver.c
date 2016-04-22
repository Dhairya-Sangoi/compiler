//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "trieDef.h"
#include "trie.h"
#include "rulesDef.h"
#include "rules.h"
#include "parseTreeDef.h"
#include "parseTree.h"
#include "helperDef.h"
#include "stackDef.h"
#include "stack.h"
#include "helper.h"
#include "parser.h"
#include "lexerDef.h"
#include "lexer.h"
#include "dynamicArrayDef.h"
#include "dynamicArray.h"
#include "quadrupleDef.h"
#include "quadruple.h"

#include "scopeHashTableDef.h"
#include "recordsHashTableDef.h"
#include "allFunctionsHashTableDef.h"
#include "allFunctionsHashTable.h"

#include "scopeHashTable.h"
#include "recordsHashTable.h"
#include "registerDef.h"
#include "semanticAnalyzerDef.h"
#include "semanticAnalyzer.h"

int main(int argc, char *argv[])
{

	if (argc != 3){
		printf("Usage: %s <testCase.txt> <code.asm>\n",argv[0]);
        exit(0);
	}

    char *filename = "rule.txt";
    char *enumfilename = "enum.h";

    trie *terminals, *nonterminals;
    token *allTokens[130];
    ruleq **allRules = (ruleq **)malloc(sizeof(ruleq)*100);
    ruleq **newRules = (ruleq **)malloc(sizeof(ruleq)*100);

    int term,nonterm,rule;

    populateDataStructures(filename, &terminals, &nonterminals, allTokens, allRules, newRules, &term, &nonterm, &rule,enumfilename);

    int firstSetRules[rule+1][nonterm+term];
    int firstSetterminals[nonterm+term][nonterm+term];
    int followSet[nonterm+term][nonterm+term];
    int parseTable[nonterm+term][nonterm+term];

    createFirstSet(term, nonterm, rule, firstSetRules, firstSetterminals, allTokens, allRules, terminals);

    createFollowSet(term, nonterm, rule, followSet, firstSetRules, firstSetterminals, allTokens, allRules, terminals, nonterminals);

    createParseTable(term, nonterm, rule, parseTable, followSet, firstSetRules, firstSetterminals, allTokens, terminals);

    FILE *inputfile = fopen(argv[1],"r");

    tokenlist *tklist = caller_function(inputfile,30,terminals);

    parseTree *pt;

    recordsHashTable *rht;
    allFunctionsHashTable *afht;
    scopeHashTable *globalScope;
    rht = createRecordsHashTable(5,7);
    afht = createAllFunctionsHashTable(5,7);
    globalScope = createScopeHashTable(5,7);
    quadruple *q = createQuadruple(20);

    int parsenodecount, astnodecount, sizenode;
    float compression = 0;
    parsenodecount = 0;
    astnodecount = 0;
    sizenode = sizeof(parseTreeNode);

    int p[2];
    pipe(p);
    int success = parseInputSourceCode(&pt, terminals, nonterminals, allTokens, allRules, term, nonterm, parseTable,tklist, firstSetterminals,followSet,p[1]);
    close(p[1]);
    char *compilationmessage = (char *)malloc(sizeof(char)*1000000);
    char *temp = compilationmessage;
    int len;
    while ( (len = read(p[0], temp, 1) ) > 0){
        temp+=len;
    }
    *temp = '\0';
    close(p[0]);
    
    int p1[2];
    pipe(p1);
    int success2 = 0;
    char *semanticmessage = NULL;
    FILE *fp1 ;
    char *parsetree;
    parsetree = (char *)malloc(sizeof(char) * 100);

    FILE *asmfile;
    
    if (success == 1){
        fp1 = fopen("parsetree.txt","w+");
        dfsParseTree(pt->head,fp1);
        dfsp(pt->head, &parsenodecount);
        makeAST(pt->head,newRules,NULL);
        dfsp(pt->head, &astnodecount);
        asmfile = fopen(argv[2],"w");
        success2 = makeScopeTables(pt->head,rht,afht,globalScope,q,p1[1],asmfile); //change the last parameter
        close(p1[1]);
        semanticmessage = (char *)malloc(sizeof(char)*100000);
        char *temp2 = semanticmessage;
        int len2;
        while ( (len2 = read(p1[0], temp2, 1) ) > 0){
            temp2+=len2;
        }
        *temp2 = '\0';
        close(p1[0]);
        compression =( ((float)(parsenodecount * sizenode) - (astnodecount * sizenode))/(parsenodecount * sizenode) ) * 100;
    }
    
    
    //return 0;



/*

    printf("\nRecord Hash Table:\n");
    printRecordHashTable(rht);
    printf("\nAllFunctions Hash Table:\n");
    printAllFunctionsHashTable(afht);
    printf("\nGlobal Scope Hash Table:\n");
    printScopeHashTable(globalScope);
    printf("\n\n");
    return 0;
    int count = 0;
    if (success){
        FILE *parsingtree = fopen(argv[2],"w");
        dfsParseTree(pt->head,parsingtree, &count);
        fclose(parsingtree);
    }
*/

    printf("(a) FIRST and FOLLOW set automated.\n(b) Both lexical and syntax analysis modules implemented.\n(c) Modules working for all test cases.\n");

    
    




    while (1){
        printf("\n**************************************************************************\n");
        printf("\nPress option for the defined task:\n");
        printf("1: For printing the token list\n");
        printf("2: For parsing to verify the syntactic correctness of the input source code\n");
        printf("3: For printing the abstract syntax tree\n");
        printf("3: For printing the abstract syntax tree\n");
        printf("3: For printing the abstract syntax tree\n");
        printf("3: For printing the abstract syntax tree\n");
        printf("4: For creating the parse tree and printing it appropriately\n");
        printf("   Any other to exit the program\n\nChoice: ");
        char choice;
        scanf(" %c",&choice);
        printf("\n**************************************************************************\n");
        switch(choice){
            case '1':
                printTokenList(tklist,allTokens);
                break;
            case '2':
                if (success){
                    fseek(fp1,0,SEEK_SET);
                    while(fgets(parsetree,100,fp1)!=NULL){
                        fputs(parsetree,stdout);
                    }
                    //printf("%s",parsetree);
                }
                else {
                    printf("Didn't generate parse tree since compilation failed\n");
                }
                break;
            case '3':
                if (success){
                    dfsParseTree(pt->head,stdout);
                }
                else {
                    printf("Didn't generate abstract syntax tree since compilation failed\n");
                }
                break;
            case '4':
                if (success){
                    printf("Parse tree\tNumber of nodes = %d\tAllocated Memory =  %d Bytes\n", parsenodecount, sizenode*(parsenodecount));
                    printf("AST\tNumber of nodes = %d\tAllocated Memory =  %d Bytes\n", astnodecount, sizenode*(astnodecount));
                    printf("Compression Peercentage =  %f%%", compression);
                }
                else {
                    printf("Didn't generate parse tree and abstract syntax tree since compilation failed\n");
                }
                break;

            case '5':
                if (success == 1 && success2 == 1){
                    printf("Symbol Table Entries: \n");
                    printAllFunctionsHashTable(afht,rht);
                }
                else {
                    printf("The code being not correct (syntactically or semantically), couldn't print symbol table.\n");
                }
                break;

            case '6':
                if (success == 0){
                    printf("Syntactical Errors:\n");
                    printf("%s\n",compilationmessage);
                }
                else {
                    if (success2 == 0){
                        printf("Semantic Errors:\n");
                        printf("%s\n",semanticmessage);
                    }
                }
                
                break;

            case '7':
                printf("Generated <%s> file.\n", argv[2]);
                break;
            default:
                printf("Exiting Compiler Project. It was a nice learning experience :)\n");
                
                //delete parsetree.txt
                return 0;
        }

    }
    return 0;
}
