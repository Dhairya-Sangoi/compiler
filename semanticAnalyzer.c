#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#include "parser.h"
#include "lexerDef.h"
#include "lexer.h"



#include "dynamicArrayDef.h"
#include "dynamicArray.h"


#include "enum.h"
#include "operators.h"

//#include "operators.h"

#include <math.h>

#include "scopeHashTableDef.h"
#include "scopeHashTable.h"
#include "allFunctionsHashTableDef.h"
#include "allFunctionsHashTable.h"
#include "recordsHashTableDef.h"
#include "recordsHashTable.h"

#include "semanticAnalyzerDef.h"
#include "quadrupleDef.h"
#include "quadruple.h"

#include "registerDef.h"
#include "register.h"
#include "registers.h"


void makeAST(parseTreeNode *head, ruleq **newRules, parseTreeNode *param){
    if (head == NULL){
        return;
    }
    if (head->index == BOOLEANEXPRESSION){
        parseTreeNode *child1 = head->child;
        parseTreeNode *operand1;// = child1->next;
        parseTreeNode *opert;// = operand1->next->next;
        parseTreeNode *operand2;// = opert->next->next;
        switch(child1->index){
        case TK_OP:
            {
                operand1 = child1->next;
                opert = operand1->next->next;
                operand2 = opert->next->next;
                opert = opert->child;
                operand1->next = operand2;
                operand2->next = NULL;
                operand1->parent = opert;
                operand2->parent = opert;
                opert->child = operand1;
                parseTreeNode *par = head->parent->child;
                if (par == head){
                    head->parent->child = opert;
                    opert->parent = head->parent;
                }
                else {
                    while (par->next != head){
                        par = par->next;
                    }
                    par->next = opert;
                    opert->parent = par->parent;
                }
                opert->next = head->next;
                opert->replacedNode = head->index;
                makeAST(operand1,newRules,NULL);
                makeAST(operand2,newRules,NULL);
                break;
            }
        case VAR:
            {
                operand1 = child1;
                opert = child1->next;
                operand2 = opert->next;
                opert = opert->child;
                operand1->next = operand2;
                operand2->next = NULL;
                operand1->parent = opert;
                operand2->parent = opert;
                opert->child = operand1;
                parseTreeNode *par = head->parent->child;
                if (par == head){
                    head->parent->child = opert;
                    opert->parent = head->parent;
                }
                else {
                    while (par->next != head){
                        par = par->next;
                    }
                    par->next = opert;
                    opert->parent = par->parent;
                }
                opert->next = head->next;
                opert->replacedNode = head->index;
                makeAST(operand1,newRules,NULL);
                makeAST(operand2,newRules,NULL);
                break;
            }
        case TK_NOT:
            {
                opert = child1;
                operand1 = opert->next->next;
                operand1->next = NULL;
                operand1->parent = opert;
                opert->child = operand1;
                parseTreeNode *par = head->parent->child;
                if (par == head){
                    head->parent->child = opert;
                    opert->parent = head->parent;
                }
                else {
                    while (par->next != head){
                        par = par->next;
                    }
                    par->next = opert;
                    opert->parent = par->parent;
                }
                opert->next = head->next;
                opert->replacedNode = head->index;
                makeAST(operand1,newRules,NULL);
                break;
            }
        }
        if (opert->index == TK_LT){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,"<");
        }
        else if (opert->index == TK_LE){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,"<=");
        }
        else if (opert->index == TK_GT){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,">");
        }
        else if (opert->index == TK_GE){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,">=");
        }
        else if (opert->index == TK_EQ){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,"==");
        }
        else if (opert->index == TK_NE){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,"!=");
        }
        else if (opert->index == TK_AND){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 4);
            strcpy(opert->nodeSymbol,"&&&");
        }
        else if (opert->index == TK_OR){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 4);
            strcpy(opert->nodeSymbol,"@@@");
        }
        else if (opert->index == TK_NOT){
            opert->nodeSymbol = (char *)malloc(sizeof(char) * 3);
            strcpy(opert->nodeSymbol,"~");
        }
        return;
    }
    else if (head->index == ARITHMETICEXPRESSION || head->index == TERM){ //
        parseTreeNode *child1 = head->child;
        parseTreeNode *child2 = child1->next;
        child1->next = NULL;
        makeAST(child1, newRules, NULL);
        child1->parent->child = child1->child; //new add
        child1->child->parent = child1->parent;//new add 2
        child1 = child1->parent->child; //new add
        //makeAST(child2, newRules,head->child); //new delete
        makeAST(child2, newRules,child1); //new add
        if (head->index == ARITHMETICEXPRESSION && head->parent->child->index == TK_OP){
            //printf("in\n");
            if (head->parent->index == FACTOR){
//                head->child->next = head->parent->parent->child->next;
//                printf("in in\n");
//                head->parent->parent->child = head->child;
//                head->child->parent = head->parent->parent;


                head->child->next = NULL;
                //printf("in in\n");
                head->parent->child = head->child;
                head->child->parent = head->parent;
            }
        }
        else {
            head->parent->child->next = head->child;
            head->child->parent = head->parent->child->parent;
            head->child->next = NULL;
        }
        return;
    }
    else if (head->index == EXPPRIME || head->index == TERMPRIME){ //
        parseTreeNode *child1 = head->child;
        if (child1->index == EPS){
            head->parent->child = param;
            param->parent = head->parent;
        }
        else {
            parseTreeNode *child2 = child1->next;
            parseTreeNode *child3 = child2->next;
            child1->next = NULL;
            child2->next = NULL;
            makeAST(child1, newRules,NULL);
            child1 = head->child;
            child1->child = param;//child1 is operator.
            param->parent = child1;//param is left operand
            child2->parent = child1;//child2 is right operand
            param->next = child2;
            makeAST(child2, newRules,NULL);

            parseTreeNode *opert = head->child;
            parseTreeNode *operand1 = opert->child;
            parseTreeNode *operand2 = operand1->next;
            if ( (operand1->index == TK_NUM && operand2->index == TK_NUM) || (operand1->index == TK_RNUM && operand2->index == TK_RNUM)  ){
                opert->child = NULL;
                opert->isLeafNode = 1;
                opert->isNumber = 1;
                free(opert->lexemeCurrentNode);
                int oper1len = strlen(operand1->lexemeCurrentNode);
                int oper2len = strlen(operand2->lexemeCurrentNode);
                int oplen = (oper1len > oper2len ) ? oper1len + 3 : oper2len + 3;
                opert->lexemeCurrentNode = (char *)malloc(sizeof(char) * oplen);
                strcpy(opert->nodeSymbol,operand1->nodeSymbol);
                strcpy(opert->token,operand1->token);
                switch(opert->index){
                case TK_PLUS:
                    {
                        sprintf(opert->lexemeCurrentNode,"%lf",operand1->valueIfNumber + operand2->valueIfNumber);
                        opert->valueIfNumber = operand1->valueIfNumber + operand2->valueIfNumber;
                        opert->index = operand1->index;
                        break;
                    }
                case TK_MINUS:
                    {
                        sprintf(opert->lexemeCurrentNode,"%lf",operand1->valueIfNumber - operand2->valueIfNumber);
                        opert->valueIfNumber = operand1->valueIfNumber - operand2->valueIfNumber;
                        opert->index = operand1->index;
                        break;
                    }
                case TK_MUL:
                    {
                        sprintf(opert->lexemeCurrentNode,"%lf",operand1->valueIfNumber * operand2->valueIfNumber);
                        opert->valueIfNumber = operand1->valueIfNumber * operand2->valueIfNumber;
                        opert->index = operand1->index;
                        break;
                    }
                case TK_DIV:
                    {
                        sprintf(opert->lexemeCurrentNode,"%lf",operand1->valueIfNumber / operand2->valueIfNumber);
                        opert->valueIfNumber = operand1->valueIfNumber / operand2->valueIfNumber;
                        opert->index = operand1->index;
                        break;
                    }
                }
            }

            makeAST(child3, newRules,child1);
            head->parent->child = head->child;
            head->child->parent = head->parent;

        }
        return;
    }




    parseTreeNode *curr = head->child;
    while (curr!=NULL){
        parseTreeNode *next = curr->next;
        makeAST(curr,newRules,NULL);
        curr = next;
    }
    if (head->ruleno == -1){
        return;
    }
    else {

        if (head->index == FACTOR || head->index == LOWPRECEDENCEOPERATORS || head->index == TERM || head->index == HIGHPRECEDENCEOPERATORS){
            return;
        }

//head->replacedNodesCount denotes the position of head in the linkedlist starting from head->parent->child (which child is head of its parent)
        parseTreeNode *headnew = (head->replacedNodesCount == 0) ? head : head->parent->child;
        int replacedCount = 1;
        while (replacedCount < head->replacedNodesCount){
            headnew = headnew->next;
            replacedCount++;
        }

        if (headnew->index == TK_PLUS){
            headnew->nodeSymbol = (char *)malloc(sizeof(char) * 8);
            strcpy(headnew->nodeSymbol,"TK_PLUS");
        }
        else if (headnew->index == TK_MINUS){
            headnew->nodeSymbol = (char *)malloc(sizeof(char) * 9);
            strcpy(headnew->nodeSymbol,"TK_MINUS");
        }
        else if (headnew->index == TK_MUL){
            headnew->nodeSymbol = (char *)malloc(sizeof(char) * 7);
            strcpy(headnew->nodeSymbol,"TK_MUL");
        }
        else if (headnew->index == TK_DIV){
            headnew->nodeSymbol = (char *)malloc(sizeof(char) * 7);
            strcpy(headnew->nodeSymbol,"TK_DIV");
        }

        if (newRules[head->ruleno]->index == 0){
            if (head->index == TK_ENDIF){
                //newRules[head->ruleno]->index = 1;
                ASTNode *temp = head->parent;
                temp->child = NULL;
                temp->replacedNode = temp->index;
                temp->index = EPS;
                temp->isLeafNode = 1;
                temp->isNumber = 0;
                temp->token = (char *)malloc(sizeof(char) * 4);
                strcpy(temp->token,"eps");
                temp->next = NULL;
                temp->nodeSymbol = (char *)malloc(sizeof(char) * 5);
                strcpy(temp->nodeSymbol,"----");
                temp->lexemeCurrentNode = temp->nodeSymbol;
                //temp->lexemeCurrentNode = NULL;
            }
            return;
        }
        if (newRules[head->ruleno]->index == 1){
            if (newRules[head->ruleno]->head->val == head->index){
                parseTreeNode *par = headnew->parent;
                parseTreeNode *parpar = par->parent;
                parseTreeNode *temp = parpar->child;
                headnew->replacedNode = par->index; //headnew->replacedNode indicates the node which it replaced
                if (par == temp){
                    parpar->child = headnew;
                    //headnew->parent = parpar; //new add
                    headnew->next = par->next;
                    par->replacedNodesCount = 1;
                }
                else {
                    int count = 2;
                    while (temp->next != par){
                        temp = temp->next;
                        count++;
                    }
                    temp->next = headnew;
                    //headnew->parent = parpar;
                    headnew->next = par->next;
                    par->replacedNodesCount = count;
                }
                headnew->parent = parpar;
            }

        }
        else {
            if (headnew->next == NULL){
                if (head->ruleno == 36 || head->ruleno == 67 || head->ruleno == 68 || head->ruleno == 69 ){
                    return;
                }
                parseTreeNode *temp = headnew->parent->child;
                parseTreeNode *prev = NULL;
                parseTreeNode *par = headnew->parent;
                ruleNode *list = newRules[head->ruleno]->head;
                int count = 0;
                int limit = newRules[head->ruleno]->index;
                while (count < limit){
                    if (temp->replacedNode == list->val){
                        prev = temp;
                        temp = temp->next;
                        list = list->next;
                        count++;
                    }
                    else {
                        if (prev == NULL){
                            par->child = temp->next;
                        }
                        else {
                            prev->next = temp->next;
                        }
                        temp = temp->next;
                    }
                }
                prev->next = NULL;
            }
        }
    }
    return;
}

int canBeAdded(int index){
    return (index == TK_FUNID
            || index == TK_ID
            || index == TK_REAL
            || index == TK_INT
            || index == TK_RECORDID
            || index == TK_FIELDID
            || index == TK_GLOBAL
            || index == TK_READ
            || index == TK_WRITE
            || index == TK_NUM
            || index == TK_RNUM
            || index == TK_NOT
            || index == TK_OR
            || index == TK_AND
            || index == TK_LT
            || index == TK_LE
            || index == TK_EQ
            || index == TK_GT
            || index == TK_GE
            || index == TK_NE
            || index == TK_DIV
            || index == TK_MUL
            || index == TK_MINUS
            || index == TK_PLUS
            || index == EPS);
}

void dfsForScopeTables(ASTNode *head, recordsHashTable *rht, allFunctionsHashTable *afht, scopeHashTable *currentScope, scopeHashTable *globalScope, typeNode **tn, int *recWidth, IONode **ion, scopeHashTable *recFields, char *fname, int *offset){
    switch (head->index){
    case PROGRAM:
        {
//            rht = createRecordsHashTable(5,7);
//            afht = createAllFunctionsHashTable(5,7);
//            globalScope = createScopeHashTable(5,7);
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            *offset = -1;
            dfsForScopeTables(child1,rht,afht,NULL,globalScope,NULL,NULL,NULL,NULL,NULL,offset);

            char *fn = (char *)malloc(sizeof(char)*6);
            strcpy(fn,"_main");
            currentScope = createScopeHashTable(5,7);
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            allFunctionsHashNode *temp = createAllFunctionsHashNode(fn,currentScope);
            insertAllFunctionsHashTable(temp,afht);
            *offset = -1; // change to 0 to work if only main function
            dfsForScopeTables(child2,rht,afht,currentScope,globalScope,NULL,NULL,NULL,NULL,fn,offset);
            *offset = -1;
            break;
        }

    case FUNCTION:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            ASTNode *child4 = child3->next;
            currentScope = createScopeHashTable(5,7);
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            allFunctionsHashNode *temp = createAllFunctionsHashNode(child1->lexemeCurrentNode,currentScope);
            int inserted = insertAllFunctionsHashTable(temp,afht);
            if (inserted == 0){
                printf("Error:9 Function overloading\n");
            }
            fname = child1->lexemeCurrentNode;
            dfsForScopeTables(child2,rht,afht,currentScope,globalScope,NULL,NULL,&(temp->data->input_params),NULL,fname,offset);
            dfsForScopeTables(child3,rht,afht,currentScope,globalScope,NULL,NULL,&(temp->data->output_params),NULL,fname, offset);
            dfsForScopeTables(child4,rht,afht,currentScope,globalScope,NULL,NULL,NULL,NULL,fname, offset);
            break;
        }
    case PARAMETER_LIST:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            scopeHashNode *temp = createScopeHashNode(child2->lexemeCurrentNode,child1->lexemeCurrentNode,child2,offset);
            int inserted = insertScopeHashTable(temp,currentScope);
            if (inserted == 0){
                printf("Error 1: Identifier Redeclaration\n");
            }
            IONode **headptr = ion;
            *ion = createIONode(child1->lexemeCurrentNode);
            dfsForScopeTables(child3,rht,afht,currentScope,globalScope,NULL,NULL,&((*ion)->next),NULL,fname, offset);
            break;
        }

    case DECLARATION:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            scopeHashNode *temp = createScopeHashNode(child2->lexemeCurrentNode,child1->lexemeCurrentNode,child2,offset);
            switch (child1->index){
            case TK_INT:
                {
                    *offset = *offset + INT_SIZE ;
                    break;
                }
            case TK_REAL:
                {
                    *offset = *offset + REAL_SIZE ;
                    break;
                }
            default:
                {
                    recordHashNode *rhn = searchEntryRecordsHashTable(child1->lexemeCurrentNode,rht);
                    if (rhn != NULL){
                        *offset = *offset + rhn->data->width;
                    }
                    break;
                }
            }
            if (child3->index == EPS){
                int inserted = insertScopeHashTable(temp,currentScope);
                if (inserted == 0){
                    printf("Error 1: Identifier Redeclaration\n");
                }
            }
            else {
                int inserted = insertScopeHashTable(temp,globalScope);
                if (inserted == 0){
                    printf("Error 3: Global Identifier Redeclaration\n");
                }
            }
            break;
        }

    case TYPEDEFINITION:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            recordHashNode *temp = createRecordHashNode(child1->lexemeCurrentNode);
            insertRecordsHashTable(temp,rht);
            dfsForScopeTables(child2,rht,afht,currentScope,globalScope,&(temp->data->type),&(temp->data->width),NULL,temp->data->fields,fname, offset);
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            break;
        }

    case FIELDDEFINITIONS:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
