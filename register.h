#ifndef REGISTER_H_INCLUDED
#define REGISTER_H_INCLUDED

#define INT_SIZE 2
#define REAL_SIZE 4

reg *createRegisterEntry(int regNum, char *regName);
void printRegister(reg *rg);

#endif // REGISTER_H_INCLUDED
