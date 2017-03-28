#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef int ** TABLEAU;
typedef int * TABLEAUel;

void talbleau_minHeap_heapify(TABLEAU coda, int idx);

int talbleau_minHeap_sinistro(int idx);

int talbleau_minHeap_destro(int idx);

int talbleau_minHeap_padre(int idx);

void talbleau_minHeap_buildHeap(TABLEAU coda);

void talbleau_minHeap_swap(TABLEAU coda, int idx_1, int idx_2);


#endif