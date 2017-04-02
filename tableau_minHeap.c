#include "tableau_minHeap.h"
//ciao
//Heapify
void tableau_minHeap_heapify(TABLEAU T_young, int idx_row, int idx_col)	{
	int idx_l, idx_r
	TABLEAUptr T_ptr_min;	//riferimento al puntatore col valore più piccolo
	TABLEAUptr T_ptr_left = tableau_minHeap_sinistro(T_young, idx_row, idx_col);	//indice di riga sul valore a sinistra
	TABLEAUptr T_ptr_right = tableau_minHeap_destro(T_young, idx_row, idx_col);	//indice di colonna sul valore a destra

	//in base all'ordinamento Heap, determino quale valore ho bisogno di sostituire nei vertici figli
	if (T_ptr_left && *T_ptr_left < *(T_young[idx_row][idx_col]))	//confronto dei valori fra posizione attuale e figlio sinistro
		T_ptr_min = T_ptr_left;
	else
		T_ptr_min = T_young[idx_row][idx_col];	//posizione attuale

	if(T_ptr_right && *(T_young[idx_row][idx_right]) < *(T_ptr_min))	//confronto dei valori fra posizione attuale e figlio destro
		T_ptr_min = T_ptr_right;

	if(T_ptr_min != T_young[idx_row][idx_col])	{	//se effettivamente ho un valore da scambiare
		int tmp = *T_young[idx_row][idx_col];	//mi conservo momentaneamente il valore nella posizione attuale
		tableau_minHeap_swap(T_young[idx_row][idx_col], T_ptr_min);	//effetto lo scambio fra i valori
		if(T_ptr_left && tmp == *T_ptr_left)	//se il valore conservato è stato posizionato a sinistra/riga successiva
			tableau_minHeap_heapify(T_young, idx_row+1, idx_col);	//continuo heapify nella riga successiva
		else if (T_ptr_right && tmp == *T_ptr_right)	//se il valore conservato è stato posizionato a destra/colonna successiva
			tableau_minHeap_heapify(T_young, idx_row, idx_col+1);	//continuo heapify nella colonna successiva
	}
}


//Ricavo l'indice del figlio sinistro (riga successiva)
TABLEAUptr tableau_minHeap_sinistro(TABLEAU T_young, int idx_row, int idx_col)	{
	if(idx_row+1 <= *(T_young[1][0]))	//se non eccedo col numero di righe
		return T_young[idx_row+1][idx_col];	//ritorno il puntatore al figlio sinistro
	return NULL;	//altrimenti ritorno un puntatore vuoto
}


//Ricavo l'indice del il figlio destro (colonna successiva)
TABLEAUptr tableau_minHeap_destro(TABLEAU T_young, int idx_row, int idx_col)	{
	if(idx_col+1 <= *(T_young[0][1]))	//se non eccedo col numero di righe
		return T_young[idx_row][idx_col+1];	//ritrono il puntatore al figlio destro
	return NULL;	//altrimenti ritorno un puntatore vuoto
}

//Riordino i valori a salire dalla posizione attuale
void tableau_minHeap_orderPadre(TABLEAU T_young, int idx_row, int idx_col)	{
	TABLEAUptr curr = T_young[idx_row][idx_col];
	while(idx_row > 1 && idx_col > 1 && *(tableau_minHeap_padre(T_young, idx_row, idx_col)) > *(curr))  {    
		tableau_minHeap_swap(curr, tableau_minHeap_padre(T_young, idx_row, idx_col));
		curr = tableau_minHeap_padre(T_young, idx_row, idx_col);
	}
}

//Indico fra i due padri chi ha il valore più grande (mi assicuro di mantenere una proprietà transitiva fra i valori da confrontare)
TABLEAUptr tableau_minHeap_padre(TABLEAU T_young, int idx_row, int idx_col)	{
	if(*(T_young[idx_row-1][idx_col]) > *(T_young[idx_row][idx_col-1]))
		return T_young[idx_row-1][idx_col];
	else
		return T_young[idx_row][idx_col-1];
}

//Swap dei valori nei riferimenti
void tableau_minHeap_swap(TABLEAUptr T_ptr_el, TABLEAUptr T_ptr_min) {
    int tmp = *T_ptr_el;
    *T_ptr_el = *T_ptr_min;
    *T_ptr_min = tmp;
}
