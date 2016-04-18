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
#include "scopeHashTable.h"
#include "allFunctionsHashTableDef.h"
#include "allFunctionsHashTable.h"
#include "recordsHashTableDef.h"
#include "recordsHashTable.h"
#include "semanticAnalyzerDef.h"
#include "semanticAnalyzer.h"

int main(int argc, char *argv[])
{

	if (argc != 3){
		printf("Usage: %s <testCase.txt> <parseTreeOutFile.txt>\n",argv[0]);
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

    int p[2];
    //pipe(p);
    p[1] = 1;
    p[0] = 0;
    int success = parseInputSourceCode(&pt, terminals, nonterminals, allTokens, allRules, term, nonterm, parseTable,tklist, firstSetterminals,followSet,p[1]);
    if (success == 0){
        printf("compilation error\n");
        return 0;
    }
//actual code
//    close(p[1]);
//    char *compilationmessage = (char *)malloc(sizeof(char)*100000);
//    char *temp = compilationmessage;
//    int len;
//    while ( (len = read(p[0], temp, 1) ) > 0){
//        temp+=len;
//    }
//    *temp = '\0';
//    close(p[0]);
//actual code end






//    FILE *fp = fopen("rule.txt","r");
//    FILE *out = fopen("output.txt","w");
//    ruleq *temp;
//    ruleNode *t;
//    char c;
//    int i = 0;
//    fputs("Rule: ",out);
//    while (!feof(fp)){
//        c = fgetc(fp);
//        if (c == '\n' || c == '\0'){
//            fputc(c,out);
//            fputs("Semantic: ",out);
//            temp = newRules[i];
//            //fputc('<',out);
//            fputs(allTokens[temp->nontermindex]->name,out);
//            fputs(".node",out);
//            t = temp->head;
//            if (newRules[i]->index != 1){
//                fputs(" = createNode( ",out);
//
//                while (t != NULL && t->next!=NULL){
//                    fputs(allTokens[t->val]->name,out);
//                    fputs(".node, ",out);
//                    t = t->next;
//                }
//                if (t!=NULL){
//                    fputs(allTokens[t->val]->name,out);
//                    fputs(".node )\n",out);
//                    fputs("Rule: ",out);
//                    i++;
//                }
//                else {
//                    i++;
//                    fputs("Rule: ",out);
//                }
//
//            }
//            else {
//                fputs(" = ",out);
//                fputs(allTokens[t->val]->name,out);
//                fputs(".node\n",out);
//                fputs("Rule: ",out);
//                i++;
//            }
//
//
//        }
//        else {
//            fputc(c,out);
//        }
//    }
//
//    fclose(fp);
//    fclose(out);
//
//
//
//
//    return 0;







    makeAST(pt->head,newRules,NULL);


    recordsHashTable *rht;
    allFunctionsHashTable *afht;
    scopeHashTable *globalScope;
    rht = createRecordsHashTable(5,7);
    afht = createAllFunctionsHashTable(5,7);
    globalScope = createScopeHashTable(5,7);
    quadruple *q = createQuadruple(20);

    makeScopeTables(pt->head,rht,afht,globalScope,q);
    printQuadruple(q);
//    return 0;




    printf("\nRecord Hash Table:\n");
    printRecordHashTable(rht);
    printf("\nAllFunctions Hash Table:\n");
    printAllFunctionsHashTable(afht);
    printf("\nGlobal Scope Hash Table:\n");
    printScopeHashTable(globalScope);
    printf("\n\n");
    return 0;
//
//    quadruple *q = createQuadruple(1);
//    int z = 1, x = 2, c = 3;
//    float a = 1.1, s = 2.2, d = 3.3;
//    int qq = 3, w = 4, e = 5;
//    addEntryQuadruple(q,1,UNION_INT,UNION_INT,UNION_REAL,&qq, &w, &a, 1);
//    addEntryQuadruple(q,5,UNION_INT,UNION_REAL,UNION_REAL,&qq, &a, &s, 2);
//    addEntryQuadruple(q,4,UNION_REAL,UNION_TEMPORARY,UNION_REAL,&d, &w, &a, 3);
//    addEntryQuadruple(q,3,UNION_INT,UNION_ID,UNION_REAL,&c, pt->head, &d, 4);
//    addEntryQuadruple(q,2,UNION_TEMPORARY,UNION_REAL,UNION_REAL,&qq, &a, &s, 5);
//
//    printQuadruple(q);
//    return 0;



    //return 0;


    if (success){
        FILE *parsingtree = fopen(argv[2],"w");
        dfsParseTree(pt->head,parsingtree);
        fclose(parsingtree);
    }


    printf("(a) FIRST and FOLLOW set automated.\n(b) Both lexical and syntax analysis modules implemented.\n(c) Modules working for all test cases.\n");

    while (1){
        printf("\n**************************************************************************\n");
        printf("\nPress option for the defined task:\n");
        printf("1: For removal of comments\n");
        printf("2: For printing the token list\n");
        printf("3: For parsing to verify the syntactic correctness of the input source code\n");
        printf("4: For creating the parse tree and printing it appropriately\n");
        printf("   Any other to exit the program\n\nChoice: ");
        char choice;
        scanf(" %c",&choice);
        printf("\n**************************************************************************\n");
        switch(choice){
            case '1':
                printCommentFreeCode(tklist);
                break;
            case '2':
                printTokenList(tklist,allTokens);
                break;
            case '3':
                //printf("%s",compilationmessage);
                break;
            case '4':
                if (success){
                	dfsParseTree(pt->head,stdout);
                }
                else {
                	printf("Didn't generate parse tree since compilation failed\n");
                }
                break;
            default:
                printf("Exiting Compiler Project. It was a nice learning experience :)\n");
                return 0;
        }

    }
    return 0;
}