//            int prev_offset = *offset;
//            *offset = 0;
            scopeHashNode *temp1 = createScopeHashNode(child1->child->next->lexemeCurrentNode,child1->child->lexemeCurrentNode,child1->child,recWidth);
            insertScopeHashTable(temp1,recFields);
            *tn = createTypeNode(child1->child->index,child1->child->next->lexemeCurrentNode);
            tn = &((*tn)->next);
            *recWidth = *recWidth + ((child1->child->index == TK_INT) ? INT_SIZE : REAL_SIZE) ;
            recFields->offset = *recWidth;

            scopeHashNode *temp2 = createScopeHashNode(child2->child->next->lexemeCurrentNode,child2->child->lexemeCurrentNode,child1->child,recWidth);
            insertScopeHashTable(temp2,recFields);
            *tn = createTypeNode(child2->child->index,child2->child->next->lexemeCurrentNode);
            tn = &((*tn)->next);
            *recWidth = *recWidth + ((child2->child->index == TK_INT) ? INT_SIZE : REAL_SIZE) ;
            recFields->offset = *recWidth;
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            dfsForScopeTables(child3,rht,afht,currentScope,globalScope,tn,recWidth,NULL,recFields,fname, offset);
            break;
        }

    case MOREFIELDS:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;

            scopeHashNode *temp1 = createScopeHashNode(child1->child->next->lexemeCurrentNode,child1->child->lexemeCurrentNode,child1->child,recWidth);
            insertScopeHashTable(temp1,recFields);
            *tn = createTypeNode(child1->child->index,child1->child->next->lexemeCurrentNode);
            tn = &((*tn)->next);
            *recWidth = *recWidth + ((child1->child->index == TK_INT) ? INT_SIZE : REAL_SIZE );
            recFields->offset = *recWidth;
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            dfsForScopeTables(child2,rht,afht,currentScope,globalScope,tn,recWidth,NULL,recFields,fname, offset);
            break;
        }
    case FUNCALLSTMT:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            if (strcmp(fname,child2->lexemeCurrentNode) == 0){
                printf("Error:10 Function invoked recursively\n");
            }
            allFunctionsHashNode *temp = searchEntryAllFunctionsHashTable(child2->lexemeCurrentNode,afht);
            if (temp == NULL){
                printf("Error:14 Function definition must precede the use\n");
            }
            //printf("%d %d \n",currentScope->elements, currentScope->f);
        }
    default:
        {
            ASTNode *curr = head->child;
            while (curr!=NULL){
                //parseTreeNode *next = curr->next;
                dfsForScopeTables(curr,rht,afht,currentScope,globalScope,tn,recWidth,ion,NULL,fname, offset);
                //printf("%d %d \n",(currentScope != NULL ) ? (currentScope->elements, currentScope->f) : (0,0));
                curr = curr->next;
            }
            break;
        }

    }
}
////////////////////////////////////////////////////

