#include "talbleau_minHeap.h"

//Heapify
void talbleau_minHeap_heapify(TABLEAU coda, int idx)	{
	int idx_l, idx_r, idx_min;
	idx_l = talbleau_minHeap_sinistro(idx);
	idx_r = talbleau_minHeap_destro(idx);

	if (idx_l <= *(coda[0]) && *(coda[idx_l]) < *(coda[idx]))
		idx_min = idx_l;
	else
		idx_min = idx;

	if(idx_r <= *(coda[0]) && *(coda[idx_r]) < *(coda[idx_min]))
		idx_min = idx_r;

	if(idx_min != idx)	{
		talbleau_minHeap_swap(coda, idx, idx_min);
		talbleau_minHeap_heapify(coda, idx_min);
	}
}


//Ricavo l'indice del figlio sinistro
int talbleau_minHeap_sinistro(int idx)	{
	return 2*idx;
}


//Ricavo l'indice del il figlio destro
int talbleau_minHeap_destro(int idx)	{
	return 2*idx+1;
}


//Ricavo l'indice del padre
int talbleau_minHeap_padre(int idx)	{
	return idx/2;
}


//Creazione dello Heap
void talbleau_minHeap_buildHeap(TABLEAU coda)	{
	int idx;
	for(idx=*(coda[0])/2; idx>0; idx--)
		talbleau_minHeap_heapify(coda, idx);
}


//Swap dei riferimenti
void talbleau_minHeap_swap(TABLEAU coda, int idx_1, int idx_2) {
    int *tmp = coda[idx_2];
    coda[idx_2] = coda[idx_1];
    coda[idx_1] = tmp;
}
