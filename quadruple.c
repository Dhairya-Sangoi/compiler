#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArrayDef.h"
#include "dynamicArray.h"
#include "parseTreeDef.h"
#include "quadrupleDef.h"
#include "enum.h"
#include "operators.h"


quadruple *createQuadruple(int capacity){
    quadruple *q = (quadruple *)malloc(sizeof(quadruple));
    q->capacity = capacity;
    q->index = 0;
    q->instructions = (quadrupleNode **)malloc(sizeof(quadrupleNode *) * capacity);
    int i;
    for (i=0;i<capacity;i++){
        q->instructions[i] = NULL;
    }
    return q;
}

void addEntryQuadruple(quadruple *q, int opcode, int arg1Type, int arg2Type, int resultType, void *arg1, void *arg2, void *res, int label){
    quadrupleNode *qn = (quadrupleNode *)malloc(sizeof(quadrupleNode));
    qn->opcode = opcode;
    qn->label = label;

    typedUnion *argument1;// = (typedUnion *)malloc(sizeof(typedUnion));

    switch(arg1Type){
    case UNION_INT:
        {
            argument1 = (typedUnion *)malloc(sizeof(typedUnion));
            argument1->type = arg1Type;
            argument1->data = (operand *)malloc(sizeof(operand));
            argument1->data->int_val = *((int *)arg1);
            //printf("%d\n",argument1->data->int_val);
            break;
        }
     case UNION_REAL:
        {
            argument1 = (typedUnion *)malloc(sizeof(typedUnion));
            argument1->type = arg1Type;
            argument1->data = (operand *)malloc(sizeof(operand));
            argument1->data->real_val = *((float *)arg1);
            break;
        }
    case UNION_ID:
        {
            argument1 = (typedUnion *)malloc(sizeof(typedUnion));
            argument1->type = arg1Type;
            argument1->data = (operand *)malloc(sizeof(operand));
            argument1->data->identifier = (ASTNode *)arg1;
            break;
        }
    case UNION_TEMPORARY:
        {
            argument1 = (typedUnion *)malloc(sizeof(typedUnion));
            argument1->type = arg1Type;
            argument1->data = (operand *)malloc(sizeof(operand));
            argument1->data->temporary_identifier = *((int *)arg1);
            break;
        }
    case UNION_LABEL:
        {
            argument1 = (typedUnion *)malloc(sizeof(typedUnion));
            argument1->type = arg1Type;
            argument1->data = (operand *)malloc(sizeof(operand));
            argument1->data->jump_label = *((int *)arg1);
            break;
        }
    default:
        {
            argument1 = NULL;
            break;
        }
    }
    qn->arg1 = argument1;

    typedUnion *argument2;// = (typedUnion *)malloc(sizeof(typedUnion));

    switch(arg2Type){
    case UNION_INT:
        {
            argument2 = (typedUnion *)malloc(sizeof(typedUnion));
            argument2->type = arg2Type;
            argument2->data = (operand *)malloc(sizeof(operand));
            argument2->data->int_val = *((int *)arg2);
            break;
        }
     case UNION_REAL:
        {
            argument2 = (typedUnion *)malloc(sizeof(typedUnion));
            argument2->type = arg2Type;
            argument2->data = (operand *)malloc(sizeof(operand));
            argument2->data->real_val = *((float *)arg2);
            break;
        }
    case UNION_ID:
        {
            argument2 = (typedUnion *)malloc(sizeof(typedUnion));
            argument2->type = arg2Type;
            argument2->data = (operand *)malloc(sizeof(operand));
            argument2->data->identifier = (ASTNode *)arg2;
            break;
        }
    case UNION_TEMPORARY:
        {
            argument2 = (typedUnion *)malloc(sizeof(typedUnion));
            argument2->type = arg2Type;
            argument2->data = (operand *)malloc(sizeof(operand));
            argument2->data->temporary_identifier = *((int *)arg2);
            break;
        }
    case UNION_LABEL:
        {
            argument2 = (typedUnion *)malloc(sizeof(typedUnion));
            argument2->type = arg2Type;
            argument2->data = (operand *)malloc(sizeof(operand));
            argument2->data->jump_label = *((int *)arg2);
            break;
        }
    default:
        {
            argument2 = NULL;
            break;
        }
    }
    qn->arg2 = argument2;


    typedUnion *result;// = (typedUnion *)malloc(sizeof(typedUnion));

    switch(resultType){
    case UNION_INT:
        {
            result = (typedUnion *)malloc(sizeof(typedUnion));
            result->type = resultType;
            result->data = (operand *)malloc(sizeof(operand));
            result->data->int_val = *((int *)res);
            break;
        }
     case UNION_REAL:
        {
            result = (typedUnion *)malloc(sizeof(typedUnion));
            result->type = resultType;
            result->data = (operand *)malloc(sizeof(operand));
            result->data->real_val = *((float *)res);
            break;
        }
    case UNION_ID:
        {
            result = (typedUnion *)malloc(sizeof(typedUnion));
            result->type = resultType;
            result->data = (operand *)malloc(sizeof(operand));
            result->data->identifier = (ASTNode *)res;
            break;
        }
    case UNION_TEMPORARY:
        {
            result = (typedUnion *)malloc(sizeof(typedUnion));
            result->type = resultType;
            result->data = (operand *)malloc(sizeof(operand));
            result->data->temporary_identifier = *((int *)res);
            break;
        }
    case UNION_LABEL:
        {
            result = (typedUnion *)malloc(sizeof(typedUnion));
            result->type = resultType;
            result->data = (operand *)malloc(sizeof(operand));
            result->data->jump_label = *((int *)res);
            break;
        }
    default:
        {
            result = NULL;
            break;
        }
    }
    qn->result = result;

    if (q->capacity == q->index){
        q->instructions = (quadrupleNode **)realloc(q->instructions,(q->capacity) * 2 * sizeof(quadrupleNode *));
        q->capacity *= 2;
    }
    q->instructions[q->index] = qn;
    q->index++;
    return;
}

