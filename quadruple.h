#ifndef QUADRUPLE_H_INCLUDED
#define QUADRUPLE_H_INCLUDED


quadruple *createQuadruple(int capacity);
void addEntryQuadruple(quadruple *q, int opcode, int arg1Type, int arg2Type, int resultType, void *arg1, void *arg2, void *res, int label);
void printQuadrupleNode(quadrupleNode *qn);
void printQuadruple(quadruple *q);
void addQuad(quadruple *q,int opcode,int arg1type,int arg2type,void *a1,void *a2,int label, int jumpIfTrue, int jumpIfFalse, int index, dynamicArray *da);
#endif // QUADRUPLE_H_INCLUDED
