//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseTreeDef.h"

parseTreeNode *createParseTreeNode(parseTreeNode *parent, char *lCN, int lineno, char *token, int isNumber, int iLN, char *nodeSymbol, int index, int ruleno){
    parseTreeNode *ptn = (parseTreeNode *)malloc(sizeof(parseTreeNode));
    ptn->child = NULL;
    ptn->parent = parent;
    ptn->lexemeCurrentNode = lCN;
    ptn->lineno = lineno;
    ptn->token = token;
    ptn->isNumber = isNumber;
    if (isNumber){
        ptn->valueIfNumber = atof(lCN);
    }
    ptn->isLeafNode = iLN;
    ptn->nodeSymbol = nodeSymbol;
    ptn->next = NULL;
    ptn->index = index;
    ptn->ruleno = ruleno;
    ptn->replacedNode = index;
    ptn->replacedNodesCount = 0;
    return ptn;
}

parseTree *createParseTree(){
    parseTree *pt = (parseTree *)malloc(sizeof(parseTree));
    pt->head = NULL;
    return pt;
}

void addChild(parseTreeNode *parent, parseTreeNode *child){
    if (parent->child == NULL){
        parent->child = child;
    }
    else {
        parseTreeNode *temp = parent->child;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = child;
    }
    child->parent = parent;
    return;
}

void printParseTreeNode(parseTreeNode *ptn, FILE *fp){
    //fprintf(fp,"lexemeCurrentNode: %s, ",ptn->lexemeCurrentNode);
    fprintf(fp,"Line: %-4d  ",ptn->lineno);
    fprintf(fp,"Tok: %-14s  ",ptn->token);
    fprintf(fp,"Val: ");
    if (ptn->isNumber){
        if (strcmp("TK_NUM",ptn->token)==0){
            fprintf(fp,"%-4d  ",(int)ptn->valueIfNumber);
        }
        else {
            fprintf(fp,"%-4.2f  ",ptn->valueIfNumber);
        }
    }
    else {
        fprintf(fp,"NAN   ");
    }
    fprintf(fp,"Parent: %-26s  ",(ptn->parent) ? ptn->parent->nodeSymbol : NULL);
    fprintf(fp,"Leaf: %-3s  ",(ptn->isLeafNode) ? "yes" : "no");
    fprintf(fp,"Node: %-26s  ",ptn->nodeSymbol);
    fprintf(fp,"Lexeme: %-20s  ",ptn->lexemeCurrentNode);
    fprintf(fp,"Index: %-4d ",ptn->index);
    fprintf(fp,"ParentIndex: %-4d ",(ptn->parent) ? ptn->parent->index : -1);
    fprintf(fp,"ReplacedNode: %-4d ",ptn->replacedNode); //to be commented
    fprintf(fp,"RuleNo: %-4d\n",ptn->ruleno); // to be commented
    return;
}

void dfsParseTree(parseTreeNode *ptn, FILE *fp){
    if (ptn == NULL){
        return;
    }
    parseTreeNode *curr = ptn->child;
    while (curr!=NULL){
        dfsParseTree(curr,fp);
        curr = curr->next;
    }

    printParseTreeNode(ptn,fp);
    return;
}