void printTypedUnion(typedUnion *tu){
    if (tu == NULL){
        printf("---- ");
        return;
    }

    switch(tu->type){
    case UNION_INT:
        {
            printf("INT->%d",tu->data->int_val);
            break;
        }
    case UNION_REAL:
        {
            printf("Real->%lf",tu->data->real_val);
            break;
        }
    case UNION_TEMPORARY:
        {
            printf("Temporary->%d",tu->data->temporary_identifier);
            break;
        }
    case UNION_ID:
        {
            if (tu->data->identifier->index == TK_ID){
                printf("ID->%s",tu->data->identifier->lexemeCurrentNode);
            }
            else {
                printf("ID->%s",tu->data->identifier->child->lexemeCurrentNode);
                printf(" FIELD->%s",tu->data->identifier->child->next->lexemeCurrentNode);
            }
            break;
        }
    case UNION_LABEL:
        {
            printf("JumpLabel->%d",tu->data->jump_label);
            break;
        }
    }
    return;
}

void printQuadrupleNode(quadrupleNode *qn){
    printf(" Label: %d ",qn->label);
    printf("Opcode: %d ",qn->opcode);
    printf("Argument1: ");
    printTypedUnion(qn->arg1);
    printf(" Argument2: ");
    printTypedUnion(qn->arg2);
    printf(" Result: ");
    printTypedUnion(qn->result);
    printf("\n");
    return;
}

void printQuadruple(quadruple *q){
    int i;
    for (i=0;i<q->index;i++){
        printQuadrupleNode(q->instructions[i]);
    }
    return;
}

