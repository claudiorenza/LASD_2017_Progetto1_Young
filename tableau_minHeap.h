#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef int ** TABLEAU;
typedef int * TABLEAUel;

void talbeau_minHeap_heapify(TABLEAU T_young, int idx);

int talbeau_minHeap_sinistro(int idx);

int talbeau_minHeap_destro(int idx);

int talbeau_minHeap_padre(int idx);

void talbeau_minHeap_buildHeap(TABLEAU T_young);

void talbeau_minHeap_swap(TABLEAU T_young, int idx_1, int idx_2);


#endif