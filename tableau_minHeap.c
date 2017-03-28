#include "talbleau_minHeap.h"

//Heapify
void talbleau_minHeap_heapify(TABLEAU T_young, int idx)	{
	int idx_l, idx_r, idx_min;
	idx_l = talbleau_minHeap_sinistro(idx);
	idx_r = talbleau_minHeap_destro(idx);

	if (idx_l <= *(T_young[0]) && *(T_young[idx_l]) < *(T_young[idx]))
		idx_min = idx_l;
	else
		idx_min = idx;

	if(idx_r <= *(T_young[0]) && *(T_young[idx_r]) < *(T_young[idx_min]))
		idx_min = idx_r;

	if(idx_min != idx)	{
		talbleau_minHeap_swap(T_young, idx, idx_min);
		talbleau_minHeap_heapify(T_young, idx_min);
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
void talbleau_minHeap_buildHeap(TABLEAU T_young)	{
	int idx;
	for(idx=*(T_young[0])/2; idx>0; idx--)
		talbleau_minHeap_heapify(T_young, idx);
}


//Swap dei riferimenti
void talbleau_minHeap_swap(TABLEAU T_young, int idx_1, int idx_2) {
    int *tmp = T_young[idx_2];
    T_young[idx_2] = T_young[idx_1];
    T_young[idx_1] = tmp;
}
