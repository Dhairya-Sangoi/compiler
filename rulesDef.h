//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef RULESDEF_H_INCLUDED
#define RULESDEF_H_INCLUDED

//structure definition for rhs of rule
typedef struct ruleNode{
    int val;
    struct ruleNode *next;
}ruleNode;

//structure definition for each rule
typedef struct ruleq{
    int index;
    ruleNode *head;
    int nontermindex;
}ruleq;


#endif // RULESDEF_H_INCLUDED
