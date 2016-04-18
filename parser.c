//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void createFirstSet(int term, int nonterm, int rule, int firstSetRules[rule+1][term+nonterm], int firstSetTerminals[term+nonterm][term+nonterm], token **allTokens, ruleq **allRules, trie *terminals){
    int epsIndex = searchTrie(terminals,"eps")->dataIndex;
    //printf("%d", epsIndex);
    firstSetTerminals[epsIndex][epsIndex] = 1;

    //calculating nullable productions
    int i,j;
    for (i=0;i<nonterm+term;i++){
        for (j=0;j<nonterm+term;j++){
            firstSetTerminals[i][j] = 0;
        }
        if (!(allTokens[i]->isTerminal) ){
            int k;
            for (k=allTokens[i]->firstRule;k <= allTokens[i]->lastRule;k++){
                if (allRules[k]->head->val == epsIndex){
                    firstSetTerminals[i][epsIndex] = 1;
                    //printf("%s->nullable->%d %d\n",allTokens[i]->name,k,i);
                    break;
                }
            }
        }
    }

    for (i=0;i<nonterm+term;i++){
        if (allTokens[i]->isTerminal){
            firstSetTerminals[i][i] = 1;
        }
    }
    for (i=0;i<rule+1;i++){
        for (j=0;j<nonterm+term;j++){
            firstSetRules[i][j] = 0;
        }
    }

    int changed = 0;
    while (1){
        changed = 0;
        for (i=0;i<nonterm+term;i++){
            if ( !(allTokens[i]->isTerminal) && firstSetTerminals[i][epsIndex] == 0){
                int k;
                for (k=allTokens[i]->firstRule;k <= allTokens[i]->lastRule; k++){
                    int isnull=1;
                    ruleNode *temp = allRules[k]->head;
                    while (temp!=NULL){
                        if (firstSetTerminals[temp->val][epsIndex]){
                            isnull = 1;
                            temp = temp->next;
                        }
                        else {
                            isnull = 0;
                            break;
                        }
                    }
                    if (isnull){
                        firstSetTerminals[i][epsIndex] = 1;
                        //printf("%s->nullable->%d %d\n",allTokens[i]->name,k);
                        changed = 1;
                        break;
                    }
                }
            }

        }
        if (changed == 0){
            break;
        }
    }



    changed = 0;
    while (1){
        changed = 0;
        for (i=0;i<rule;i++){
                ruleNode *temp = allRules[i]->head;
                if ( (allTokens[temp->val])->isTerminal ){
                    if ( !(firstSetRules[i][temp->val]) ){
                        firstSetRules[i][temp->val] = 1;
                        changed = 1;
                    }
                    if ( !(firstSetTerminals[allRules[i]->nontermindex ][temp->val]) ){
                        firstSetTerminals[allRules[i]->nontermindex ][temp->val] = 1;
                        changed = 1;
                    }
                    continue;
                }
                else {
                    int k;
                    do {
                        for (k=0;k<nonterm+term;k++){
                            if (k==epsIndex){
                                continue;
                            }
                            if (!(firstSetRules[i][k]) ){

                                firstSetRules[i][k] = firstSetRules[i][k] | firstSetTerminals[temp->val][k];
                                if (firstSetRules[i][k]){
                                    changed = 1;
                                }
                            }
                            if (!(firstSetTerminals[allRules[i]->nontermindex][k]) ){
                                firstSetTerminals[allRules[i]->nontermindex][k] = firstSetTerminals[allRules[i]->nontermindex][k] | firstSetTerminals[temp->val][k];
                                if (firstSetTerminals[allRules[i]->nontermindex][k]){
                                    changed = 1;
                                }
                            }
                        }

                    }while (temp!=NULL && firstSetTerminals[temp->val][epsIndex]== 1 && (temp = temp->next) );
                }
        }
        if (changed == 0){
            break;
        }
    }
}

void printFirstSetTerminals(int term, int nonterm,int firstSetTerminals[term+nonterm][term+nonterm],token **allTokens){
    int i,j;

    for (i=0;i<term+nonterm;i++){
        if (!(allTokens[i]->isTerminal)){
            printf("%s:",allTokens[i]->name);
            for (j=0;j<term+nonterm;j++){
                if (firstSetTerminals[i][j]){
                    printf("%s, ",allTokens[j]->name);
                }
            }
            printf("\n");
        }

    }
}

