#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef int ** PRQUEUE;
typedef int * PRQUEUEel;

void matrix_minHeap_heapify(PRQUEUE coda, int idx);

int matrix_minHeap_sinistro(int idx);

int matrix_minHeap_destro(int idx);

int matrix_minHeap_padre(int idx);

void matrix_minHeap_buildHeap(PRQUEUE coda);

void matrix_minHeap_swap(PRQUEUE coda, int idx_1, int idx_2);


#endif