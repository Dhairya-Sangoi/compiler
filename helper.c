//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

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

#include "dynamicArrayDef.h"
#include "dynamicArray.h"
#include "quadrupleDef.h"
#include "quadruple.h"
#include "scopeHashTableDef.h"
#include "recordsHashTableDef.h"
#include "scopeHashTable.h"
#include "allFunctionsHashTableDef.h"
#include "allFunctionsHashTable.h"

#include "recordsHashTable.h"

#include "registerDef.h"

#include "semanticAnalyzerDef.h"
#include "semanticAnalyzer.h"


char *strupr(char *inp){
    int len = strlen(inp);
    int i;
    for (i=0;i<len;i++){
        if (*(inp+i) >= 97 && *(inp+i) <= 122){
            *(inp+i) = *(inp+i) - 32;
        }
    }
    return inp;
}

int isPrime(int n){
    int i;
    int j = (int) sqrt((float)n);
    for (i=2;i<=j;i++){
        if (n%i == 0){
            return 0;
        }
    }
    return 1;
}

int nextPrime(int n){
    n = n * 2;
    while (isPrime(n)==0){
        n++;
    }
    return n;
}

void makeNewRules(ruleq **newRules, ruleq **allRules, int rule, token **allTokens){
    int i;
    for (i=0;i<rule;i++){
        newRules[i] = createRule(0,allRules[i]->nontermindex);
        ruleNode *rn = allRules[i]->head;
            int count = 0;
            newRules[i]->head = NULL;
            ruleNode *head,*temp;
            temp = NULL;
            head = NULL;
            while (rn != NULL){
                if (canBeAdded(rn->val)  || allTokens[rn->val]->isTerminal == 0){
                    if (temp == NULL){
                        temp = createRuleNode(rn->val);
                        head = temp;
                    }
                    else {
                        temp->next = createRuleNode(rn->val);
                        temp = temp->next;
                    }
                    count++;
                }
                rn = rn->next;
            }
            newRules[i]->index = count;
            newRules[i]->head = head;
    }
}

void printCommentFreeCode(tokenlist* head){
	int lineno;
	tokenlist* ctr = head;

	printf("\n");
	while(ctr->next!=NULL){
		lineno = ctr->tok.lineno;
		while(ctr->next!=NULL && lineno == ctr->tok.lineno){
			printf("%s ", ctr->tok.name);
			ctr = ctr->next;
		}
		printf("\n");
	}
}

token *createToken(char *name, int isTerm, int index, int firstRule){
    token *temp = (token *)malloc(sizeof(token));
    temp->name = (char *)malloc(strlen(name)+1);
    strcpy(temp->name,name);
    temp->index = index;
    temp->isTerminal = isTerm;
    temp->firstRule = firstRule;
    temp->lastRule = firstRule;
    return temp;
}

void printToken(token *temp){
    printf("name:%s isTerm:%d index:%d firstRule:%d lastRule:%d\n",temp->name,temp->isTerminal,temp->index,temp->firstRule,temp->lastRule);

}

