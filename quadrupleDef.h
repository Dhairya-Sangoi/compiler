#ifndef QUADRUPLEDEF_H_INCLUDED
#define QUADRUPLEDEF_H_INCLUDED

#define UNION_INT 1
#define UNION_REAL 2
#define UNION_ID 3
#define UNION_TEMPORARY 4
#define UNION_LABEL 5

typedef parseTreeNode ASTNode;

typedef union operand{
    int int_val;
    float real_val;
    ASTNode *identifier;
    int temporary_identifier;
    int jump_label;
}operand;

typedef struct typedUnion{
    int type;
    operand *data;
}typedUnion;

typedef struct quadrupleNode{
    int opcode;
    typedUnion *arg1;
    typedUnion *arg2;
    typedUnion *result;
    int label;
}quadrupleNode;

typedef struct quadruple{
    int capacity;
    int index;
    quadrupleNode **instructions;
}quadruple;

#endif // QUADRUPLEDEF_H_INCLUDED
