#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef int *** TABLEAU;
typedef int ** TABLEAUptr;
typedef int * TABLEAUval;

void tableau_minHeap_heapify(TABLEAU T_young, int idx_row, int idx_col);

TABLEAUptr tableau_minHeap_sinistro(TABLEAU T_young, int idx_row, int idx_col);

TABLEAUptr tableau_minHeap_destro(TABLEAU T_young, int idx_row, int idx_col);

TABLEAUptr tableau_minHeap_padre(TABLEAU T_young, int idx_row, int idx_col);

void tableau_minHeap_buildHeap(TABLEAU T_young);

void tableau_minHeap_swap(TABLEAUptr T_ptr_el, TABLEAUptr T_ptr_min);


#endif