void addQuad(quadruple *q,int opcode,int arg1type,int arg2type,void *a1,void *a2,int label, int jumpIfTrue, int jumpIfFalse, int index, dynamicArray *da){
        if (jumpIfTrue == -1 && jumpIfFalse == -1){
            return;
        }
        if (jumpIfTrue == -1 && jumpIfFalse != -1) {
                switch(index){
                case TK_LT:
                    {
                        index = TK_GE;
                        break;
                    }
                case TK_LE:
                    {
                        index = TK_GT;
                        break;
                    }
                case TK_GT:
                    {
                        index = TK_LE;
                        break;
                    }
                case TK_GE:
                    {
                        index = TK_LT;
                        break;
                    }
                case TK_EQ:
                    {
                        index = TK_NE;
                        break;
                    }
                case TK_NE:
                    {
                        index = TK_EQ;
                        break;
                    }
                }
        }
            switch (index){
            case TK_LT:
                {
                    switch(opcode){
                    case 1:
                        {
                            opcode = JLT_INT;
                            break;
                        }
                    case 2:
                        {
                            opcode = JLT_REAL;
                            break;
                        }
                    case 3:
                        {
                            opcode = JLT_RECORD;
                            break;
                        }
                    }
                    break;
                }
            case TK_LE:
                {
                    switch(opcode){
                    case 1:
                        {
                            opcode = JLE_INT;
                            break;
                        }
                    case 2:
                        {
                            opcode = JLE_REAL;
                            break;
                        }
                    case 3:
                        {
                            opcode = JLE_RECORD;
                            break;
                        }
                    }
                    break;
                }
            case TK_GT:
                {
                    switch(opcode){
                    case 1:
                        {
                            opcode = JGT_INT;
                            break;
                        }
                    case 2:
                        {
                            opcode = JGT_REAL;
                            break;
                        }
                    case 3:
                        {
                            opcode = JGT_RECORD;
                            break;
                        }
                    }
                    break;
                }
            case TK_GE:
                {
                    switch(opcode){
                    case 1:
                        {
                            opcode = JGE_INT;
                            break;
                        }
                    case 2:
                        {
                            opcode = JGE_REAL;
                            break;
                        }
                    case 3:
                        {
                            opcode = JGE_RECORD;
                            break;
                        }
                    }
                    break;
                }
            case TK_EQ:
                {
                    switch(opcode){
                    case 1:
                        {
                            opcode = JEQ_INT;
                            break;
                        }
                    case 2:
                        {
                            opcode = JEQ_REAL;
                            break;
                        }
                    case 3:
                        {
                            opcode = JEQ_RECORD;
                            break;
                        }
                    }
                    break;
                }

            case TK_NE:
                {
                    switch(opcode){
                    case 1:
                        {
                            opcode = JNE_INT;
                            break;
                        }
                    case 2:
                        {
                            opcode = JNE_REAL;
                            break;
                        }
                    case 3:
                        {
                            opcode = JNE_RECORD;
                            break;
                        }
                    }
                    break;
                }

            }


        if (jumpIfTrue != -1 && jumpIfFalse != -1){
            addEntryQuadruple(q,opcode,arg1type,arg2type,UNION_LABEL,a1,a2,&jumpIfTrue,label);
            addEntryDynamicArray(jumpIfTrue,da);
            addEntryQuadruple(q,JUMP,UNION_LABEL,-1,-1,&jumpIfFalse,NULL,NULL,-1);
            addEntryDynamicArray(jumpIfFalse,da);
        }
        else {
            if (jumpIfTrue == -1){
                addEntryQuadruple(q,opcode,arg1type,arg2type,UNION_LABEL,a1,a2,&jumpIfFalse,label);
                addEntryDynamicArray(jumpIfFalse,da);
            }
            else {
                addEntryQuadruple(q,opcode,arg1type,arg2type,UNION_LABEL,a1,a2,&jumpIfTrue,label);
                addEntryDynamicArray(jumpIfTrue,da);
            }

        }
        return;
}






