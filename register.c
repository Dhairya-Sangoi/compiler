#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseTreeDef.h"
#include "registerDef.h"
#include "scopeHashTableDef.h"
#include "scopeHashTable.h"
#include "recordsHashTableDef.h"
#include "recordsHashTable.h"
#include "quadrupleDef.h"

#include "enum.h"
#include "operators.h"

reg *createRegisterEntry(int regNum, char *regName){
	reg *rg = (reg *)malloc(sizeof(reg));
	rg->regNum = regNum;
	rg->regName = (char *)malloc(sizeof(char) * (strlen(regName) + 1) );
	strcpy(rg->regName,regName);
	rg->offsetIfPresent = -1;
	return rg;
}

int searchRegister(reg **rgs, int size, int offset){
	int i;
	for (i=0;i<size;i++){
		if (rgs[i]->offsetIfPresent == offset){
			return i;
		}
	}
	return -1;
}

void addValueRegister(reg **rgs, int size, int rg, int offset){
	int i;
	for (i=0;i<size;i++){
		if (rgs[i]->offsetIfPresent == offset && i != rg){
			rgs[i]->offsetIfPresent = -1;
		}
	}
	rgs[rg]->offsetIfPresent = offset;
	return;
}

int getOffset(typedUnion *arg, int opcode, scopeHashTable *currentScope, scopeHashTable *globalScope, recordsHashTable *rht, char *type){
	if (arg == NULL || arg->type == UNION_INT || arg->type == UNION_REAL || arg->type == UNION_LABEL){
		return -1;
	}
	if (arg->type == UNION_TEMPORARY){
		int offset = globalScope->offset + currentScope->offset;
		switch(opcode){
		case PLUS_RECORD:
		case MINUS_RECORD:
		case MULT_RECORD:
		case DIV_RECORD:
		case ASSIGN_RECORD:
		case WRITE_RECORD:
		case JLT_RECORD:
		case JLE_RECORD:
		case JGT_RECORD:
		case JGE_RECORD:
		case JEQ_RECORD:
		case JNE_RECORD:
			{
				recordHashNode *rhn = searchEntryRecordsHashTable(type,rht);
				offset += (arg->data->temporary_identifier - 1) * rhn->data->width;
				return offset;
			}

		case PLUS_INT:
		case MINUS_INT:
		case MULT_INT:
		case DIV_INT:
		case ASSIGN_INT:
		case WRITE_INT:
		case JLT_INT:
		case JLE_INT:
		case JGT_INT:
		case JGE_INT:
		case JEQ_INT:
		case JNE_INT:
			{
				offset += (arg->data->temporary_identifier - 1) * INT_SIZE;
				return offset;
			}
		
		case PLUS_REAL:
		case MINUS_REAL:
		case MULT_REAL:
		case DIV_REAL:
		case ASSIGN_REAL:
		case WRITE_REAL:
		case JLT_REAL:
		case JLE_REAL:
		case JGT_REAL:
		case JGE_REAL:
		case JEQ_REAL:
		case JNE_REAL:
			{
				offset += (arg->data->temporary_identifier - 1) * REAL_SIZE;
				return offset;
			}

		}//switch end
	}//if end
	if (arg->data->identifier->index == TK_ID){
		scopeHashNode *shn = searchEntryScopeHashTable(arg->data->identifier->lexemeCurrentNode,currentScope);
		if (shn != NULL){
			int offset = globalScope->offset;
			offset += shn->data->offset;
			return offset;
		}
		shn = searchEntryScopeHashTable(arg->data->identifier->lexemeCurrentNode,globalScope);
		if (shn != NULL){
			int offset = shn->data->offset;
			return offset;
		}
		return -1;
	}
	else {
		scopeHashNode *shn = searchEntryScopeHashTable(arg->data->identifier->child->lexemeCurrentNode,currentScope);
		int offset = 0;
		if (shn != NULL){
			offset = globalScope->offset;
		}
		else {
			shn = searchEntryScopeHashTable(arg->data->identifier->child->lexemeCurrentNode,globalScope);
		}
		offset += shn->data->offset;
		recordHashNode *rhn = searchEntryRecordsHashTable(shn->data->typeName,rht);
		if (rhn != NULL){
			scopeHashTable *sht = rhn->data->fields;
			shn = searchEntryScopeHashTable(arg->data->identifier->child->next->lexemeCurrentNode,sht);
			if (shn != NULL){
				offset += shn->data->offset;
				return offset;
			}
		}
		return -1;
	}
	return -1;
}

void printRegister(reg *rg){
	printf("Name:%s Index:%d Offset:%d\n",rg->regName, rg->regNum, rg->offsetIfPresent);
	return;
}
