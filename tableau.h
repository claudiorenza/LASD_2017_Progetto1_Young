#ifndef TABLEAU_H_INCLUDED
#define TABLEAU_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include "io.h"
#include "num.h"
#include "tableau_minHeap.h"

#define MAX_matrix 256

typedef int *** TABLEAU;
typedef int ** TABLEAUptr;
typedef int * TABLEAUval;

int tableau_menu();

TABLEAU tableau_init();

void tableau_generate(TABLEAU T_young);

void tableau_insertKey(TABLEAU T_young);

int tableau_searchKey(TABLEAU T_young, int idx, int key);

void tableau_overwrite(TABLEAU T_young, int idx);

void tableau_deleteKey(TABLEAU T_young);

void tableau_delete(TABLEAU T_young);

TABLEAU tableau_free(TABLEAU T_young, int del_complete);

TABLEAUval tableau_free_node(TABLEAUval T_young_el);

void tableau_min(TABLEAU T_young);

int tableau_extractMin(TABLEAU T_young);

void tableau_print(TABLEAU T_young);

int tableau_isEmpty(TABLEAU T_young);

int tableau_isFull(TABLEAU T_young)


#endif