#ifndef PRQUEUE_H_INCLUDED
#define PRQUEUE_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include "io.h"
#include "num.h"
#include "matrix_minHeap.h"

#define MAX_queue 256

typedef int ** PRQUEUE;
typedef int * PRQUEUEel;

int prQueue_menu();

PRQUEUE prQueue_init();

void prQueue_generate(PRQUEUE coda);

void prQueue_insertKey(PRQUEUE coda);

int prQueue_searchKey(PRQUEUE coda, int idx, int key);

void prQueue_overwrite(PRQUEUE coda, int idx);

void prQueue_deleteKey(PRQUEUE coda);

void prQueue_delete(PRQUEUE coda);

PRQUEUE prQueue_free(PRQUEUE coda, int del_complete);

PRQUEUEel prQueue_free_node(PRQUEUEel coda_el);

void prQueue_min(PRQUEUE coda);

int prQueue_extractMin(PRQUEUE coda);

void prQueue_print(PRQUEUE coda);

int prQueue_isEmpty(PRQUEUE coda);


#endif