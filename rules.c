//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rulesDef.h"

ruleq *createRule(int ruleno, int nontermindex){
    ruleq *r = (ruleq *)malloc(sizeof(ruleq));
    r->index = ruleno;
    r->head = NULL;
    r->nontermindex = nontermindex;
    return r;
}

ruleNode *createRuleNode(int val){
    ruleNode *rn = (ruleNode *)malloc(sizeof(ruleNode));
    rn->val = val;
    rn->next = NULL;
    return rn;
}

void addRuleNodeEnd(ruleq *r, int val){
    if (r->head == NULL){
        r->head = createRuleNode(val);
        return;
    }
    else {
        ruleNode *temp = r->head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = createRuleNode(val);
        return;
    }
}

void printRule(ruleq *q){
    if (q==NULL){
        return;
    }
    else {
        printf("rule:%d->", q->index);
        ruleNode *temp = q->head;
        while (temp!=NULL){
            printf("%d ",temp->val);
            temp = temp->next;
        }
        printf("\n");
        return;
    }
}
