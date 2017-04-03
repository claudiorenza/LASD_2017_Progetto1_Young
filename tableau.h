#ifndef TABLEAU_H_INCLUDED
#define TABLEAU_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "io.h"
#include "num.h"
#include "tableau_minHeap.h"

#define MAX_matrix 24

typedef int *** TABLEAU;
typedef int * TABLEAUptr;


int tableau_menu();

TABLEAU tableau_init();

void tableau_setLimits(TABLEAU T_young);

void tableau_generate(TABLEAU T_young);

void tableau_insertKey(TABLEAU T_young, int random);

void tableau_insertKey_setLast(TABLEAU T_young);

TABLEAUptr tableau_searchKey(TABLEAU T_young, int *p_idx_row, int *p_idx_col, int key);

void tableau_overwrite(TABLEAU T_young, int idx_row, int idx_col);

void tableau_overwrite_setLast(TABLEAU T_young);

void tableau_deleteKey(TABLEAU T_young);

void tableau_delete(TABLEAU T_young);

TABLEAU tableau_free(TABLEAU T_young, int del_complete);

TABLEAUptr tableau_free_node(TABLEAUptr T_young_el);

void tableau_min(TABLEAU T_young);

int tableau_extractMin(TABLEAU T_young);

void tableau_print(TABLEAU T_young);

int tableau_isEmpty(TABLEAU T_young);

int tableau_isFull(TABLEAU T_young);


#endif