void printFirstSetRules(int term, int nonterm, int rule, int firstSetRules[rule+1][term+nonterm],token **allTokens, ruleq **allRules){
    int i,j;
    for (i=0;i<rule+1;i++){
        printf("%s:",allTokens[allRules[i]->nontermindex]->name);
        for (j=0;j<term+nonterm;j++){
            if (firstSetRules[i][j]){
                printf("%s, ",allTokens[j]->name);
            }
        }
        printf("\n");
    }
}

void createFollowSet(int term, int nonterm, int rule, int followSet[nonterm+term][nonterm+term], int firstSetRules[rule+1][term+nonterm], int firstSetTerminals[term+nonterm][term+nonterm], token **allTokens, ruleq **allRules, trie *terminals, trie *nonterminals){
    int i,j,changed;
    int epsIndex = searchTrie(terminals,"eps")->dataIndex;
    for (i=0;i<nonterm+term;i++){
        for (j=0;j<nonterm+term;j++){
            followSet[i][j] = 0;
        }
    }
    followSet[searchTrie(nonterminals,"<program>")->dataIndex][searchTrie(terminals,"$")->dataIndex] = 1;
    changed = 0;
    while (1){
        changed = 0;
        for (i=0;i<rule;i++){
            ruleq *ithrule = allRules[i];
            int nontermindex = ithrule->nontermindex;
            ruleNode *temp = ithrule->head;
            while (temp!=NULL){
                if ( !(allTokens[temp->val]->isTerminal) ){
                    if (temp->next == NULL){
                        int k;
                        for (k=0;k<nonterm+term;k++){
                            if (!(followSet[temp->val][k] ) ){
                                followSet[temp->val][k] = followSet[temp->val][k] | followSet[nontermindex][k];
                                if (followSet[temp->val][k]){
                                    changed = 1;
                                }
                            }
                        }
                    }
                    else {
                        if (allTokens[temp->next->val]->isTerminal){
                            if (!(followSet[temp->val][temp->next->val] ) ){
                                followSet[temp->val][temp->next->val] = 1;
                                changed = 1;
                            }
                        }
                        else {
                            ruleNode *temp2 = temp->next;
                            while (temp2!=NULL){
                                int k;
                                for (k=0;k<nonterm+term;k++){
                                    if (k == epsIndex){
                                        continue;
                                    }
                                    if (!(followSet[temp->val][k] ) ){
                                        followSet[temp->val][k] = followSet[temp->val][k] | firstSetTerminals[temp2->val][k];
                                        if (followSet[temp->val][k]){
                                            changed = 1;
                                        }
                                    }
                                }
                                if (!(firstSetTerminals[temp2->val][epsIndex]) ){
                                    break;
                                }
                                temp2 = temp2->next;
                            }
                            if (temp2 == NULL){
                                int k;
                                for (k=0;k<nonterm+term;k++){
                                    if (!(followSet[temp->val][k] ) ){
                                        followSet[temp->val][k] = followSet[temp->val][k] | followSet[nontermindex][k];
                                        if (followSet[temp->val][k]){
                                            changed = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                temp = temp->next;
            }
        }

        if (changed == 0){
            break;
        }
    }
}

void printFollowSet(int term, int nonterm,int followSet[term+nonterm][term+nonterm],token **allTokens){
        int i,j;

    for (i=0;i<term+nonterm;i++){
        if (!(allTokens[i]->isTerminal)){
            printf("%s:",allTokens[i]->name);
            for (j=0;j<term+nonterm;j++){
                if (followSet[i][j]){
                    printf("%s, ",allTokens[j]->name);
                }
            }
            printf("\n");
        }

    }
}

void createParseTable(int term, int nonterm, int rule, int parseTable[nonterm+term][nonterm+term], int followSet[nonterm+term][nonterm+term], int firstSetRules[rule+1][term+nonterm], int firstSetTerminals[term+nonterm][term+nonterm], token **allTokens, trie *terminals){
    int i,j;
    int epsIndex = searchTrie(terminals,"eps")->dataIndex;
    for (i=0;i<nonterm+term;i++){
        for (j=0;j<nonterm+term;j++){
            parseTable[i][j] = -1;
            if ((allTokens[i]->isTerminal == 0) && (allTokens[j]->isTerminal == 1)){//i is nonterminal and j is terminal
                if (firstSetTerminals[i][j]){
                    int k;
                    for (k=allTokens[i]->firstRule;k<=allTokens[i]->lastRule;k++){
                        if (firstSetRules[k][j]){
                            parseTable[i][j] = k;
                            break;
                        }
                    }
                }
                else {
                    if (firstSetTerminals[i][epsIndex]==1 && followSet[i][j]==1){
                        int k;
                        for (k=allTokens[i]->firstRule;k<=allTokens[i]->lastRule;k++){
                            if (firstSetRules[k][epsIndex]){
                                parseTable[i][j] = k;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

int parseInputSourceCode(parseTree **p, trie *terminals, trie *nonterminals, token **allTokens, ruleq **allRules, int term, int nonterm, int parseTable[nonterm+term][nonterm+term], tokenlist *tklist, int firstSetTerminals[term+nonterm][term+nonterm], int followSet[nonterm+term][nonterm+term], int fd){
    parsedStack *stack = createParsedStack();
    pushStack(stack,searchTrie(terminals,"$")->dataIndex);
    pushStack(stack,searchTrie(nonterminals,"<program>")->dataIndex);
    int epsIndex = searchTrie(terminals,"eps")->dataIndex;
    *p = createParseTree();
    parseTree *pt = *p;

    int error = 0;
    int match = 0;
    char *defaultLexeme = (char *)malloc(sizeof(char) * 5);
    strcpy(defaultLexeme, "----");
    pt->head = createParseTreeNode(NULL,defaultLexeme,-1,NULL,0,0,"<program>",peekStack(stack),-1);
    parseTreeNode *curr = pt->head;

    int index;
    int dolarindex = searchTrie(terminals,"$")->dataIndex;
    int semindex = searchTrie(terminals,"TK_SEM")->dataIndex;
    int epsindex = searchTrie(terminals,"eps")->dataIndex;
    int tk_err = searchTrie(terminals,"TK_ERR")->dataIndex;

    FILE *fp = fdopen(fd,"w");

    while (tklist!=NULL){
        index = peekStack(stack);
        if (index == (tklist->tok).index ){
            match++;
            popStack(stack);
                if (curr != NULL && error==0){
                curr->isLeafNode = 1;
                curr->isNumber = (searchTrie(terminals,"TK_NUM")->dataIndex == index || searchTrie(terminals,"TK_RNUM")->dataIndex == index) ? 1 : 0;
                curr->lexemeCurrentNode = tklist->tok.name;
                curr->lineno = tklist->tok.lineno;
                curr->nodeSymbol = defaultLexeme;
                curr->token = allTokens[index]->name;
                curr->valueIfNumber = (curr->isNumber) ? atof(curr->lexemeCurrentNode) : 0;
//                curr->index = index;
//                curr->replacedNode = index;
               // fprintf(parsingtree,"TerminalNameChange:\n");
              //  dfsParseTree(pt->head,parsingtree);
                while (curr != NULL && curr->next == NULL){
                    curr = curr->parent;
                }
                if (curr != NULL){
                    curr = curr->next;
                }
            }


            tklist = tklist->next;
//            printf("stack: ");
//            printStack(stack->head,allTokens);
//            printf("\n");
        }
        else {
            if (allTokens[index]->isTerminal){
                if (index != dolarindex && tklist->tok.index != dolarindex ){
                    if (tklist->tok.index == tk_err){
                        fprintf(fp,"%s\n",tklist->tok.errmsg);
                    }
                    else {
                        fprintf(fp,"ERROR_5: The token of type <%s> for lexeme <%s> does not match at line number <%d>. The expected token is of type <%s>.\n",
                                allTokens[tklist->tok.index]->name,tklist->tok.name, tklist->tok.lineno, allTokens[index]->name);
                    }
                    error = 1;
                    while (tklist->tok.index != dolarindex && tklist->tok.index != semindex){
                        tklist = tklist->next;
                    }
                    if (tklist->tok.index == semindex){
                        tklist = tklist->next;
                    }
                    while (peekStack(stack)!=dolarindex && peekStack(stack)!=semindex){
                        popStack(stack);
                    }
                    if (peekStack(stack) == semindex){
                        popStack(stack);
                    }
                }
                else if (index == dolarindex){
                    if (tklist->tok.index == tk_err){
                        fprintf(fp,"%s",tklist->tok.errmsg);
                        fprintf(fp," No token is expected\n");
                    }
                    else {
                        fprintf(fp,"ERROR_4: The token of type <%s> for lexeme <%s> does not match at line number <%d>. ",
                               allTokens[tklist->tok.index]->name,tklist->tok.name,  tklist->tok.lineno);
                        fprintf(fp,"No token is expected\n");
                    }
                    error = 1;
                    break;
                }
                else {
                    fprintf(fp,"ERROR_4: Input is consumed while it is expected to have token <%s> at line number <%d>\n",
                            allTokens[index]->name,tklist->tok.lineno);
                    error = 1;
                    break;
                }
            }
            else {
                int ruleno = parseTable[index][(tklist->tok).index];
                if (ruleno == -1){
                    if (tklist->tok.index == dolarindex){
                        fprintf(fp,"ERROR_4: Input is consumed while it is expected to have token %s at line number <%d>\n",
                            allTokens[index]->name,tklist->tok.lineno);
                        error = 1;
                        break;
                    }
                    else {
                        if (tklist->tok.index == tk_err){
                            fprintf(fp,"%s\n",tklist->tok.errmsg);
                        }
                        else {
                            if (firstSetTerminals[index][epsindex] == 1){
                                popStack(stack);
                                continue;
                            }
                            fprintf(fp,"ERROR_5: The token of type <%s> for lexeme <%s> does not match at line number <%d>.",
                                allTokens[tklist->tok.index]->name,tklist->tok.name, tklist->tok.lineno);
                            fprintf(fp," The expected tokens are of type:");
                            int j;
                            for (j=0;j<term+nonterm;j++){
                                if (firstSetTerminals[index][j]){
                                    fprintf(fp," %s",allTokens[j]->name);
                                }
                            }
                            fprintf(fp,".\n");
                        }
                        error = 1;
                        while (tklist->tok.index != dolarindex && tklist->tok.index != semindex && followSet[index][tklist->tok.index]!=1){
                            tklist = tklist->next;
                        }
                        popStack(stack);
                    }
                }
                else {
                    popStack(stack);
                    if (allRules[ruleno]->head->val != epsIndex){
                        addTokensToStack(stack,allRules[ruleno]->head);
                        if (error == 0){
                            curr = addNodeToParseTree(curr,allRules[ruleno]->head,allTokens,defaultLexeme,ruleno);
                        }

                      //  fprintf(parsingtree,"NewInsertion:\n");
                     //   dfsParseTree(pt->head,parsingtree);
                        //break;
//                        printf("ruleno->%d stack: ",ruleno);
//                        printStack(stack->head,allTokens);
//                        printf("\n");
                    }
                    else {
                        if (error == 0){
                            curr = addNodeToParseTree(curr,allRules[ruleno]->head,allTokens,defaultLexeme,ruleno);
                            curr->isLeafNode = 1;
                            curr->isNumber = 0;
                            curr->lexemeCurrentNode = "";
                            curr->lineno = tklist->tok.lineno;
                            curr->nodeSymbol = defaultLexeme;
                            curr->token = "eps";
                            curr->valueIfNumber = 0;
//                            curr->replacedNode =
                            while (curr->next == NULL){
                                curr = curr->parent;
                            }
                            curr = curr->next;
                        }
                        //fprintf(parsingtree,"NewInsertion:\n");
                     //   dfsParseTree(pt->head,parsingtree);
//                        printf("ruleno->%d stack: ",ruleno);
//                        printStack(stack->head,allTokens);
//                        printf("\n");
                    }
                }
            }
        }

    }
    if (!error){
        fprintf(fp,"Compilation successful\n");
        fflush(fp);
        return 1;
    }
    else {
        fflush(fp);
		//*p = NULL;
        return 0;
    }
}
