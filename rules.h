//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED

//create rule
ruleq *createRule(int ruleno, int nontermindex);

//create rule node
ruleNode *createRuleNode(int val);

//add node to end of the rule
void addRuleNodeEnd(ruleq *r, int val);

//print rule
void printRule(ruleq *q);

#endif // RULES_H_INCLUDED
