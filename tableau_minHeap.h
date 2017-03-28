#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef int ** TABLEAU;
typedef int * TABLEAUel;

void tableau_minHeap_heapify(TABLEAU T_young, int idx);

int tableau_minHeap_sinistro(int idx);

int tableau_minHeap_destro(int idx);

int tableau_minHeap_padre(int idx);

void tableau_minHeap_buildHeap(TABLEAU T_young);

void tableau_minHeap_swap(TABLEAU T_young, int idx_1, int idx_2);


#endif