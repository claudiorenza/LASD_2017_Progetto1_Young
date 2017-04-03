#ifndef TABLEAU_HEAP_H_INCLUDED
#define TABLEAU_HEAP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef int *** TABLEAU;
typedef int * TABLEAUptr;

void tableau_minHeap_heapify(TABLEAU T_young, int idx_row, int idx_col);

TABLEAUptr tableau_minHeap_sinistro(TABLEAU T_young, int idx_row, int idx_col);

TABLEAUptr tableau_minHeap_destro(TABLEAU T_young, int idx_row, int idx_col);

void tableau_minHeap_orderPadre(TABLEAU T_young, int idx_row, int idx_col);

TABLEAUptr tableau_minHeap_padre(TABLEAU T_young, int idx_row, int idx_col);

void tableau_minHeap_swap(TABLEAUptr T_ptr_el, TABLEAUptr T_ptr_min);


#endif