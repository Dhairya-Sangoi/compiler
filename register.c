#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "registerDef.h"

reg *createRegisterEntry(int regNum, char *regName){
	reg *rg = (reg *)malloc(sizeof(reg));
	rg->regNum = regNum;
	rg->regName = (char *)malloc(sizeof(char) * (strlen(regName) + 1) );
	strcpy(rg->regName,regName);
	rg->offsetIfPresent = -1;
	return rg;
}

void printRegister(reg *rg){
	printf("Name:%s Index:%d Offset:%d\n",rg->regName, rg->regNum, rg->offsetIfPresent);
	return;
}