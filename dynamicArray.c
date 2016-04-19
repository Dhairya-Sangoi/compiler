#include <stdio.h>
#include <stdlib.h>
#include "dynamicArrayDef.h"
#include "dynamicArray.h"

dynamicArray *createDynamicArray(int capacity){
    dynamicArray *da = (dynamicArray *)malloc(sizeof(dynamicArray));
    da->capacity = capacity;
    da->size = 0;
    da->arr = (int *)malloc(sizeof(int) * capacity);
    return da;
}

void addEntryDynamicArray(int lbl, dynamicArray *da){
    if (searchEntryDynamicArray(lbl,da) == 1){
        return;
    }
    if (da->size == da->capacity){
        da->arr = (int *)realloc(da->arr,2*(da->capacity) * sizeof(int));
        da->capacity *= 2;
    }
    da->arr[da->size] = lbl;
    da->size += 1;
    return;
}

int searchEntryDynamicArrayAux(int lbl, int *arr, int start, int end){
    int mid;
    if (start > end){
        return 0;
    }
    else if (start == end){
        //printf("here start = %d\n", start);
        int ret = (arr[start] == lbl) ? 1 : 0;
        if (ret == 1){
            printf("here start = %d\n", start);
        }
        return ret;
    }
    else {
        mid = start + end;
        mid /= 2;
        if (arr[mid] == lbl){
            printf("here index = %d\n", mid);
            return 1;
        }
        else if (arr[mid] > lbl){
            return searchEntryDynamicArrayAux(lbl,arr,start,mid-1);
        }
        else {
            return searchEntryDynamicArrayAux(lbl,arr,mid+1,end);
        }
    }
}

int searchEntryDynamicArray(int lbl, dynamicArray *da){
    return searchEntryDynamicArrayAux(lbl, da->arr, 0, da->size - 1);
}

void printDynamicArray(dynamicArray *da){
    int i;
    printf("Printing Dynamic Array:\n");
    for (i=0;i<da->size;i++){
        printf("%d ",da->arr[i]);
    }
    printf("\n");
}

void sortDynamicArray(dynamicArray *da){
    sortDynamicArrayAux(da->arr,0,da->size-1);
}

void sortDynamicArrayAux(int *arr, int start, int end){
    int pivot,j,temp,i;
     if(start<end){
         pivot=start;
         i=start;
         j=end;
         while(i<j){
             while(arr[i]<=arr[pivot] && i<end)
                 i++;
             while(arr[j]>arr[pivot])
                 j--;
             if(i<j){
                 temp=arr[i];
                  arr[i]=arr[j];
                  arr[j]=temp;
             }
         }

         temp=arr[pivot];
         arr[pivot]=arr[j];
         arr[j]=temp;
         sortDynamicArrayAux(arr,start,j-1);
         sortDynamicArrayAux(arr,j+1,end);
    }
}

