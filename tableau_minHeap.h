#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef int ** TABLEAU;
typedef int * TABLEAUel;

void talbleau_minHeap_heapify(TABLEAU T_young, int idx);

int talbleau_minHeap_sinistro(int idx);

int talbleau_minHeap_destro(int idx);

int talbleau_minHeap_padre(int idx);

void talbleau_minHeap_buildHeap(TABLEAU T_young);

void talbleau_minHeap_swap(TABLEAU T_young, int idx_1, int idx_2);


#endif