void dfsForSemanticAnalysis(ASTNode *head, recordsHashTable *rht, allFunctionsHashTable *afht, scopeHashTable *currentScope, scopeHashTable *globalScope, char *fname, char **argtype, whileScopeList *headWhile, whileScopeList *tailWhile, int mylabel, int nextlabel, int grandnextlabel, int *ptrAvailableLabels, int *ptrAvailableTemporary, int jumpIfTrue, int jumpIfFalse, int shouldOutputLabel, int *success, quadruple *q, dynamicArray *da, int *maxoffset){
    switch (head->index){
    case PROGRAM:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            dfsForSemanticAnalysis(child1,rht,afht,NULL,globalScope,NULL,NULL,NULL,NULL, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            char *fn = (char *)malloc(sizeof(char)*6);
            strcpy(fn,"_main");
            currentScope = searchEntryAllFunctionsHashTable(fn,afht)->data->scope;
            dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fn,NULL,NULL,NULL, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            break;
        }

    case FUNCTION:
        {
            ASTNode *child1 = head->child;
//            ASTNode *child2 = child1->next;
//            ASTNode *child3 = child2->next;
//            ASTNode *child4 = child3->next;
            currentScope = searchEntryAllFunctionsHashTable(child1->lexemeCurrentNode,afht)->data->scope;
            fname = child1->lexemeCurrentNode;
            while (child1!=NULL){
                dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,NULL,NULL,NULL, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                child1 = child1->next;
            }
            ASTNode *outparams = head->child->next->next;
            while (outparams->index != EPS){
                scopeHashNode *temp = searchEntryScopeHashTable(outparams->child->next->lexemeCurrentNode,currentScope);
                if (temp != NULL){
                    if (temp->data->isChanged != 1){
                        *success = 0; printf("Error 5:The parameters being returned by a function must be assigned a value\n");
                        break;
                    }
                    else {
                        temp->data->isChanged = 0;
                        outparams = outparams->child->next->next;
                    }
                }
                else {
                    break;
                }
            }
            break;
        }

    case PARAMETER_LIST:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            //printf("%d %d \n",currentScope->elements, currentScope->f);
            //scopeHashNode *temp = createScopeHashNode(child2->lexemeCurrentNode,child1->lexemeCurrentNode,child2,offset);
            //int inserted = insertScopeHashTable(temp,currentScope);
//            if (inserted == 0){
//                printf("Error 1: Identifier Redeclaration\n");
//            }
//            IONode **headptr = ion;
//            *ion = createIONode(child1->lexemeCurrentNode);
            if (child1->index == TK_RECORDID){
                recordHashNode *r = searchEntryRecordsHashTable(child1->lexemeCurrentNode,rht);
                if (r==NULL){
                    *success = 0; printf("Error 12: Record definition not found\n");
                }
                else {
                    scopeHashNode *t = searchEntryScopeHashTable(child2->lexemeCurrentNode,currentScope);
                    if (t != NULL){
                        t->data->offset = currentScope->offset;
                        currentScope->offset = currentScope->offset + r->data->width;
                    }
                }
            }
            else {
                scopeHashNode *t = searchEntryScopeHashTable(child2->lexemeCurrentNode,currentScope);
                if (t != NULL){
                    t->data->offset = currentScope->offset;
                    currentScope->offset = currentScope->offset + ((child1->index == TK_INT) ? INT_SIZE : REAL_SIZE );
                }
            }
            //dfsForScopeTables(child3,rht,afht,currentScope,globalScope,NULL,NULL,&((*ion)->next),NULL,fname, offset);
            dfsForSemanticAnalysis(child3,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            break;
        }

    case DECLARATION:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            scopeHashTable *searchScope = globalScope;
            if (child3->index == EPS){
                scopeHashNode *exists = searchEntryScopeHashTable(child2->lexemeCurrentNode,globalScope);
                if (exists != NULL){
                    *success = 0; printf("Error 3: Global Identifier Redeclaration. Trying to declare a local variable with global variable name.\n");
                }
                searchScope = currentScope;
            }
            if (child1->index == TK_RECORDID){
                recordHashNode *r = searchEntryRecordsHashTable(child1->lexemeCurrentNode,rht);
                if (r==NULL){
                    *success = 0; printf("Error 12: Record definition not found\n");
                }
                else {
                    scopeHashNode *t = searchEntryScopeHashTable(child2->lexemeCurrentNode,searchScope);
                    if (t != NULL){
                        t->data->offset = searchScope->offset;
                        searchScope->offset = searchScope->offset + r->data->width;
                    }
                }
            }
            else {
                scopeHashNode *t = searchEntryScopeHashTable(child2->lexemeCurrentNode,searchScope);
                if (t != NULL){
                    t->data->offset = searchScope->offset;
                    searchScope->offset = searchScope->offset + ((child1->index == TK_INT) ? INT_SIZE : REAL_SIZE );
                }
            }


            //printf("%d %d   ",currentScope->elements,currentScope->f);
            break;
        }

    case TK_ID:
        {
            int parent = head->parent->index;
            if (parent != DECLARATION){
                scopeHashNode *exists = searchEntryScopeHashTable(head->lexemeCurrentNode,currentScope);
                if (exists == NULL){
                    exists = searchEntryScopeHashTable(head->lexemeCurrentNode,globalScope);
                    if (exists == NULL){
                        *success = 0; printf("Error 2: Identifier must be declared before its use\n");
                    }
                    else {
                        if (parent == SINGLEORRECID){
                            ASTNode *parpar = head->parent->parent;
                            //int parpar = head->parent->parent->index;
                            if (parpar->index == ASSIGNMENTSTMT || (parpar->index == IOSTMT && parpar->child->index == TK_READ)){
                                exists->data->isChanged = 1;
                                whileScopeList *tt = headWhile;
                                while (tt != NULL){
                                    if (tt->isChanged == 0){
                                        scopeHashNode *d = searchEntryScopeHashTable(head->lexemeCurrentNode,tt->wsht);
                                        if (d!=NULL){
                                            tt->isChanged = 1;
                                        }
                                    }
                                    tt = tt->next;
                                }
                            }
                        }
                    } //not needed since global variables cannot be returned


                }
                else {
                    if (parent == SINGLEORRECID){
                        ASTNode *parpar = head->parent->parent;
                        if (parpar->index == ASSIGNMENTSTMT || (parpar->index == IOSTMT && parpar->child->index == TK_READ) ){
                            exists->data->isChanged = 1;
                            whileScopeList *tt = headWhile;
                            while (tt != NULL){
                                if (tt->isChanged == 0){
                                    scopeHashNode *d = searchEntryScopeHashTable(head->lexemeCurrentNode,tt->wsht);
                                    if (d!=NULL){
                                        tt->isChanged = 1;
                                    }
                                }
                                tt = tt->next;
                            }
                        }
                    }
                }
            }
            break;
        }

    case FUNCALLSTMT:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            ASTNode *child3 = child2->next;
            IONode *in = searchEntryAllFunctionsHashTable(child2->lexemeCurrentNode,afht)->data->input_params;
            IONode *on = searchEntryAllFunctionsHashTable(child2->lexemeCurrentNode,afht)->data->output_params;
            if (on == NULL){
                if (child1->index != EPS){
                    *success = 0; printf("Error 6: The function that does not return any value, must be invoked appropriately.\n");
                }
            }
            else {
                child1 = child1->child;
                while (child1!=NULL && on !=NULL){
                    scopeHashNode *temp = searchEntryScopeHashTable(child1->lexemeCurrentNode,currentScope);
                    if (temp == NULL){
                        temp = searchEntryScopeHashTable(child1->lexemeCurrentNode,globalScope);
                    }
                    if (temp != NULL){
                        temp->data->isChanged = 1;
                        whileScopeList *tt = headWhile;
                        while (tt != NULL){
                            if (tt->isChanged == 0){
                                scopeHashNode *d = searchEntryScopeHashTable(child1->lexemeCurrentNode,tt->wsht);
                                if (d!=NULL){
                                    tt->isChanged = 1;
                                }
                            }
                            tt = tt->next;
                        }

                        if (strcmp(temp->data->typeName,on->param) != 0){
                            *success = 0; printf("Error 4:output params type not matching\n");
                            break;
                        }
                        else {
                            child1 = child1->next->child;
                            on = on->next;
                        }
                    }
                    else {
                        *success = 0; printf("Error 4:output params not matching. no such identifier exists\n");
                        break;
                    }

                }
                if (on != NULL && child1 == NULL){
                    *success = 0; printf("Error 4:output params not matching. actual list long\n");
                }
                if (child1 != NULL && on == NULL){
                    *success = 0; printf("Error 4:output params not matching. used list long\n");
                }
            }

            ////////////////

                child3 = child3->child;
                child1 = child3;
                on = in;
                while (child1!=NULL && on !=NULL){
                    scopeHashNode *temp = searchEntryScopeHashTable(child1->lexemeCurrentNode,currentScope);
                    if (temp == NULL){
                        temp = searchEntryScopeHashTable(child1->lexemeCurrentNode,globalScope);
                    }
                    if (temp != NULL){
                        if (strcmp(temp->data->typeName,on->param) != 0){
                            *success = 0; printf("Error 4:input params type not matching\n");
                            break;
                        }
                        else {
                            child1 = child1->next->child;
                            on = on->next;
                        }
                    }
                    else {
                        *success = 0; printf("Error 4:input params not matching. no such identifier exists\n");
                        break;
                    }

                }
                if (on != NULL && child1 == NULL){
                    *success = 0; printf("Error 4:input params not matching. actual list long\n");
                }
                if (child1 != NULL && on == NULL){
                    *success = 0; printf("Error 4:input params not matching. used list long\n");
                }


            break;
        }

    case ELSEPART:
    case OTHERSTMTS:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;

            int mylbl1,mylbl2,nxtlbl1,nxtlbl2,grndlbl1,grndlbl2;
            mylbl1 = mylabel;
            if (child2->index == EPS){
                if (grandnextlabel == -1){
                    //nxtlbl1 = *ptrAvailableLabels;
                    nxtlbl1 = *ptrAvailableLabels;
                    // (*ptrAvailableLabels)++;
                }
                else {
                    nxtlbl1 = grandnextlabel;
                }
            }
            else {
                nxtlbl1 = *ptrAvailableLabels;
            }
            if (grandnextlabel == -1){
                grndlbl1 = *ptrAvailableLabels;
            }
            else {
                grndlbl1 = grandnextlabel;
            }
            if (child2->index == EPS){
                if (grandnextlabel == -1){
                    (*ptrAvailableLabels)++;
                }
            }
            else {
                //nxtlbl2 = *ptrAvailableLabels; //previous version
                mylbl2 = *ptrAvailableLabels; //changed version
                (*ptrAvailableLabels)++;
            }
            if (head->index == ELSEPART){
                if (child2->index == EPS){
                    grndlbl1 = grandnextlabel;
                }
                else {
                    grndlbl1 = mylbl2;
                }
            }
            nxtlbl2 = -1;
            grndlbl2 = grandnextlabel;

            dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylbl1, nxtlbl1, grndlbl1, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            if (child1->index == CONDITIONALSTMT || child1->index == ITERATIVESTMT){
                shouldOutputLabel = 1;
            }
            dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylbl2, nxtlbl2, grndlbl2, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            //add instruction for generating label if otherstmts's 2nd child goes to eps and grandnextlabel = -1
            if (head->index == OTHERSTMTS && child2->index == EPS && grandnextlabel == -1){
                addEntryQuadruple(q,LABEL,-1,-1,-1,NULL,NULL,NULL,nxtlbl1);
                //addEntryDynamicArray(nxtlbl1,da);
            }
            break;
        }

    case ASSIGNMENTSTMT:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            char *arg1 = (char *)malloc(sizeof(char) * 32);
            char *arg2 = (char *)malloc(sizeof(char) * 32);

            ASTNode *c1 = child1->child;
            ASTNode *c2 = c1->next;

            int opcode;


            //change the isChanged attribute of c1.
            if (c2->index == EPS){
                scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                if (t == NULL){
                    t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                    if (t == NULL){
                        strcpy(arg1,"error");
                    }
                    else {
                        whileScopeList *tt = headWhile;
                        while (tt != NULL){
                            if (tt->isChanged == 0){
                                scopeHashNode *d = searchEntryScopeHashTable(c1->lexemeCurrentNode,tt->wsht);
                                if (d!=NULL){
                                    tt->isChanged = 1;
                                }
                            }
                            tt = tt->next;
                        }
                        if (strcmp(t->data->typeName,"int") == 0){
                            strcpy(arg1,"int");
                            opcode = ASSIGN_INT;
                        }
                        else if (strcmp(t->data->typeName,"real") == 0){
                            strcpy(arg1,"real");
                            opcode = ASSIGN_REAL;
                        }
                        else {
                            recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                            if (r == NULL){
                                strcpy(arg1,"error");
                            }
                            else {
                                arg1 = t->data->typeName;
                                opcode = ASSIGN_RECORD;
                            }
                        }
                    }
                }
                else {
                    whileScopeList *tt = headWhile;
                    while (tt != NULL){
                        if (tt->isChanged == 0){
                            scopeHashNode *d = searchEntryScopeHashTable(c1->lexemeCurrentNode,tt->wsht);
                            if (d!=NULL){
                                tt->isChanged = 1;
                            }
                        }
                        tt = tt->next;
                    }
                    t->data->isChanged = 1;
                    if (strcmp(t->data->typeName,"int") == 0){
                        strcpy(arg1,"int");
                        opcode = ASSIGN_INT;
                    }
                    else if (strcmp(t->data->typeName,"real") == 0){
                        strcpy(arg1,"real");
                        opcode = ASSIGN_REAL;
                    }
                    else {
                        recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                        if (r == NULL){
                            strcpy(arg1,"error");
                        }
                        else {
                            arg1 = t->data->typeName;
                            opcode = ASSIGN_RECORD;
                        }
                    }
                }
            }
            else {
                scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                if (t == NULL){
                    t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                    if (t == NULL){
                        strcpy(arg1,"error");
                    }
                    else {

                        whileScopeList *tt = headWhile;
                        while (tt != NULL){
                            if (tt->isChanged == 0){
                                scopeHashNode *d = searchEntryScopeHashTable(c1->lexemeCurrentNode,tt->wsht);
                                if (d!=NULL){
                                    tt->isChanged = 1;
                                }
                            }
                            tt = tt->next;
                        }

                        if (strcmp(t->data->typeName,"int") == 0){
                            strcpy(arg1,"error");
                        }
                        else if (strcmp(t->data->typeName,"real") == 0){
                            strcpy(arg1,"error");
                        }
                        else {
                            recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                            if (r == NULL){
                                strcpy(arg1,"error");
                            }
                            else {
                                scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                if (rf == NULL){
                                    strcpy(arg1,"error");
                                }
                                else {
                                    arg1 = rf->data->typeName;
                                    if (strcmp(arg1,"int") == 0){
                                        opcode = ASSIGN_INT;
                                    }
                                    else if (strcmp(arg1,"real") == 0){
                                        opcode = ASSIGN_REAL;
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    whileScopeList *tt = headWhile;
                    while (tt != NULL){
                        if (tt->isChanged == 0){
                            scopeHashNode *d = searchEntryScopeHashTable(c1->lexemeCurrentNode,tt->wsht);
                            if (d!=NULL){
                                tt->isChanged = 1;
                            }
                        }
                        tt = tt->next;
                    }
                    t->data->isChanged = 1;
                    if (strcmp(t->data->typeName,"int") == 0){
                        strcpy(arg1,"error");
                    }
                    else if (strcmp(t->data->typeName,"real") == 0){
                        strcpy(arg1,"error");
                    }
                    else {
                        recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                        if (r == NULL){
                            strcpy(arg1,"error");
                        }
                        else {
                            scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                            if (rf == NULL){
                                strcpy(arg1,"error");
                            }
                            else {
                                arg1 = rf->data->typeName;
                                if (strcmp(arg1,"int") == 0){
                                    opcode = ASSIGN_INT;
                                }
                                else if (strcmp(arg1,"real") == 0){
                                    opcode = ASSIGN_REAL;
                                }
                            }
                        }
                    }
                }
            }

            dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            switch (child2->index){
            case ALLVAR:
                {
                    c1 = child2->child;
                    c2 = c1->next;
                    if (c2->index == EPS){
                        scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                        if (t == NULL){
                            t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                            if (t == NULL){
                                strcpy(arg2,"error");
                            }
                            else {
                                if (strcmp(t->data->typeName,"int") == 0){
                                    strcpy(arg2,"int");
                                }
                                else if (strcmp(t->data->typeName,"real") == 0){
                                    strcpy(arg2,"real");
                                }
                                else {
                                    recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                    if (r == NULL){
                                        strcpy(arg2,"error");
                                    }
                                    else {
                                        arg2 = t->data->typeName;
                                    }
                                }
                            }
                        }
                        else {
                            if (strcmp(t->data->typeName,"int") == 0){
                                strcpy(arg2,"int");
                            }
                            else if (strcmp(t->data->typeName,"real") == 0){
                                strcpy(arg2,"real");
                            }
                            else {
                                recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                if (r == NULL){
                                    strcpy(arg2,"error");
                                }
                                else {
                                    arg2 = t->data->typeName;
                                }
                            }
                        }
                    }
                    else {
                        scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                        if (t == NULL){
                            t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                            if (t == NULL){
                                strcpy(arg2,"error");
                            }
                            else {
                                if (strcmp(t->data->typeName,"int") == 0){
                                    strcpy(arg2,"error");
                                }
                                else if (strcmp(t->data->typeName,"real") == 0){
                                    strcpy(arg2,"error");
                                }
                                else {
                                    recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                    if (r == NULL){
                                        strcpy(arg2,"error");
                                    }
                                    else {
                                        scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                        if (rf == NULL){
                                            strcpy(arg2,"error");
                                        }
                                        else {
                                            arg2 = rf->data->typeName;
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (strcmp(t->data->typeName,"int") == 0){
                                strcpy(arg2,"error");
                            }
                            else if (strcmp(t->data->typeName,"real") == 0){
                                strcpy(arg2,"error");
                            }
                            else {
                                recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                if (r == NULL){
                                    strcpy(arg2,"error");
                                }
                                else {
                                    scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                    if (rf == NULL){
                                        strcpy(arg2,"error");
                                    }
                                    else {
                                        arg2 = rf->data->typeName;
                                    }
                                }
                            }
                        }
                    }
                    dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                    break;
                }
            case TK_NUM:
                {
                    strcpy(arg2,"int");
                    break;
                }
            case TK_RNUM:
                {
                    strcpy(arg2,"real");
                    break;
                }
            default:
                {
                    dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,&arg2,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset); // mylabel changed to -1
                    mylabel = -1;
                    break;
                }
            }

            if (strcmp(arg1,arg2) == 0){
                if (strcmp(arg1,"error") == 0){
                    *success = 0; printf("Error 13: The right hand side expression of an assignment statement must be of the same type as that of the left hand side identifier.\n");
                }
                /////////////////////////////////////////////////////////////////////////////////////////
                void *res;
                if (child1->child->next->index == EPS){
                    res = child1->child;
                }
                else {
                    res = child1;
                }
                int label = -1;
                if (shouldOutputLabel){
                    label = mylabel;
                }
                if (*ptrAvailableTemporary != 1){
                    if (opcode == ASSIGN_INT){
                        *maxoffset = (*maxoffset < (*ptrAvailableTemporary-1) * INT_SIZE) ? (*ptrAvailableTemporary-1) * INT_SIZE : *maxoffset;
                    }
                    else if (opcode == ASSIGN_REAL){
                        *maxoffset = (*maxoffset < (*ptrAvailableTemporary-1) * REAL_SIZE) ? (*ptrAvailableTemporary-1) * REAL_SIZE : *maxoffset;
                    }
                    else {
                        recordHashNode *rhn = searchEntryRecordsHashTable(arg1,rht);
                        int sz = 0;
                        if (rhn != NULL){
                            sz = rhn->data->width;
                        }
                        *maxoffset = (*maxoffset < (*ptrAvailableTemporary-1) * sz) ? (*ptrAvailableTemporary-1) * sz : *maxoffset;
                    }
                    *ptrAvailableTemporary -= 1;
                    addEntryQuadruple(q,opcode,UNION_TEMPORARY,-1,UNION_ID,ptrAvailableTemporary,NULL,res,label);
                }
                else {
                    if (child2->index == TK_RNUM){
                        addEntryQuadruple(q,opcode,UNION_REAL,-1,UNION_ID,&(child2->valueIfNumber),NULL,res,label);
                    }
                    else if (child2->index == TK_NUM){
                        int a = child2->valueIfNumber;
                        addEntryQuadruple(q,opcode,UNION_INT,-1,UNION_ID,&a,NULL,res,label);
                    }
                    else { ////////////
                        if (child2->child->next->index == EPS){
                            addEntryQuadruple(q,opcode,UNION_ID,-1,UNION_ID,child2->child,NULL,res,label);
                        }
                        else {
                            addEntryQuadruple(q,opcode,UNION_ID,-1,UNION_ID,child2,NULL,res,label);
                        }
                    }
                }
                *ptrAvailableTemporary = 1;

            }
            else {
                *success = 0; printf("Error 13: The right hand side expression of an assignment statement must be of the same type as that of the left hand side identifier.\n");

            }
            break;

        }

    case TK_MINUS:
    case TK_PLUS:
    case TK_MUL:
    case TK_DIV:

        {
            //printf("\nin +\n");
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            char *arg1 = (char *)malloc(sizeof(char) * 32);
            char *arg2 = (char *)malloc(sizeof(char) * 32);

            int opcode;
            void *oper1, *oper2, *resu;
            int op1,op2, resul;
            int op1typ, op2typ, restyp;

            ASTNode *c1 = child1->child;
            //ASTNode *c2 = c1->next;
            switch (child1->index){
            case ALLVAR:
                {
                    ASTNode *c2 = c1->next;
                    op1typ = UNION_ID;
                    if (c2->index == EPS){
                        oper1 = c1;
                        scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                        if (t == NULL){
                            t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                            if (t == NULL){
                                strcpy(arg1,"error");
                            }
                            else {
                                if (strcmp(t->data->typeName,"int") == 0){
                                    strcpy(arg1,"int");
                                    switch(head->index){
                                    case TK_PLUS:
                                        {
                                            opcode = PLUS_INT;
                                            break;
                                        }
                                    case TK_MINUS:
                                        {
                                            opcode = MINUS_INT;
                                            break;
                                        }
                                    case TK_MUL:
                                        {
                                            opcode = MULT_INT;
                                            break;
                                        }
                                    case TK_DIV:
                                        {
                                            opcode = DIV_INT;
                                            break;
                                        }
                                    }
                                }
                                else if (strcmp(t->data->typeName,"real") == 0){
                                    strcpy(arg1,"real");
                                    switch(head->index){
                                    case TK_PLUS:
                                        {
                                            opcode = PLUS_REAL;
                                            break;
                                        }
                                    case TK_MINUS:
                                        {
                                            opcode = MINUS_REAL;
                                            break;
                                        }
                                    case TK_MUL:
                                        {
                                            opcode = MULT_REAL;
                                            break;
                                        }
                                    case TK_DIV:
                                        {
                                            opcode = DIV_REAL;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                    if (r == NULL){
                                        strcpy(arg1,"error");
                                    }
                                    else {
                                        arg1 = t->data->typeName;
                                        switch(head->index){
                                        case TK_PLUS:
                                            {
                                                opcode = PLUS_RECORD;
                                                break;
                                            }
                                        case TK_MINUS:
                                            {
                                                opcode = MINUS_RECORD;
                                                break;
                                            }
                                        case TK_MUL:
                                            {
                                                opcode = MULT_RECORD;
                                                break;
                                            }
                                        case TK_DIV:
                                            {
                                                opcode = DIV_RECORD;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (strcmp(t->data->typeName,"int") == 0){
                                strcpy(arg1,"int");
                                switch(head->index){
                                case TK_PLUS:
                                    {
                                        opcode = PLUS_INT;
                                        break;
                                    }
                                case TK_MINUS:
                                    {
                                        opcode = MINUS_INT;
                                        break;
                                    }
                                case TK_MUL:
                                    {
                                        opcode = MULT_INT;
                                        break;
                                    }
                                case TK_DIV:
                                    {
                                        opcode = DIV_INT;
                                        break;
                                    }
                                }
                            }
                            else if (strcmp(t->data->typeName,"real") == 0){
                                strcpy(arg1,"real");
                                switch(head->index){
                                case TK_PLUS:
                                    {
                                        opcode = PLUS_REAL;
                                        break;
                                    }
                                case TK_MINUS:
                                    {
                                        opcode = MINUS_REAL;
                                        break;
                                    }
                                case TK_MUL:
                                    {
                                        opcode = MULT_REAL;
                                        break;
                                    }
                                case TK_DIV:
                                    {
                                        opcode = DIV_REAL;
                                        break;
                                    }
                                }
                            }
                            else {
                                recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                if (r == NULL){
                                    strcpy(arg1,"error");
                                }
                                else {
                                    arg1 = t->data->typeName;
                                    switch(head->index){
                                        case TK_PLUS:
                                            {
                                                opcode = PLUS_RECORD;
                                                break;
                                            }
                                        case TK_MINUS:
                                            {
                                                opcode = MINUS_RECORD;
                                                break;
                                            }
                                        case TK_MUL:
                                            {
                                                opcode = MULT_RECORD;
                                                break;
                                            }
                                        case TK_DIV:
                                            {
                                                opcode = DIV_RECORD;
                                                break;
                                            }
                                        }
                                }
                            }
                        }
                    }
                    else {
                        oper1 = child1;
                        scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                        if (t == NULL){
                            t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                            if (t == NULL){
                                strcpy(arg1,"error");
                            }
                            else {
                                if (strcmp(t->data->typeName,"int") == 0){
                                    strcpy(arg1,"error");
                                }
                                else if (strcmp(t->data->typeName,"real") == 0){
                                    strcpy(arg1,"error");
                                }
                                else {
                                    recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                    if (r == NULL){
                                        strcpy(arg1,"error");
                                    }
                                    else {
                                        scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                        if (rf == NULL){
                                            strcpy(arg1,"error");
                                        }
                                        else {
                                            arg1 = rf->data->typeName;
                                            //arg1 = rf->data->typeName;
                                            if (strcmp(arg1,"int") == 0){
                                                //opcode = ASSIGN_INT;
                                                switch(head->index){
                                                case TK_PLUS:
                                                    {
                                                        opcode = PLUS_INT;
                                                        break;
                                                    }
                                                case TK_MINUS:
                                                    {
                                                        opcode = MINUS_INT;
                                                        break;
                                                    }
                                                case TK_MUL:
                                                    {
                                                        opcode = MULT_INT;
                                                        break;
                                                    }
                                                case TK_DIV:
                                                    {
                                                        opcode = DIV_INT;
                                                        break;
                                                    }
                                                }
                                            }
                                            else if (strcmp(arg1,"real") == 0){
                                                //opcode = ASSIGN_REAL;
                                                switch(head->index){
                                                case TK_PLUS:
                                                    {
                                                        opcode = PLUS_REAL;
                                                        break;
                                                    }
                                                case TK_MINUS:
                                                    {
                                                        opcode = MINUS_REAL;
                                                        break;
                                                    }
                                                case TK_MUL:
                                                    {
                                                        opcode = MULT_REAL;
                                                        break;
                                                    }
                                                case TK_DIV:
                                                    {
                                                        opcode = DIV_REAL;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (strcmp(t->data->typeName,"int") == 0){
                                strcpy(arg1,"error");
                            }
                            else if (strcmp(t->data->typeName,"real") == 0){
                                strcpy(arg1,"error");
                            }
                            else {
                                recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                if (r == NULL){
                                    strcpy(arg1,"error");
                                }
                                else {
                                    scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                    if (rf == NULL){
                                        strcpy(arg1,"error");
                                    }
                                    else {
                                        arg1 = rf->data->typeName;
                                        if (strcmp(arg1,"int") == 0){
                                                //opcode = ASSIGN_INT;
                                                switch(head->index){
                                                case TK_PLUS:
                                                    {
                                                        opcode = PLUS_INT;
                                                        break;
                                                    }
                                                case TK_MINUS:
                                                    {
                                                        opcode = MINUS_INT;
                                                        break;
                                                    }
                                                case TK_MUL:
                                                    {
                                                        opcode = MULT_INT;
                                                        break;
                                                    }
                                                case TK_DIV:
                                                    {
                                                        opcode = DIV_INT;
                                                        break;
                                                    }
                                                }
                                            }
                                            else if (strcmp(arg1,"real") == 0){
                                                //opcode = ASSIGN_REAL;
                                                switch(head->index){
                                                case TK_PLUS:
                                                    {
                                                        opcode = PLUS_REAL;
                                                        break;
                                                    }
                                                case TK_MINUS:
                                                    {
                                                        opcode = MINUS_REAL;
                                                        break;
                                                    }
                                                case TK_MUL:
                                                    {
                                                        opcode = MULT_REAL;
                                                        break;
                                                    }
                                                case TK_DIV:
                                                    {
                                                        opcode = DIV_REAL;
                                                        break;
                                                    }
                                                }
                                            }
                                    }
                                }
                            }
                        }
                    }
                    dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                    break;
                }

            case TK_NUM:
                {
                    strcpy(arg1,"int");
                    int a = child1->valueIfNumber;
                    int *x = (int *)malloc(sizeof(int));
                    *x = a;
                    oper1 = x;
                    //oper1 = (int *)&a;
                    //oper1 = &(child1->valueIfNumber);
                    op1typ = UNION_INT;
                    switch(head->index){
                    case TK_PLUS:
                        {
                            opcode = PLUS_INT;
                            break;
                        }
                    case TK_MINUS:
                        {
                            opcode = MINUS_INT;
                            break;
                        }
                    case TK_MUL:
                        {
                            opcode = MULT_INT;
                            break;
                        }
                    case TK_DIV:
                        {
                            opcode = DIV_INT;
                            break;
                        }
                    }
                    break;
                }
            case TK_RNUM:
                {
                    strcpy(arg1,"real");
                    oper1 = &(child1->valueIfNumber);
                    op1typ = UNION_REAL;
                    switch(head->index){
                    case TK_PLUS:
                        {
                            opcode = PLUS_REAL;
                            break;
                        }
                    case TK_MINUS:
                        {
                            opcode = MINUS_REAL;
                            break;
                        }
                    case TK_MUL:
                        {
                            opcode = MULT_REAL;
                            break;
                        }
                    case TK_DIV:
                        {
                            opcode = DIV_REAL;
                            break;
                        }
                    }
                    break;
                }
            default:
                {

                    dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,&arg1,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                    mylabel = -1;
                    op1 = (*ptrAvailableTemporary )- 1;
                    oper1 = &op1;
                    op1typ = UNION_TEMPORARY;
                    if (strcmp(arg1,"int")==0){
                        switch(head->index){
                        case TK_PLUS:
                            {
                                opcode = PLUS_INT;
                                break;
                            }
                        case TK_MINUS:
                            {
                                opcode = MINUS_INT;
                                break;
                            }
                        case TK_MUL:
                            {
                                opcode = MULT_INT;
                                break;
                            }
                        case TK_DIV:
                            {
                                opcode = DIV_INT;
                                break;
                            }
                        }
                    }
                    else if (strcmp(arg1,"real") == 0){
                        switch(head->index){
                        case TK_PLUS:
                            {
                                opcode = PLUS_REAL;
                                break;
                            }
                        case TK_MINUS:
                            {
                                opcode = MINUS_REAL;
                                break;
                            }
                        case TK_MUL:
                            {
                                opcode = MULT_REAL;
                                break;
                            }
                        case TK_DIV:
                            {
                                opcode = DIV_REAL;
                                break;
                            }
                        }
                    }
                    else {
                        switch(head->index){
                        case TK_PLUS:
                            {
                                opcode = PLUS_RECORD;
                                break;
                            }
                        case TK_MINUS:
                            {
                                opcode = MINUS_RECORD;
                                break;
                            }
                        case TK_MUL:
                            {
                                opcode = MULT_RECORD;
                                break;
                            }
                        case TK_DIV:
                            {
                                opcode = DIV_RECORD;
                                break;
                            }
                        }
                    }
                    break;
                }

            }



            switch (child2->index){
            case ALLVAR:
                {
                    op2typ = UNION_ID;
                    c1 = child2->child;
                    ASTNode *c2 = c1->next;
                    if (c2->index == EPS){
                        oper2 = c1;
                        scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                        if (t == NULL){
                            t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                            if (t == NULL){
                                strcpy(arg2,"error");
                            }
                            else {
                                if (strcmp(t->data->typeName,"int") == 0){
                                    strcpy(arg2,"int");
                                }
                                else if (strcmp(t->data->typeName,"real") == 0){
                                    strcpy(arg2,"real");
                                }
                                else {
                                    recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                    if (r == NULL){
                                        strcpy(arg2,"error");
                                    }
                                    else {
                                        arg2 = t->data->typeName;
                                    }
                                }
                            }
                        }
                        else {
                            if (strcmp(t->data->typeName,"int") == 0){
                                strcpy(arg2,"int");
                            }
                            else if (strcmp(t->data->typeName,"real") == 0){
                                strcpy(arg2,"real");
                            }
                            else {
                                recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                if (r == NULL){
                                    strcpy(arg2,"error");
                                }
                                else {
                                    arg2 = t->data->typeName;
                                }
                            }
                        }
                    }
                    else {
                        oper2 = child2;
                        scopeHashNode *t = searchEntryScopeHashTable(c1->lexemeCurrentNode,currentScope);
                        if (t == NULL){
                            t = searchEntryScopeHashTable(c1->lexemeCurrentNode,globalScope);
                            if (t == NULL){
                                strcpy(arg2,"error");
                            }
                            else {
                                if (strcmp(t->data->typeName,"int") == 0){
                                    strcpy(arg2,"error");
                                }
                                else if (strcmp(t->data->typeName,"real") == 0){
                                    strcpy(arg2,"error");
                                }
                                else {
                                    recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                    if (r == NULL){
                                        strcpy(arg2,"error");
                                    }
                                    else {
                                        scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                        if (rf == NULL){
                                            strcpy(arg2,"error");
                                        }
                                        else {
                                            arg2 = rf->data->typeName;
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (strcmp(t->data->typeName,"int") == 0){
                                strcpy(arg2,"error");
                            }
                            else if (strcmp(t->data->typeName,"real") == 0){
                                strcpy(arg2,"error");
                            }
                            else {
                                recordHashNode *r = searchEntryRecordsHashTable(t->data->typeName,rht);
                                if (r == NULL){
                                    strcpy(arg2,"error");
                                }
                                else {
                                    scopeHashNode *rf = searchEntryScopeHashTable(c2->lexemeCurrentNode,r->data->fields);
                                    if (rf == NULL){
                                        strcpy(arg2,"error");
                                    }
                                    else {
                                        arg2 = rf->data->typeName;
                                    }
                                }
                            }
                        }
                    }
                    dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);

                    break;
                }
            case TK_NUM:
                {
                    strcpy(arg2,"int");
                    op2typ = UNION_INT;
                    int a = child2->valueIfNumber;
                    int *x = (int *)malloc(sizeof(int));
                    *x = a;
                    oper2 = x;
                    //oper2 = (int *)&a;
                    //oper2 = &(child2->valueIfNumber);
                    break;
                }
            case TK_RNUM:
                {
                    strcpy(arg2,"real");
                    op2typ = UNION_REAL;
                    oper2 = &(child2->valueIfNumber);
                    break;
                }
            default:
                {

                    dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,&arg2,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                    mylabel = -1;
                    op2typ = UNION_TEMPORARY;
                    op2 = (*ptrAvailableTemporary) - 1;
                    oper2 = &op2;
                    break;
                }
            }

            resul = *ptrAvailableTemporary;
            (*ptrAvailableTemporary)++;
            resu = &resul;
            restyp = UNION_TEMPORARY;
            int label = -1;
            if (shouldOutputLabel){
                label = mylabel;
            }
            if (head->index == TK_PLUS || head->index == TK_MINUS){
                if (strcmp(arg1,arg2) == 0){
                    if (strcmp(arg1,"error") == 0){
                        strcpy(*argtype,"error");
                    }
                    else {
                        strcpy(*argtype,arg1);
                        addEntryQuadruple(q,opcode,op1typ,op2typ,restyp,oper1,oper2,resu,label);
                    }
                }
                else {
                    strcpy(*argtype,"error");
                }
            }
            else if (head->index == TK_MUL || head->index == TK_DIV){
                if (strcmp(arg1,arg2) == 0){
                    if (strcmp(arg1,"error") == 0){
                        strcpy(*argtype,"error");
                    }
                    else if (strcmp(arg1,"int") == 0) {
                        strcpy(*argtype,arg1);
                        addEntryQuadruple(q,opcode,op1typ,op2typ,restyp,oper1,oper2,resu,label);
                    }
                    else if (strcmp(arg1,"real") == 0){
                        strcpy(*argtype,arg1);
                        addEntryQuadruple(q,opcode,op1typ,op2typ,restyp,oper1,oper2,resu,label);
                    }
                    else {
                        strcpy(*argtype,"error");
                    }
                }
                else {
                    if ( (strcmp(arg1,"int") == 0 && strcmp(arg2,"real") == 0 ) || (strcmp(arg2,"int") == 0 && strcmp(arg1,"real") == 0 ) ){
                        strcpy(*argtype,"error");
                    }
                    else if ((strcmp(arg1,"int") == 0 || strcmp(arg1,"real") == 0 )) {
                        strcpy(*argtype,arg2);
                        addEntryQuadruple(q,opcode,op1typ,op2typ,restyp,oper1,oper2,resu,label);
                    }
                    else if ((strcmp(arg2,"int") == 0 || strcmp(arg2,"real") == 0 )) {
                        strcpy(*argtype,arg1);
                        addEntryQuadruple(q,opcode,op1typ,op2typ,restyp,oper1,oper2,resu,label);
                    }
                    else {
                        strcpy(*argtype,"error");
                    }
                }
            }



            break;
        }

    case TK_LT:
    case TK_GT:
    case TK_GE:
    case TK_EQ:
    case TK_NE:
    case TK_LE:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;
            int arg1type,arg2type,opcode;
            void *a1,*a2;
            if (child1->index == TK_ID){
                if (tailWhile != NULL){
                    scopeHashNode *t = (scopeHashNode *)malloc(sizeof(scopeHashNode));
                    t->data = NULL;
                    t->key = child1->lexemeCurrentNode;
                    insertScopeHashTable(t,tailWhile->wsht);
                }
            }
            if (child2->index == TK_ID){
                if (tailWhile != NULL){
                    scopeHashNode *t = (scopeHashNode *)malloc(sizeof(scopeHashNode));
                    t->data = NULL;
                    t->key = child2->lexemeCurrentNode;
                    insertScopeHashTable(t,tailWhile->wsht);
                }
            }
            char *arg1 = (char *)malloc(sizeof(char) * 32);
            char *arg2 = (char *)malloc(sizeof(char) * 32);
            if (child1->index == TK_NUM){
                strcpy(arg1,"int");
                opcode = 1;
                arg1type = UNION_INT;
                int a = child1->valueIfNumber;
                int *x = (int *)malloc(sizeof(int));
                *x = a;
                a1 = x;
                //a1 = (int *)&a;
                //a1 = &(child1->valueIfNumber);
            }
            else if (child1->index == TK_RNUM){
                strcpy(arg1,"real");
                opcode = 2;
                arg1type = UNION_REAL;
                a1 = &(child1->valueIfNumber);
            }
            else {
                scopeHashNode *t = searchEntryScopeHashTable(child1->lexemeCurrentNode,currentScope);
                if (t == NULL){
                    t = searchEntryScopeHashTable(child1->lexemeCurrentNode,globalScope);
                    if (t == NULL){
                        strcpy(arg1,"error");
                    }
                    else {
                        arg1 = t->data->typeName;
                        arg1type = UNION_ID;
                        a1 = child1;
                        if (strcmp(arg1,"int") == 0){
                            opcode = 1;
                        }
                        else if (strcmp(arg1,"real") == 0){
                            opcode = 2;
                        }
                        else {
                            opcode = 3;
                        }
                    }
                }
                else {
                    arg1 = t->data->typeName;
                    arg1type = UNION_ID;
                    a1 = child1;
                    if (strcmp(arg1,"int") == 0){
                        opcode = 1;
                    }
                    else if (strcmp(arg1,"real") == 0){
                        opcode = 2;
                    }
                    else {
                        opcode = 3;
                    }
                }
                dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            }
            if (child2->index == TK_NUM){
                strcpy(arg2,"int");
                arg2type = UNION_INT;
                int a = child2->valueIfNumber;
                int *x = (int *)malloc(sizeof(int));
                *x = a;
                a2 = x;
                //a2 = (int *)&a;
                //a2 = &(child2->valueIfNumber);
            }
            else if (child2->index == TK_RNUM){
                strcpy(arg2,"real");
                arg2type = UNION_REAL;
                a2 = &(child2->valueIfNumber);
            }
            else {
                scopeHashNode *t = searchEntryScopeHashTable(child2->lexemeCurrentNode,currentScope);
                if (t == NULL){
                    t = searchEntryScopeHashTable(child2->lexemeCurrentNode,globalScope);
                    if (t == NULL){
                        strcpy(arg2,"error");
                    }
                    else {
                        arg2 = t->data->typeName;
                        arg2type = UNION_ID;
                        a2 = child2;
                    }
                }
                else {
                    arg2 = t->data->typeName;
                    arg2type = UNION_ID;
                    a2 = child2;
                }
                dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            }
            if (strcmp(arg1,arg2) == 0){
                if (strcmp(arg1,"error") == 0){
                    strcpy(*argtype,"error");
                    //*success = 0; printf("Error 8:An if statement must have the expression of boolean type\n");
                }
                else {
                    strcpy(*argtype,"_boolean");
                    int label = -1;
                    if (shouldOutputLabel){
                        label = mylabel;
                    }
                    addQuad(q,opcode,arg1type,arg2type,a1,a2,label,jumpIfTrue,jumpIfFalse,head->index,da);
                }
            }
            else {
                strcpy(*argtype,"error");
                //*success = 0; printf("Error 8:An if statement must have the expression of boolean type\n");
            }
            break;
        }
    case TK_AND:
    case TK_OR:
        {
            ASTNode *child1 = head->child;
            ASTNode *child2 = child1->next;

            int b1t,b2t,b1f,b2f,lbl;
            lbl = *ptrAvailableLabels;
            (*ptrAvailableLabels)++;
            if (head->index == TK_OR){
                if (jumpIfTrue == -1){
                    b1t = nextlabel;
                }
                else {
                    b1t = jumpIfTrue;
                }
                b1f = -1;
                b2t = jumpIfTrue;
                b2f = jumpIfFalse;
            }
            else if (head->index == TK_AND){
                if (jumpIfFalse == -1){
                    b1f = nextlabel;
                }
                else {
                    b1f = jumpIfFalse;
                }
                b1t = -1;
                b2t = jumpIfTrue;
                b2f = jumpIfFalse;
            }

            char *arg1 = (char *)malloc(sizeof(char) * 32);
            char *arg2 = (char *)malloc(sizeof(char) * 32);
            dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,&arg1,headWhile,tailWhile, mylabel, lbl, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, b1t, b1f, shouldOutputLabel, success, q, da, maxoffset);
            dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,&arg2,headWhile,tailWhile, lbl, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, b2t, b2f, shouldOutputLabel, success, q, da, maxoffset);
            if (strcmp(arg1,arg2) == 0){
                if (strcmp(arg1,"_boolean") == 0){
                    strcpy(*argtype,"_boolean");
                }
                else {
                    strcpy(*argtype,"error");
                }
            }
            else {
                strcpy(*argtype,"error");
                dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset); //previously uncommented. should be commented
            }
            break;
        }
    case TK_NOT:
        {
            ASTNode *child1 = head->child;
            dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfFalse, jumpIfTrue, shouldOutputLabel, success, q, da, maxoffset); //new addition
            break;
        }

    case ITERATIVESTMT:
    case CONDITIONALSTMT:
        {
            //whileScopeList *currWhile = NULL;
            if (head->index==ITERATIVESTMT){
                if (tailWhile == NULL){
                    headWhile = (whileScopeList *)malloc(sizeof(whileScopeList));
                    headWhile->wsht = createScopeHashTable(5,7);
                    headWhile->isChanged = 0;
                    headWhile->next = NULL;
                    tailWhile = headWhile;
                    //currWhile = headWhile;
                }
                else {
                    tailWhile->next = (whileScopeList *)malloc(sizeof(whileScopeList));
                    tailWhile->next->wsht = createScopeHashTable(5,7);
                    tailWhile->next->isChanged = 0;
                    tailWhile->next->next = NULL;
                    //currWhile = tailWhile->next;
                    tailWhile = tailWhile->next;
                }
            }

            if (head->index == CONDITIONALSTMT){
                ASTNode *child1 = head->child;
                ASTNode *child2 = child1->next;
                ASTNode *child3 = child2->next;
                ASTNode *child4 = child3->next;
                int mylbl1, mylbl2,mylbl3,mylbl4,nxtlbl1,nxtlbl2,grndnxtlbl2,jit,jif;
                mylbl1 = mylabel;
                mylbl2 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                nxtlbl1 = mylbl2;
                if (child3->index != EPS){
                    mylbl3 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                    nxtlbl2 = mylbl3;
                    grndnxtlbl2 = mylbl3;//new add
                }
                else {
                    nxtlbl2 = grandnextlabel;
                    grndnxtlbl2 = grandnextlabel; // new add
                }
                if (child4->index != EPS){
                    mylbl4 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                }
                jit = -1;
                jif = (child4->index != EPS) ? mylbl4 : nextlabel;

                if (child1->index == TK_NOT){ //traversing boolean condition
                    char *arg = (char *)malloc(sizeof(char) * 32);
                    dfsForSemanticAnalysis(child1->child,rht,afht,currentScope,globalScope,fname,&arg,headWhile,tailWhile, mylbl1, nxtlbl1, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jit, jif, shouldOutputLabel, success, q, da, maxoffset);
                    if (strcmp(arg,"_boolean") != 0){
                        *success = 0; printf("Error 8:An if statement must have the expression of boolean type\n");
                    }
                }
                else {
                    char *arg = (char *)malloc(sizeof(char) * 32);
                    dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,&arg,headWhile,tailWhile, mylbl1, nxtlbl1, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jit, jif, shouldOutputLabel, success, q, da, maxoffset);
                    if (strcmp(arg,"_boolean") != 0){
                        *success = 0; printf("Error 8:An if statement must have the expression of boolean type\n");
                    }
                }
                dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylbl2, nxtlbl2, grndnxtlbl2, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                int shldoutlbl = 1;
                if (child2->index == CONDITIONALSTMT || child2->index == ITERATIVESTMT){
                    shldoutlbl = 1;
                }
                dfsForSemanticAnalysis(child3,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylbl3, -1, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shldoutlbl, success, q, da, maxoffset);
                if (child4->index != EPS){
                    addEntryQuadruple(q,JUMP,UNION_LABEL,-1,-1,&nextlabel,NULL,NULL,-1);
                    addEntryDynamicArray(nextlabel,da);
                }
                dfsForSemanticAnalysis(child4,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylbl4, -1, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);

            }
            else {
                ASTNode *child1 = head->child;
                ASTNode *child2 = child1->next;
                ASTNode *child3 = child2->next;
                int beglabel, mylbl1, mylbl2,mylbl3,mylbl4,nxtlbl1,nxtlbl2,grndnxt2,grndnxt3,jit,jif;
                beglabel = mylabel;
                mylbl1 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                mylbl2 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                nxtlbl1 = mylbl2;
                if (child3->index != EPS){
                    mylbl3 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                    mylbl4 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                    nxtlbl2 = mylbl3;
                    grndnxt2 = mylbl3;
                    grndnxt3 = mylbl4;
                }
                else {
                    mylbl4 = *ptrAvailableLabels; (*ptrAvailableLabels)++;
                    nxtlbl2 = mylbl4;
                    grndnxt2 = mylbl4;
                }
                jit = -1;
                jif = nextlabel;
                addEntryQuadruple(q,LABEL,-1,-1,-1,NULL,NULL,NULL,beglabel);
                if (child1->index == TK_NOT){ //traversing boolean condition
                    char *arg = (char *)malloc(sizeof(char) * 32);
                    dfsForSemanticAnalysis(child1->child,rht,afht,currentScope,globalScope,fname,&arg,headWhile,tailWhile, mylbl1, nxtlbl1, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jit, jif, shouldOutputLabel, success, q, da, maxoffset);
                    if (strcmp(arg,"_boolean") != 0){
                        *success = 0; printf("Error 8:A while statement must have the expression of boolean type\n");
                    }
                }
                else {
                    char *arg = (char *)malloc(sizeof(char) * 32);
                    dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,&arg,headWhile,tailWhile, mylbl1, nxtlbl1, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jit, jif, shouldOutputLabel, success, q, da, maxoffset);
                    if (strcmp(arg,"_boolean") != 0){
                        *success = 0; printf("Error 8:A while statement must have the expression of boolean type\n");
                    }
                }
                dfsForSemanticAnalysis(child2,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylbl2, nxtlbl2, grndnxt2, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                int shldoutlbl = 1;
                if (child2->index == CONDITIONALSTMT || child2->index == ITERATIVESTMT){
                    shldoutlbl = 1;
                }
                dfsForSemanticAnalysis(child3,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylbl3, -1, grndnxt3, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shldoutlbl, success, q, da, maxoffset);
                addEntryQuadruple(q,JUMP,UNION_LABEL,-1,-1,&beglabel,NULL,NULL,mylbl4);
                addEntryDynamicArray(beglabel,da);
                if (tailWhile->isChanged == 0){
                    *success = 0; printf("Error 15:A  while statement  must redefine the variable that participates in the iterations\n");
                }
            }

//            child1 = child1->next;
//            while (child1!= NULL){ //traversing other part
//                dfsForSemanticAnalysis(child1,rht,afht,currentScope,globalScope,fname,argtype,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
//                child1 = child1->next;
//            }

            break;

        }

    case TK_READ:
        {
            ASTNode *singrec = head->next;
            ASTNode *arg = (singrec->child->next->index == EPS) ? singrec->child :  singrec;
            //scopeHashNode *shn = (singrec->child->next->index == EPS) ?
            int argtype,opcode;
            if (arg == singrec){ //reading in one of the fields of record
                scopeHashNode *shn = searchEntryScopeHashTable(singrec->child->lexemeCurrentNode,currentScope);
                if (shn == NULL){
                    shn = searchEntryScopeHashTable(singrec->child->lexemeCurrentNode,globalScope);
                }
                if (shn != NULL){
                    recordHashNode *rhn = searchEntryRecordsHashTable(shn->data->typeName,rht);
                    if (rhn != NULL){
                        scopeHashNode *shn = searchEntryScopeHashTable(arg->child->next->lexemeCurrentNode,rhn->data->fields);
                        if (shn != NULL){
                            if (strcmp(shn->data->typeName,"int") == 0){
                                opcode = READ_INT;
                            }
                            else if (strcmp(shn->data->typeName,"real") == 0){
                                opcode = READ_REAL;
                            }
                        }
                    }
                }
            }
            else {
                scopeHashNode *shn = searchEntryScopeHashTable(singrec->child->lexemeCurrentNode,currentScope);
                if (shn == NULL){
                    shn = searchEntryScopeHashTable(singrec->child->lexemeCurrentNode,globalScope);
                }
                if (shn != NULL){
                    if (strcmp(shn->data->typeName,"int") == 0){
                        opcode = READ_INT;
                    }
                    else if (strcmp(shn->data->typeName,"real") == 0){
                        opcode = READ_REAL;
                    }
                }
            }
            addEntryQuadruple(q,opcode,UNION_ID,-1,-1,arg,NULL,NULL,mylabel);
            dfsForSemanticAnalysis(arg,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            break;
        }
    case TK_WRITE:
        {
            ASTNode *nxt = head->next;
            int argtype,opcode;
            int arg_int;
            float arg_real;
            ASTNode *arg_id;
            switch(nxt->index){
            case TK_NUM:
                {
                    opcode = WRITE_INT;
                    arg_int = nxt->valueIfNumber;
                    argtype = UNION_INT;
                    addEntryQuadruple(q,opcode,argtype,-1,-1,&arg_int,NULL,NULL,mylabel);
                    break;
                }
            case TK_RNUM:
                {
                    opcode = WRITE_REAL;
                    arg_real = nxt->valueIfNumber;
                    argtype = UNION_REAL;
                    addEntryQuadruple(q,opcode,argtype,-1,-1,&arg_real,NULL,NULL,mylabel);
                    break;
                }
            default:
                {
                    argtype = UNION_ID;
                    if (nxt->child->next->index != EPS){
                        scopeHashNode *shn = searchEntryScopeHashTable(nxt->child->lexemeCurrentNode,currentScope);
                        if (shn == NULL){
                            shn = searchEntryScopeHashTable(nxt->child->lexemeCurrentNode,globalScope);
                        }
                        if (shn != NULL){
                            recordHashNode *rhn = searchEntryRecordsHashTable(shn->data->typeName,rht);
                            if (rhn != NULL){
                                scopeHashNode *shn = searchEntryScopeHashTable(nxt->child->next->lexemeCurrentNode,rhn->data->fields);
                                if (shn != NULL){
                                    if (strcmp(shn->data->typeName,"int") == 0){
                                        opcode = WRITE_INT;
                                    }
                                    else if (strcmp(shn->data->typeName,"real") == 0){
                                        opcode = WRITE_REAL;
                                    }
                                }
                            }
                        }
                        arg_id = nxt;
                    }
                    else {
                        scopeHashNode *shn = searchEntryScopeHashTable(nxt->child->lexemeCurrentNode,currentScope);
                        if (shn == NULL){
                            shn = searchEntryScopeHashTable(nxt->child->lexemeCurrentNode,globalScope);
                        }
                        if (shn != NULL){
                            if (strcmp(shn->data->typeName,"int") == 0){
                                opcode = WRITE_INT;
                                arg_id = nxt->child;
                            }
                            else if (strcmp(shn->data->typeName,"real") == 0){
                                opcode = WRITE_REAL;
                                arg_id = nxt->child;
                            }
                            else {
                                opcode = WRITE_RECORD;
                                arg_id = nxt->child;
                            }
                        }
                    }
                    addEntryQuadruple(q,opcode,argtype,-1,-1,arg_id,NULL,NULL,mylabel);
                    break;
                }
            }
            dfsForSemanticAnalysis(nxt,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
            break;
        }

    default:
        {
            ASTNode *curr = head->child;
            while (curr!=NULL){
                dfsForSemanticAnalysis(curr,rht,afht,currentScope,globalScope,fname,NULL,headWhile,tailWhile, mylabel, nextlabel, grandnextlabel, ptrAvailableLabels, ptrAvailableTemporary, jumpIfTrue, jumpIfFalse, shouldOutputLabel, success, q, da, maxoffset);
                curr = curr->next;
            }
            break;
        }
    }
}

void makeCode(quadrupleNode *qn, scopeHashTable *currentScope, scopeHashTable *globalScope, recordsHashTable *rht, dynamicArray *usedLabels, reg **registers, int total_registers, int *nextlabel, FILE *fp){
        
        switch(qn->opcode){
        case LABEL:
            {
                fprintf(fp,"l%d:\n",qn->label);
                break;
            }
        case JUMP:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                fprintf(fp,"\tjmp l%d\n", qn->arg1->data->jump_label);
                break;
            }
        case JLT_INT:
        case JLE_INT:
        case JGT_INT:
        case JGE_INT:
        case JEQ_INT:
        case JNE_INT:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_INT && qn->arg2->type == UNION_INT){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                }
                else if (qn->arg1->type != UNION_INT){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;

                    /*
                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = getOffset(qn->arg2);
                    */
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    if (qn->arg2->type != UNION_INT){
                        int offset2 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                        int reg2 = searchRegister(registers,total_registers,offset2);
                        if (reg2 == -1){
                            fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName ,offset2);
                        }
                        else {
                            fprintf(fp,"\tcmp %s, %s\n", registers[reg1]->regName ,registers[reg2]->regName);
                        }

                    }
                    else {
                        fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName ,qn->arg2->data->int_val);
                    }
                    switch(qn->opcode){
                    case JLT_INT:
                        {
                            fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JLE_INT:
                        {
                            fprintf(fp,"\tjle l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JGT_INT:
                        {
                            fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JGE_INT:
                        {
                            fprintf(fp,"\tjge l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JEQ_INT:
                        {
                            fprintf(fp,"\tje l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JNE_INT:
                        {
                            fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    }
                }
                else {
                    int offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 == -1) ? 1 : 0;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName ,qn->arg1->data->int_val);
                    switch(qn->opcode){
                    case JLT_INT:
                        {
                            fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JLE_INT:
                        {
                            fprintf(fp,"\tjge l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JGT_INT:
                        {
                            fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JGE_INT:
                        {
                            fprintf(fp,"\tjle l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JEQ_INT:
                        {
                            fprintf(fp,"\tje l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    case JNE_INT:
                        {
                            fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                            break;
                        }
                    }
                }
                break;
            }
        case PLUS_INT:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_INT && qn->arg2->type == UNION_INT){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName ,qn->arg1->data->int_val);
                    fprintf(fp,"\tadd %s, %d\n", registers[reg1]->regName, qn->arg2->data->int_val);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2;

                }
                else if (qn->arg1->type != UNION_INT && qn->arg2->type == UNION_INT){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\tadd %s, %d\n", registers[reg1]->regName, qn->arg2->data->int_val);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                else if (qn->arg1->type == UNION_INT && qn->arg2->type != UNION_INT){
                    int offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\tadd %s, %d\n", registers[reg1]->regName, qn->arg1->data->int_val);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\tadd %s, dword [programdata + %d]\n", registers[reg1]->regName, getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL) );
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                break;
            }
        case MINUS_INT:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_INT && qn->arg2->type == UNION_INT){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName ,qn->arg1->data->int_val);
                    fprintf(fp,"\tsub %s, %d\n", registers[reg1]->regName, qn->arg2->data->int_val);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2;

                }
                else if (qn->arg1->type != UNION_INT && qn->arg2->type == UNION_INT){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\tsub %s, %d\n", registers[reg1]->regName, qn->arg2->data->int_val);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                else if (qn->arg1->type == UNION_INT && qn->arg2->type != UNION_INT){
                    
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, qn->arg1->data->int_val);
                    fprintf(fp,"\tsub %s, dword [programdata + %d]\n", registers[reg1]->regName, getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL));
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2;
                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\tsub %s, dword [programdata + %d]\n", registers[reg1]->regName, getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL) );
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                break;
            }
        case MULT_INT:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_INT && qn->arg2->type == UNION_INT){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName ,qn->arg1->data->int_val);
                    fprintf(fp,"\timul %s, %s, %d\n", registers[reg1]->regName, registers[reg1]->regName, qn->arg2->data->int_val);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2;
                }
                else if (qn->arg1->type != UNION_INT && qn->arg2->type == UNION_INT){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    if (flag == 1){
                        fprintf(fp,"\timul %s, dword [programdata + %d], %d\n", registers[reg1]->regName , offset1, qn->arg2->data->int_val);
                    }
                    else {
                        fprintf(fp,"\timul %s, %s, %d\n", registers[reg1]->regName , registers[reg1]->regName, qn->arg2->data->int_val);
                    }
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                else if (qn->arg1->type == UNION_INT && qn->arg2->type != UNION_INT){
                    int offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    if (flag == 1){
                        fprintf(fp,"\timul %s, dword [programdata + %d], %d\n", registers[reg1]->regName , offset1, qn->arg1->data->int_val);
                    }
                    else {
                        fprintf(fp,"\timul %s, %s, %d\n", registers[reg1]->regName , registers[reg1]->regName, qn->arg1->data->int_val);
                    }
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2;
                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    fprintf(fp,"\timul %s, dword [programdata + %d]\n", registers[reg1]->regName,  getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL));
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                break;
            }
        case DIV_INT:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_INT && qn->arg2->type == UNION_INT){
                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, %d\n", qn->arg1->data->int_val);
                    registers[EAX]->offsetIfPresent = 1;
                    registers[EDX]->offsetIfPresent = 1;
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[EAX]->offsetIfPresent = -1;
                    registers[EDX]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    fprintf(fp,"\tidiv %s\n", registers[reg1]->regName);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], EAX\n", offset2 );
                    registers[EAX]->offsetIfPresent = offset2;
                }
                else if (qn->arg1->type != UNION_INT && qn->arg2->type == UNION_INT){
                    fprintf(fp,"\tmov EDX, 0\n");
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    if (reg1 == -1){
                        fprintf(fp,"\tmov EAX, dword [programdata + %d]\n", offset1);
                        registers[EAX]->offsetIfPresent = offset1;
                    }
                    else {
                        fprintf(fp,"\tmov EAX, %s\n", registers[reg1]->regName);
                    }
                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    fprintf(fp,"\tidiv %s\n", registers[reg2]->regName);
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], EAX\n", offset2);
                    registers[EAX]->offsetIfPresent = offset2;
                }
                else if (qn->arg1->type == UNION_INT && qn->arg2->type != UNION_INT){
                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, %d\n", qn->arg1->data->int_val);
                    int offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    if (reg1 == -1){
                        fprintf(fp,"\tidiv   dword [programdata + %d]\n", offset1);
                    }
                    else {
                        fprintf(fp,"\tidiv %s\n", registers[reg1]->regName);
                    }
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], EAX\n", offset2);
                    registers[EAX]->offsetIfPresent = offset2;
                }
                else {
                    fprintf(fp,"\tmov EDX, 0\n");
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    if (reg1 == -1){
                        fprintf(fp,"\tmov EAX, dword [programdata + %d]\n", offset1);
                        registers[EAX]->offsetIfPresent = offset1;
                    }
                    else {
                        fprintf(fp,"\tmov EAX, %s\n", registers[reg1]->regName);
                    }
                    fprintf(fp,"\tidiv   dword [programdata + %d]\n", getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL));
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], EAX\n", offset2 );
                    registers[EAX]->offsetIfPresent = offset2; // adding changes the value of the register
                }
                break;
            }
        case ASSIGN_INT:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels)){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_INT){
                    fprintf(fp,"\tmov dword [programdata + %d], %d\n", getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL), qn->arg1->data->int_val);
                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset2;
                }
                break;
            }
        case JEQ_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;

                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjne l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tje l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3);
                    fprintf(fp,"\tjne l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4);
                    fprintf(fp,"\tje l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else {
                    float data2;
                    if (qn->arg1->type != UNION_REAL){
                        data2 = qn->arg2->data->real_val;
                    }
                    else {
                        data2 = qn->arg1->data->real_val;
                    }
                    int intpart2, fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    int offset1;
                    if (qn->arg1->type != UNION_REAL){
                        offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    else {
                        offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjne l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tje l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                break;
            }
        case JNE_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;

                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3);
                    fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4);
                    fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                }
                else {
                    float data2;
                    if (qn->arg1->type != UNION_REAL){
                        data2 = qn->arg2->data->real_val;
                    }
                    else {
                        data2 = qn->arg1->data->real_val;
                    }
                    int intpart2, fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    int offset1;
                    if (qn->arg1->type != UNION_REAL){
                        offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    else {
                        offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                }
                break;
            }
        case JLT_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;

                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjg l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjg l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else {
                    float data2;
                    if (qn->arg1->type != UNION_REAL){
                        data2 = qn->arg2->data->real_val;
                    }
                    else {
                        data2 = qn->arg1->data->real_val;
                    }
                    int intpart2, fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    int offset1;
                    if (qn->arg1->type != UNION_REAL){
                        offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    else {
                        offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjg l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                break;
            }
        case JLE_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;

                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjg l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjle l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL)  ;
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjg l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3);
                    fprintf(fp,"\tjle l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else {
                    float data2;
                    if (qn->arg1->type != UNION_REAL){
                        data2 = qn->arg2->data->real_val;
                    }
                    else {
                        data2 = qn->arg1->data->real_val;
                    }
                    int intpart2, fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    int offset1;
                    if (qn->arg1->type != UNION_REAL){
                        offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    else {
                        offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjl l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjg l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjle l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                break;
            }
        case JGT_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;

                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjl l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL)   ;
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjl l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else {
                    float data2;
                    if (qn->arg1->type != UNION_REAL){
                        data2 = qn->arg2->data->real_val;
                    }
                    else {
                        data2 = qn->arg1->data->real_val;
                    }
                    int intpart2, fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    int offset1;
                    if (qn->arg1->type != UNION_REAL){
                        offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    else {
                        offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjl l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                break;
            }
        case JGE_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;

                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = -1;
                    registers[reg1]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjl l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjge l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL)  ;
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjl l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4);
                    fprintf(fp,"\tjge l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                else {
                    float data2;
                    if (qn->arg1->type != UNION_REAL){
                        data2 = qn->arg2->data->real_val;
                    }
                    else {
                        data2 = qn->arg1->data->real_val;
                    }
                    int intpart2, fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;
                    int offset1;
                    if (qn->arg1->type != UNION_REAL){
                        offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    else {
                        offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    }
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tjg l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"\tjl l%d\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tjge l%d\n", qn->result->data->jump_label);
                    fprintf(fp,"l%d:\n",*nextlabel);
                    *nextlabel++;
                }
                break;
            }
        case ASSIGN_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels)){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL){
                    float data1 = qn->arg1->data->real_val;
                    int intpart1,fracpart1;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    int offset1 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %d\n", offset1, intpart1);
                    fprintf(fp,"\tmov dword [programdata + %d], %d\n", offset1 + 4, fracpart1);
                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }
                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }
                    int offset3 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3, registers[reg1]->regName );
                    registers[reg1]->offsetIfPresent = offset3;
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3 + 4, registers[reg2]->regName );
                    registers[reg2]->offsetIfPresent = offset3 + 4;
                }
                break;
            }
        case PLUS_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;
                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    registers[reg2]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;

                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName ,intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName ,fracpart1);
                    fprintf(fp,"\tadd %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tadd %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset2;
                    registers[reg2]->offsetIfPresent = offset2 + 4;

                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type == UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }

                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }

                    float data2 = qn->arg2->data->real_val;
                    int intpart2,fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;

                    fprintf(fp,"\tadd %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tadd %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    int offset3 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset3;
                    registers[reg2]->offsetIfPresent = offset3 + 4;
                }
                else if (qn->arg1->type == UNION_REAL && qn->arg2->type != UNION_REAL){
                    int offset1 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }

                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }

                    float data1 = qn->arg1->data->real_val;
                    int intpart1,fracpart1;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;

                    fprintf(fp,"\tadd %s, %d\n", registers[reg1]->regName, intpart1);
                    fprintf(fp,"\tadd %s, %d\n", registers[reg2]->regName, fracpart1);
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    int offset3 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset3;
                    registers[reg2]->offsetIfPresent = offset3 + 4;

                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }

                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }

                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tadd %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3 );
                    fprintf(fp,"\tadd %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4 );
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    int offset4 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset4, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset4 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset4;
                    registers[reg2]->offsetIfPresent = offset4 + 4;
                }
                break;
            }
        case MINUS_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;
                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    registers[reg2]->offsetIfPresent = -1;
                    float data1 = qn->arg1->data->real_val;
                    float data2 = qn->arg2->data->real_val;
                    int intpart1,intpart2,fracpart1,fracpart2;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;

                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName ,intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName ,fracpart1);
                    fprintf(fp,"\tsub %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tsub %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset2 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset2;
                    registers[reg2]->offsetIfPresent = offset2 + 4;

                }
                else if (qn->arg1->type != UNION_REAL && qn->arg2->type == UNION_REAL){
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag1 = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag1 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }

                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }

                    float data2 = qn->arg2->data->real_val;
                    int intpart2,fracpart2;
                    intpart2 = (int)data2;
                    fracpart2 = ((int)(data2*100))%100;

                    fprintf(fp,"\tsub %s, %d\n", registers[reg1]->regName, intpart2);
                    fprintf(fp,"\tsub %s, %d\n", registers[reg2]->regName, fracpart2);
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    int offset3 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset3 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset3;
                    registers[reg2]->offsetIfPresent = offset3 + 4;
                }
                else if (qn->arg1->type == UNION_REAL && qn->arg2->type != UNION_REAL){
                    int reg1 = getEmptyRegister(registers);
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = 1;
                    int reg2 = getEmptyRegister(registers);
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg1]->offsetIfPresent = -1;
                    registers[reg2]->offsetIfPresent = -1;

                    float data1 = qn->arg1->data->real_val;
                    int intpart1,fracpart1;
                    intpart1 = (int)data1;
                    fracpart1 = ((int)(data1*100))%100;

                    fprintf(fp,"\tmov %s, %d\n", registers[reg1]->regName ,intpart1);
                    fprintf(fp,"\tmov %s, %d\n", registers[reg2]->regName ,fracpart1);

                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tsub %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3 );
                    fprintf(fp,"\tsub %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4 );
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    int offset4 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset4, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset4 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset4;
                    registers[reg2]->offsetIfPresent = offset4 + 4;
                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    int reg1 = searchRegister(registers,total_registers,offset1);
                    int flag = (reg1 != -1) ? 0 : 1;
                    if (reg1 == -1){
                        reg1 = getEmptyRegister(registers);
                    }
                    if (reg1 == -1){
                        reg1 = EDI;
                    }
                    registers[reg1]->offsetIfPresent = offset1;
                    if (flag == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg1]->regName ,registers[reg1]->offsetIfPresent);
                    }

                    int offset2 = offset1 + 4;
                    int reg2 = searchRegister(registers,total_registers,offset2);
                    int flag2 = (reg2 != -1) ? 0 : 1;
                    if (reg2 == -1){
                        reg2 = getEmptyRegister(registers);
                    }
                    if (reg2 == -1){
                        reg2 = (reg1 == EDI) ? ESI : EDI;
                    }
                    registers[reg2]->offsetIfPresent = offset2;
                    if (flag2 == 1){
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", registers[reg2]->regName ,registers[reg2]->offsetIfPresent);
                    }

                    int offset3 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tsub %s, dword [programdata + %d]\n", registers[reg1]->regName, offset3 );
                    fprintf(fp,"\tsub %s, dword [programdata + %d]\n", registers[reg2]->regName, offset3 + 4 );
                    fprintf(fp,"\tcmp %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tsub %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tinc %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp %s, -100\n", registers[reg2]->regName);
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd %s, 100\n", registers[reg2]->regName);
                    fprintf(fp,"\tdec %s\n", registers[reg1]->regName);
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    int offset4 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset4, registers[reg1]->regName );
                    fprintf(fp,"\tmov dword [programdata + %d], %s\n", offset4 + 4, registers[reg2]->regName );
                    registers[reg1]->offsetIfPresent = offset4;
                    registers[reg2]->offsetIfPresent = offset4 + 4;
                }
                break;
            }
        case MULT_REAL:
            {
                if (searchEntryDynamicArray(qn->label,usedLabels) == 1){
                    fprintf(fp,"l%d:\n",qn->label);
                }
                if (qn->arg1->type == UNION_REAL || qn->arg2->type == UNION_REAL){
                    float data2;
                    int intpart2, fracpart2;
                    if (qn->arg1->type == UNION_REAL && qn->arg2->type == UNION_REAL){
                        registers[ECX]->offsetIfPresent = -1;
                        registers[EBX]->offsetIfPresent = -1;
                        float data1 = qn->arg1->data->real_val;
                        int intpart1 = (int)data1;
                        int fracpart1 = ((int)(data1*100))%100;
                        fprintf(fp,"\tmov ECX, %d\n", intpart1);
                        fprintf(fp,"\tmov EBX, %d\n", fracpart1);
                        data2 = qn->arg2->data->real_val;
                        intpart2 = (int)data2;
                        fracpart2 = ((int)(data2*100))%100;
                    }
                    else if (qn->arg1->type != UNION_REAL){
                        int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                        registers[ECX]->offsetIfPresent = offset1;
                        registers[EBX]->offsetIfPresent = offset1 + 4;
                        fprintf(fp,"\tmov ECX, dword [programdata + %d]\n", offset1);
                        fprintf(fp,"\tmov EBX, dword [programdata + %d]\n", offset1 + 4);
                        data2 = qn->arg2->data->real_val;
                        intpart2 = (int)data2;
                        fracpart2 = ((int)(data2*100))%100;
                    }
                    else {
                        int offset2 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                        registers[ECX]->offsetIfPresent = offset2;
                        registers[EBX]->offsetIfPresent = offset2 + 4;
                        fprintf(fp,"\tmov ECX, dword [programdata + %d]\n", offset2);
                        fprintf(fp,"\tmov EBX, dword [programdata + %d]\n", offset2 + 4);
                        data2 = qn->arg1->data->real_val;
                        intpart2 = (int)data2;
                        fracpart2 = ((int)(data2*100))%100;
                    }



                    fprintf(fp,"\tmov ESI, 0\n");
                    fprintf(fp,"\tmov EDI, 0\n");

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, ECX\n");
                    fprintf(fp,"\timul EAX, EAX, %d\n",intpart2);
                    fprintf(fp,"\tadd ESI, EAX\n");

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, ECX\n");
                    fprintf(fp,"\timul EAX, EAX, %d\n", fracpart2);
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tadd ESI, EAX\n");
                    fprintf(fp,"\tadd EDI, EDX\n");
                    fprintf(fp,"\tcmp EDI, 100\n");
                    fprintf(fp,"\tjl %d\n",*nextlabel);
                    fprintf(fp,"\tsub EDI, 100\n");
                    fprintf(fp,"\tinc ESI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDI, -100\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd EDI, 100\n");
                    fprintf(fp,"\tdec ESI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, EBX\n");
                    fprintf(fp,"\timul EAX, EAX, %d\n",intpart2);
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tadd ESI, EAX\n");
                    fprintf(fp,"\tadd EDI, EDX\n");
                    fprintf(fp,"\tcmp EDI, 100\n");
                    fprintf(fp,"\tjl %d\n",*nextlabel);
                    fprintf(fp,"\tsub EDI, 100\n");
                    fprintf(fp,"\tinc ESI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDI, -100\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd EDI, 100\n");
                    fprintf(fp,"\tdec ESI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, EBX\n");
                    fprintf(fp,"\timul EAX, EAX, %d\n", fracpart2);
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tcmp EDX, 50\n");
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tinc EDI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDX, -50\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tdec EDI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tadd ESI, EAX\n");
                    fprintf(fp,"\tadd EDI, EDX\n");
                    fprintf(fp,"\tcmp EDI, 100\n");
                    fprintf(fp,"\tjl %d\n",*nextlabel);
                    fprintf(fp,"\tsub EDI, 100\n");
                    fprintf(fp,"\tinc ESI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDI, -100\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd EDI, 100\n");
                    fprintf(fp,"\tdec ESI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;
                    
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], ESI\n", offset2 );
                    fprintf(fp,"\tmov dword [programdata + %d], EDI\n", offset2 + 4 );
                    registers[ESI]->offsetIfPresent = offset2;
                    registers[EDI]->offsetIfPresent = offset2 + 4;

                }
                else {
                    int offset1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL);
                    registers[ECX]->offsetIfPresent = offset1;
                    registers[EBX]->offsetIfPresent = offset1 + 4;
                    fprintf(fp,"\tmov ECX, dword [programdata + %d]\n", offset1);
                    fprintf(fp,"\tmov EBX, dword [programdata + %d]\n", offset1 + 4);

                    int intpart2 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,NULL);
                    int fracpart2 = intpart2 + 4;

                    fprintf(fp,"\tmov ESI, 0\n");
                    fprintf(fp,"\tmov EDI, 0\n");

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, ECX\n");
                    fprintf(fp,"\timul EAX, dword [programdata + %d]\n",intpart2);
                    fprintf(fp,"\tadd ESI, EAX\n");

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, ECX\n");
                    fprintf(fp,"\timul EAX, dword [programdata + %d]\n", fracpart2);
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tadd ESI, EAX\n");
                    fprintf(fp,"\tadd EDI, EDX\n");
                    fprintf(fp,"\tcmp EDI, 100\n");
                    fprintf(fp,"\tjl %d\n",*nextlabel);
                    fprintf(fp,"\tsub EDI, 100\n");
                    fprintf(fp,"\tinc ESI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDI, -100\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd EDI, 100\n");
                    fprintf(fp,"\tdec ESI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, EBX\n");
                    fprintf(fp,"\timul EAX, dword [programdata + %d]\n",intpart2);
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tadd ESI, EAX\n");
                    fprintf(fp,"\tadd EDI, EDX\n");
                    fprintf(fp,"\tcmp EDI, 100\n");
                    fprintf(fp,"\tjl %d\n",*nextlabel);
                    fprintf(fp,"\tsub EDI, 100\n");
                    fprintf(fp,"\tinc ESI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDI, -100\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd EDI, 100\n");
                    fprintf(fp,"\tdec ESI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tmov EAX, EBX\n");
                    fprintf(fp,"\timul EAX, dword [programdata + %d]\n", fracpart2);
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tcmp EDX, 50\n");
                    fprintf(fp,"\tjl %d\n", *nextlabel);
                    fprintf(fp,"\tinc EDI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDX, -50\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tdec EDI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;

                    fprintf(fp,"\tmov EDX, 0\n");
                    fprintf(fp,"\tidiv 100\n");
                    fprintf(fp,"\tadd ESI, EAX\n");
                    fprintf(fp,"\tadd EDI, EDX\n");
                    fprintf(fp,"\tcmp EDI, 100\n");
                    fprintf(fp,"\tjl %d\n",*nextlabel);
                    fprintf(fp,"\tsub EDI, 100\n");
                    fprintf(fp,"\tinc ESI\n");
                    fprintf(fp,"\tjmp %d\n", *nextlabel + 1);
                    fprintf(fp,"\tl%d:\n", *nextlabel);
                    fprintf(fp,"\tcmp EDI, -100\n");
                    fprintf(fp,"\tjg %d\n", *nextlabel + 1);
                    fprintf(fp,"\tadd EDI, 100\n");
                    fprintf(fp,"\tdec ESI\n");
                    fprintf(fp,"\tl%d:\n", *nextlabel + 1);
                    *nextlabel += 2;
                    
                    int offset2 = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,NULL);
                    fprintf(fp,"\tmov dword [programdata + %d], ESI\n", offset2 );
                    fprintf(fp,"\tmov dword [programdata + %d], EDI\n", offset2 + 4 );
                    registers[ESI]->offsetIfPresent = offset2;
                    registers[EDI]->offsetIfPresent = offset2 + 4;

                }
                break;
            }
        
        }//end switch for opcode

}