void populateDataStructures(char *filename, trie **termptr, trie **nontermptr, token **allTokens, ruleq **allRules, ruleq **newRules, int *t, int *nt, int *rl, char *enumfilename){
    *termptr = createTrie();
    *nontermptr = createTrie();
    trie *terminals = *termptr;
    trie *nonterminals = *nontermptr;

    FILE *fp = fopen(filename, "r");
    FILE *enumfile = fopen(enumfilename,"w");
    fprintf(enumfile,"enum { ");

    int isFirst=1;

    int j;
    for (j=0;j<100;j++){
        allRules[j] = NULL;
    }

    char c;
    char *buf = (char *)malloc(sizeof(char)*30);
    int i=0;
    int rule=0;
    c = fgetc(fp);
    int leftfound=0;
    int nonterm = 0, term = 0;
    int isRight = 0;
    char *enumtokenname ;
    while(c!=EOF){
        if (c == '<' && leftfound == 0 ){
            buf[i] = '\0';
            if (i != 0){
                trieNode *srch = searchTrie(terminals,buf);
                if ( srch == NULL){
                    insert(terminals,buf,term+nonterm);
                    token *temp = createToken(buf,1,nonterm+term,rule);
                    allTokens[nonterm+term] = temp;
                    addRuleNodeEnd(allRules[rule],nonterm+term);
                    if (isFirst){
                        isFirst = 0;
                        fprintf(enumfile, "%s = %d \n", temp->name, temp->index); //change
                    }
                    else {
                        fprintf(enumfile, " , %s = %d \n", temp->name, temp->index);
                    }
                    //search(terminals,buf);
                   // printf("%s:%d\n",buf,strlen(buf));
                    term++;
                }
                else {
                    (allTokens[srch->dataIndex])->lastRule = rule;
                    addRuleNodeEnd(allRules[rule],srch->dataIndex);
                    //printf("hi");
                }
            }
            i=0;
            buf[i] = c;
            i++;
            leftfound=1;
        }
        else if (c == '>' ){
            if (leftfound == 1){
                buf[i] = c;
                i++;
                buf[i] = '\0';

                trieNode *srch = searchTrie(nonterminals,buf);
                if (srch == NULL){
                    if (!isRight){
                        insert(nonterminals,buf,nonterm+term);
                        token *temp = createToken(buf,0,nonterm+term,rule);
                        allTokens[nonterm+term] = temp;
                        allRules[rule] = createRule(rule,nonterm+term);
                        enumtokenname = (char *)malloc(strlen(temp->name));
                        strcpy(enumtokenname,temp->name+1);
                        enumtokenname[strlen(enumtokenname)-1] = '\0';
                        if (isFirst){
                            isFirst = 0;
                            fprintf(enumfile, "%s = %d \n", strupr(enumtokenname), temp->index);
                        }
                        else {
                            fprintf(enumfile, " , %s = %d \n", strupr(enumtokenname), temp->index);
                        }
                    }
                    else {
                        insert(nonterminals,buf,nonterm+term);
                        token *temp = createToken(buf,0,nonterm+term,-1);
                        allTokens[nonterm+term] = temp;
                        addRuleNodeEnd(allRules[rule],nonterm+term);
                        enumtokenname = (char *)malloc(strlen(temp->name));
                        strcpy(enumtokenname,temp->name+1);
                        enumtokenname[strlen(enumtokenname)-1] = '\0';
                        if (isFirst){
                            isFirst = 0;
                            fprintf(enumfile, "%s = %d \n", strupr(enumtokenname), temp->index);
                        }
                        else {
                            fprintf(enumfile, " , %s = %d \n", strupr(enumtokenname), temp->index);
                        }
                    }
                    /////////////////////////////////
                    //search(nonterminals,buf);
                    //printf("%s:%d\n",buf,nonterm);
                    nonterm++;
                }
                else {
                    if (!isRight){
                        if ( (allTokens[srch->dataIndex])->firstRule == -1 ){
                            (allTokens[srch->dataIndex])->firstRule = rule;
                            (allTokens[srch->dataIndex])->lastRule = rule;
                        }
                        else {
                            (allTokens[srch->dataIndex])->lastRule = rule;
                        }
                        allRules[rule] = createRule(rule,srch->dataIndex);
                    }
                    else {
                        addRuleNodeEnd(allRules[rule],srch->dataIndex);
                    }

                }


                leftfound = 0;
                i=0;
            }
            else {
                i=0;
            }

        }
        else if (c == ' ' || c == '\n' || c == '\t' || c=='\r'){
            buf[i] = '\0';
            if (i != 0){
                trieNode *srch = searchTrie(terminals,buf);
                if (srch == NULL){
                    insert(terminals,buf,term+nonterm);
                    token *temp = createToken(buf,1,nonterm+term,rule);
                    allTokens[nonterm+term] = temp;
                    addRuleNodeEnd(allRules[rule],nonterm+term);
                    if (isFirst){
                        isFirst = 0;
                        fprintf(enumfile, "%s = %d \n", strupr(temp->name), temp->index);
                    }
                    else {
                        fprintf(enumfile, " , %s = %d \n", strupr(temp->name), temp->index);
                    }
                    //search(terminals,buf);
                    //printf("%s:%d\n",buf,strlen(buf));
                    term++;
                }
                else {
                    (allTokens[srch->dataIndex])->lastRule = rule;
                    addRuleNodeEnd(allRules[rule],srch->dataIndex);
                }
            }
            if (c == '\n'){
                rule++;
                isRight = 0;
            }
            i=0;
        }
        else if (c == '='){
            i=0;
            isRight = 1;
        }
        else {
            buf[i] = c;
            i++;
        }

        c = fgetc(fp);
        //printf("%c",c);
    }


    insert(terminals,"TK_COMMENT",term+nonterm);
    token *temp = createToken("TK_COMMENT",1,nonterm+term,rule+1);
    allTokens[nonterm+term] = temp;
    term++;
    insert(terminals,"$",term+nonterm);
    temp = createToken("$",1,nonterm+term,rule+1);
    fprintf(enumfile, " , DOLLAR = %d \n", temp->index);
    allTokens[nonterm+term] = temp;
    term++;
    insert(terminals,"TK_ERR",term+nonterm);
    temp = createToken("TK_ERR",1,nonterm+term,rule+1);
    fprintf(enumfile, " , TK_ERR = %d \n", temp->index);
    allTokens[nonterm+term] = temp;
    term++;

    *t = term;
    *nt = nonterm;
    *rl = rule;


    makeNewRules(newRules,allRules,rule,allTokens);

    fprintf(enumfile," } ;");

    fclose(enumfile);
    fclose(fp);


}

tokenlist *createTokenList(int index){
    tokenlist *temp =(tokenlist *)malloc(sizeof(tokenlist));
    temp->tok.index = index;
    temp->tok.name = NULL;
    temp->next = NULL;
    return temp;
}

parseTreeNode *addNodeToParseTree(parseTreeNode *curr,ruleNode *head,token **allTokens, char *lexname, int ruleno){
    while (head!=NULL){
        parseTreeNode *ptn = createParseTreeNode(curr,lexname,-1,lexname,0,0,allTokens[head->val]->name,head->val,ruleno);
        addChild(curr,ptn);
        head = head->next;
    }
    return curr->child;
}

void printTokenList(tokenlist *temp,token **allTokens){
    if (temp == NULL){
		return;
	}
    while (temp->next != NULL){
        printf("Lexeme: %-30s \tToken: %-15s \tLineno: %d\n", temp->tok.name,allTokens[temp->tok.index]->name,temp->tok.lineno);
        temp = temp->next;
    }
}

void addTokensToStack(parsedStack *stack,ruleNode *head){

    if (head == NULL){
        return;
    }
    else {
        addTokensToStack(stack,head->next);
        pushStack(stack,head->val);
        return;
    }
}

void printStack(parsedNode *head, token **allTokens){
    if (head==NULL){
        return;
    }
    else {
        printf("%s ",allTokens[head->index]->name);
        printStack(head->next,allTokens);
        return;
    }
}
