#ifndef TABLEAU_H_INCLUDED
#define TABLEAU_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include "io.h"
#include "num.h"
#include "talbleau_minHeap.h"

#define MAX_queue 256

typedef int ** TABLEAU;
typedef int * TABLEAUel;

int tableau_menu();

TABLEAU tableau_init();

void tableau_generate(TABLEAU coda);

void tableau_insertKey(TABLEAU coda);

int tableau_searchKey(TABLEAU coda, int idx, int key);

void tableau_overwrite(TABLEAU coda, int idx);

void tableau_deleteKey(TABLEAU coda);

void tableau_delete(TABLEAU coda);

TABLEAU tableau_free(TABLEAU coda, int del_complete);

TABLEAUel tableau_free_node(TABLEAUel coda_el);

void tableau_min(TABLEAU coda);

int tableau_extractMin(TABLEAU coda);

void tableau_print(TABLEAU coda);

int tableau_isEmpty(TABLEAU coda);


#endif