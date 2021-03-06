#ifndef REGISTER_H_INCLUDED
#define REGISTER_H_INCLUDED

#define INT_SIZE 4
#define REAL_SIZE 8

reg *createRegisterEntry(int regNum, char *regName);
int searchRegister(reg **rgs, int size, int offset);
void addValueRegister(reg **rgs, int size, int rg, int offset);//add offset to rg register, remove offset from other registers
int getOffset(typedUnion *arg, int opcode, scopeHashTable *currentScope, scopeHashTable *globalScope, recordsHashTable *rht, char *type);
int getEmptyRegister(reg **rg);
void printRegister(reg *rg);
void flushRegisters(reg **rgs, int size);

#endif // REGISTER_H_INCLUDED