int isNotRecordOpcode(int opcode){
    switch(opcode){
    case PLUS_RECORD:
    case MINUS_RECORD:
    case MULT_RECORD:
    case DIV_RECORD:
    case JLT_RECORD:
    case JLE_RECORD:
    case JGT_RECORD:
    case JGE_RECORD:
    case JEQ_RECORD:
    case JNE_RECORD:
    case ASSIGN_RECORD:
    case WRITE_RECORD:
        {
            return 0;
        }
    default:
        {
            return 1;
        }
    }

}

void makeScopeTables(ASTNode *head, recordsHashTable *rht, allFunctionsHashTable *afht, scopeHashTable *globalScope, quadruple *q){
    int *offset = (int *)malloc(sizeof(int));
    int *maxoffset = (int *)malloc(sizeof(int));
    *maxoffset = 0;
    *offset = -1;
    dfsForScopeTables(head,rht,afht,NULL,globalScope,NULL,NULL,NULL,NULL,NULL,offset);
    //printf("%d %d %d\n",rht->f,afht->f,globalScope->f);
    //printAllFunctionsHashTable(afht);
    int *ptrAvailableLabels = (int *)malloc(sizeof(int));
    int *ptrAvailableTemporary = (int *)malloc(sizeof(int));
    *ptrAvailableLabels = 1;
    *ptrAvailableTemporary = 1;
    int *success = (int *)malloc(sizeof(int));
    *success = 1;
    dynamicArray *da = createDynamicArray(10);
    dfsForSemanticAnalysis(head,rht,afht,NULL,globalScope,NULL,NULL,NULL,NULL,0,0,-1,ptrAvailableLabels,ptrAvailableTemporary,-1,-1,1,success,q,da,maxoffset);
    //printDynamicArray(da);
    sortDynamicArray(da);
    //printDynamicArray(da);

    //printf("label:%d\n",*ptrAvailableLabels);

    //making datastructure for registers


    

    scopeHashTable *currentScope = searchEntryAllFunctionsHashTable("_main",afht)->data->scope;

    printf("\n\nmaxoffset: %d\n\n", *maxoffset);
    *maxoffset += globalScope->offset;
    *maxoffset += currentScope->offset;
    printf("\n\nmaxoffset: %d\n\n", *maxoffset);

    reg *rg[20];
    rg[0] = createRegisterEntry(EAX,"EAX");
    rg[1] = createRegisterEntry(EBX,"EBX");
    rg[2] = createRegisterEntry(ECX,"ECX");
    rg[3] = createRegisterEntry(EDX,"EDX");
    rg[4] = createRegisterEntry(AX,"AX");
    rg[5] = createRegisterEntry(BX,"BX");
    rg[6] = createRegisterEntry(CX,"CX");
    rg[7] = createRegisterEntry(DX,"DX");
    rg[8] = createRegisterEntry(AH,"AH");
    rg[9] = createRegisterEntry(AL,"AL");
    rg[10] = createRegisterEntry(BH,"BH");
    rg[11] = createRegisterEntry(BL,"BL");
    rg[12] = createRegisterEntry(CH,"CH");
    rg[13] = createRegisterEntry(CL,"CL");
    rg[14] = createRegisterEntry(DH,"DH");
    rg[15] = createRegisterEntry(DL,"DL");
    rg[16] = createRegisterEntry(ESI,"ESI");
    rg[17] = createRegisterEntry(EDI,"EDI");
    rg[18] = createRegisterEntry(ESP,"ESP");
    rg[19] = createRegisterEntry(EBP,"EBP");


    FILE *fp = fopen("code.asm","w");
    
    {
        fprintf(fp,"\tSECTION .data\n");
        fprintf(fp,"\tSECTION .bss\n");
        fprintf(fp,"programdata\tresd\t%d\n", *maxoffset);
        fprintf(fp,"\tSECTION .text\n");
        fprintf(fp,"\t\t\tglobal main\n");
        fprintf(fp,"main:\n");
        //fprintf(fp,"\tlea EBP, programdata\n");
        
        

        int i;
        char *type = NULL;
        for (i=0; i<q->index; i++){

            quadrupleNode *qn = q->instructions[i];
            ASTNode *temp = NULL;
            if (qn->arg1 != NULL && qn->arg1->type == UNION_ID){
                temp = qn->arg1->data->identifier;
                scopeHashNode *shn = searchEntryScopeHashTable(temp->lexemeCurrentNode,currentScope);
                if (shn == NULL){
                    shn = searchEntryScopeHashTable(temp->lexemeCurrentNode,globalScope);
                }
                if (shn != NULL){
                    if (strcmp(shn->data->typeName,"int") != 0 && strcmp(shn->data->typeName,"real") != 0 )
                    	type = shn->data->typeName;
                }

            }
            if (qn->arg2 != NULL && qn->arg2->type == UNION_ID){
                temp = qn->arg2->data->identifier;
                scopeHashNode *shn = searchEntryScopeHashTable(temp->lexemeCurrentNode,currentScope);
                if (shn == NULL){
                    shn = searchEntryScopeHashTable(temp->lexemeCurrentNode,globalScope);
                }
                if (shn != NULL){
                    if (strcmp(shn->data->typeName,"int") != 0 && strcmp(shn->data->typeName,"real") != 0 )
                    	type = shn->data->typeName;
                }
            }
            
            if (isNotRecordOpcode(qn->opcode)){
                makeCode(qn, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);    
            }
            else {
                recordHashNode *rhn = searchEntryRecordsHashTable(type,rht);
                int offarg1, offarg2, offres;
                offarg1 = getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,type);
                offarg2 = getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,type);
                offres = getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,type);
                typeNode *tn = rhn->data->type;
                quadrupleNode *temp = (quadrupleNode *)malloc(sizeof(quadrupleNode));
                temp->arg1 = (typedUnion *)malloc(sizeof(typedUnion));
                temp->arg2 = (typedUnion *)malloc(sizeof(typedUnion));
                temp->result = (typedUnion *)malloc(sizeof(typedUnion));
                temp->arg1->data = (operand *)malloc(sizeof(operand));
                temp->arg2->data = (operand *)malloc(sizeof(operand));
                temp->result->data = (operand *)malloc(sizeof(operand));
                
                //temp->arg1->type = UNION_OFFSET;
                //temp->arg2->type = UNION_OFFSET;
                //temp->result->type = UNION_OFFSET;

                switch (qn->opcode){
                case ASSIGN_RECORD:
                    {
                        while (tn != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? ASSIGN_INT : ASSIGN_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->result->type = UNION_OFFSET;
                            temp->arg1->data->offset = offarg1;
                            temp->result->data->offset = offres;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        break;

                    }

                case PLUS_RECORD:
                    {
                        while (tn != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? PLUS_INT : PLUS_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->arg2->type = UNION_OFFSET;
                            temp->result->type = UNION_OFFSET;
                            temp->arg1->data->offset = offarg1;
                            temp->arg2->data->offset = offarg2;
                            temp->result->data->offset = offres;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg2 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        break;

                    }
                
                case MINUS_RECORD:
                    {
                        while (tn != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? MINUS_INT : MINUS_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->arg2->type = UNION_OFFSET;
                            temp->result->type = UNION_OFFSET;
                            temp->arg1->data->offset = offarg1;
                            temp->arg2->data->offset = offarg2;
                            temp->result->data->offset = offres;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg2 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        break;

                    }

                case MULT_RECORD:
                    {
                        int flag = 2;
                        int flagint = 0;
                        if (qn->arg1->type == UNION_INT || qn->arg1->type == UNION_REAL  ){
                            flag = 1;
                        }
                        else if (qn->arg1->type == UNION_ID){
                            ASTNode *temp2 = qn->arg1->data->identifier;
                            scopeHashNode *shn = searchEntryScopeHashTable(temp2->lexemeCurrentNode,currentScope);
                            if (shn == NULL){
                                shn = searchEntryScopeHashTable(temp2->lexemeCurrentNode,globalScope);
                            }
                            if (shn != NULL){
                                if (strcmp(shn->data->typeName,"int") == 0 || strcmp(shn->data->typeName,"real") == 0 ){
                                    flag = 1;
                                    if (strcmp(shn->data->typeName,"int") == 0){
                                        flagint = 1;
                                    }
                                }
                            }
                        }  
                        
                        if (flag == 2){
                            if (qn->arg2->type == UNION_ID){
                                ASTNode *temp2 = qn->arg2->data->identifier;
                                scopeHashNode *shn = searchEntryScopeHashTable(temp2->lexemeCurrentNode,currentScope);
                                if (shn == NULL){
                                    shn = searchEntryScopeHashTable(temp2->lexemeCurrentNode,globalScope);
                                }
                                if (shn != NULL){
                                    if (strcmp(shn->data->typeName,"int") == 0 ){
                                        flagint = 1;
                                    }
                                }
                            }   
                        }

                        while (tn != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? MULT_INT : MULT_REAL;
                            if (flag == 1){
                                //right code
                                if (qn->arg1->type == UNION_ID){
                                    temp->arg1->type = UNION_OFFSET;
                                }
                                else {
                                    temp->arg1->type = (tn->fieldtype == TK_INT) ? UNION_INT : UNION_REAL;                                    
                                }
                                if (temp->arg1->type == UNION_INT){
                                    temp->arg1->data->int_val = (qn->arg1->type == UNION_INT) ? qn->arg1->data->int_val : (int) qn->arg1->data->real_val;
                                }
                                else if (temp->arg1->type == UNION_REAL){
                                    temp->arg1->data->real_val = (qn->arg1->type == UNION_INT) ? (float)qn->arg1->data->int_val : qn->arg1->data->real_val;
                                }
                                else {
                                    temp->arg1->data->offset = offarg1;
                                }

                                //right code
                                temp->arg2->type = UNION_OFFSET;
                                temp->arg2->data->offset = offarg2;
                            }
                            else {
                                if (qn->arg2->type == UNION_ID){
                                    temp->arg2->type = UNION_OFFSET;
                                }
                                else {
                                    temp->arg2->type = (tn->fieldtype == TK_INT) ? UNION_INT : UNION_REAL;                                    
                                }
                                if (temp->arg2->type == UNION_INT){
                                    temp->arg2->data->int_val = (qn->arg2->type == UNION_INT) ? qn->arg2->data->int_val : (int) qn->arg2->data->real_val;
                                }
                                else if (temp->arg2->type == UNION_REAL){
                                    temp->arg2->data->real_val = (qn->arg2->type == UNION_INT) ? (float)qn->arg2->data->int_val : qn->arg2->data->real_val;
                                }
                                else {
                                    temp->arg2->data->offset = offarg2;
                                }

                                //right code
                                temp->arg1->type = UNION_OFFSET;
                                temp->arg1->data->offset = offarg1;

                            }
                            temp->result->type = UNION_OFFSET;
                            temp->result->data->offset = offres;

                            if (flagint == 1 && temp->opcode == MULT_REAL){
                                int off = (flag == 1) ? offarg1 + 4 : offarg2 + 4;
                                fprintf(fp,"\tmov ESP, dword [programdata + %d]\n", off);
                                fprintf(fp,"\tmov dword [programdata + %d], 0\n", off);
                            }

                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);

                            if (flagint == 1 && temp->opcode == MULT_REAL){
                                int off = (flag == 1) ? offarg1 + 4 : offarg2 + 4;
                                fprintf(fp,"\tmov dword [programdata + %d], ESP\n", off);
                            }

                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            if (flag == 1){
                                offarg2 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );    
                            }
                            else {
                                offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );    
                            }
                            tn = tn->next;
                        }
                        break;

                    }

                case JEQ_RECORD:
                    {

                        int width = rhn->data->width;
                        int j = 0;
                        int reg1 = getEmptyRegister(rg);
                        if (reg1 == -1){
                            reg1 = EDI;
                        }
                        rg[reg1]->offsetIfPresent = -1;
                        while (j < width-4 ){
                            fprintf(fp,"\tmov %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg1 + j);
                            fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg2 + j);
                            fprintf(fp,"\tjne l%d\n", *ptrAvailableLabels);
                            j += 4;
                        }
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg1 + j);
                        fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg2 + j);
                        fprintf(fp,"\tje l%d\n", qn->result->data->jump_label);
                        fprintf(fp,"\tl%d:\n", *ptrAvailableLabels);
                        *ptrAvailableLabels++;
                        break; 
                    }

                case JNE_RECORD:
                    {

                        int width = rhn->data->width;
                        int j = 0;
                        int reg1 = getEmptyRegister(rg);
                        if (reg1 == -1){
                            reg1 = EDI;
                        }
                        rg[reg1]->offsetIfPresent = -1;
                        while (j < width-4 ){
                            fprintf(fp,"\tmov %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg1 + j);
                            fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg2 + j);
                            fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                            j += 4;
                        }
                        fprintf(fp,"\tmov %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg1 + j);
                        fprintf(fp,"\tcmp %s, dword [programdata + %d]\n", rg[reg1]->regName ,offarg2 + j);
                        fprintf(fp,"\tjne l%d\n", qn->result->data->jump_label);
                        break; 
                    }
                
                case JLT_RECORD:
                    {
                        int lbl = *ptrAvailableLabels;
                        *ptrAvailableLabels++;
                        while (tn->next != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? JLT_INT : JLT_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->arg2->type = UNION_OFFSET;
                            temp->result->type = UNION_LABEL;
                            temp->arg1->data->offset = offarg1;
                            temp->arg2->data->offset = offarg2;
                            temp->result->data->offset = qn->result->data->jump_label;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            temp->opcode = (tn->fieldtype == TK_INT) ? JGT_INT : JGT_REAL;
                            temp->result->data->offset = lbl;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        temp->opcode = (tn->fieldtype == TK_INT) ? JLT_INT : JLT_REAL;
                        temp->arg1->type = UNION_OFFSET;
                        temp->arg2->type = UNION_OFFSET;
                        temp->result->type = UNION_LABEL;
                        temp->arg1->data->offset = offarg1;
                        temp->arg2->data->offset = offarg2;
                        temp->result->data->offset = qn->result->data->jump_label;
                        makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                        fprintf(fp,"\tl%d:\n", lbl);
                        break;
                    }

                case JLE_RECORD:
                    {
                        int lbl = *ptrAvailableLabels;
                        *ptrAvailableLabels++;
                        while (tn->next != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? JLT_INT : JLT_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->arg2->type = UNION_OFFSET;
                            temp->result->type = UNION_LABEL;
                            temp->arg1->data->offset = offarg1;
                            temp->arg2->data->offset = offarg2;
                            temp->result->data->offset = qn->result->data->jump_label;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            temp->opcode = (tn->fieldtype == TK_INT) ? JGT_INT : JGT_REAL;
                            temp->result->data->offset = lbl;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        temp->opcode = (tn->fieldtype == TK_INT) ? JLE_INT : JLE_REAL;
                        temp->arg1->type = UNION_OFFSET;
                        temp->arg2->type = UNION_OFFSET;
                        temp->result->type = UNION_LABEL;
                        temp->arg1->data->offset = offarg1;
                        temp->arg2->data->offset = offarg2;
                        temp->result->data->offset = qn->result->data->jump_label;
                        makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                        fprintf(fp,"\tl%d:\n", lbl);
                        break;
                    }

                case JGT_RECORD:
                    {
                        int lbl = *ptrAvailableLabels;
                        *ptrAvailableLabels++;
                        while (tn->next != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? JGT_INT : JGT_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->arg2->type = UNION_OFFSET;
                            temp->result->type = UNION_LABEL;
                            temp->arg1->data->offset = offarg1;
                            temp->arg2->data->offset = offarg2;
                            temp->result->data->offset = qn->result->data->jump_label;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            temp->opcode = (tn->fieldtype == TK_INT) ? JLT_INT : JLT_REAL;
                            temp->result->data->offset = lbl;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        temp->opcode = (tn->fieldtype == TK_INT) ? JGT_INT : JGT_REAL;
                        temp->arg1->type = UNION_OFFSET;
                        temp->arg2->type = UNION_OFFSET;
                        temp->result->type = UNION_LABEL;
                        temp->arg1->data->offset = offarg1;
                        temp->arg2->data->offset = offarg2;
                        temp->result->data->offset = qn->result->data->jump_label;
                        makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                        fprintf(fp,"\tl%d:\n", lbl);
                        break;
                    }
                
                case JGE_RECORD:
                    {
                        
                        int lbl = *ptrAvailableLabels;
                        *ptrAvailableLabels++;
                        while (tn->next != NULL){
                            temp->opcode = (tn->fieldtype == TK_INT) ? JGT_INT : JGT_REAL;
                            temp->arg1->type = UNION_OFFSET;
                            temp->arg2->type = UNION_OFFSET;
                            temp->result->type = UNION_LABEL;
                            temp->arg1->data->offset = offarg1;
                            temp->arg2->data->offset = offarg2;
                            temp->result->data->offset = qn->result->data->jump_label;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            temp->opcode = (tn->fieldtype == TK_INT) ? JLT_INT : JLT_REAL;
                            temp->result->data->offset = lbl;
                            makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                            offres += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            offarg1 += ( (tn->fieldtype == TK_INT) ? INT_SIZE : REAL_SIZE );
                            tn = tn->next;
                        }
                        temp->opcode = (tn->fieldtype == TK_INT) ? JGE_INT : JGE_REAL;
                        temp->arg1->type = UNION_OFFSET;
                        temp->arg2->type = UNION_OFFSET;
                        temp->result->type = UNION_LABEL;
                        temp->arg1->data->offset = offarg1;
                        temp->arg2->data->offset = offarg2;
                        temp->result->data->offset = qn->result->data->jump_label;
                        makeCode(temp, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);
                        fprintf(fp,"\tl%d:\n", lbl);
                        break;
                    
                    }

                }//END SWITCH
            
            }//END ELSE
            
            //printf("Arg1_Offset: %d ", getOffset(qn->arg1,qn->opcode,currentScope,globalScope,rht,NULL));
            //printf("Arg2_Offset: %d ", getOffset(qn->arg2,qn->opcode,currentScope,globalScope,rht,type));
            //printf("Res_Offset: %d\n", getOffset(qn->result,qn->opcode,currentScope,globalScope,rht,type));
        }

        fprintf(fp,"\tmov EBX, 0\n");
        fprintf(fp,"\tmov EAX, 1\n");
        fprintf(fp,"\tint 0x80\n");
    }
    

    
    /*
    //printing registers
    int i;
    for (i=0;i<20;i++){
        printRegister(rg[i]);
    }
    */
    
    //makeCode(q, currentScope, globalScope, rht, da, rg, 20, ptrAvailableLabels, fp);

}

