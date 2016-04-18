#ifndef DYNAMICARRAY_H_INCLUDED
#define DYNAMICARRAY_H_INCLUDED

dynamicArray *createDynamicArray(int capacity);
void addEntryDynamicArray(int lbl, dynamicArray *da);
int searchEntryDynamicArray(int lbl, dynamicArray *da);
int searchEntryDynamicArrayAux(int lbl, int *da, int start, int end);
void sortDynamicArray(dynamicArray *da);
void sortDynamicArrayAux(int *arr, int start, int end);
void printDynamicArray(dynamicArray *da);

#endif // DYNAMICARRAY_H_INCLUDED
