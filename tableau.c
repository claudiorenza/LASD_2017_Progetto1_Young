#include "tableau.h"

//Interfaccia utente
int tableau_menu(int isEmpty)	{
	int choiceMenu;
	printf("\t\t\tCoda a Priorità - MENÙ PRINCIPALE\n");
	printf("1. Genera coda di priorità\n");
	printf("2. Inserisci nuovo elemento\n");
    if(!isEmpty)    {   //se la coda esiste, mostro gli altri elementi del menù
        printf("3. Stampa coda di priorità\n");
        printf("4. Visualizza/Estrai elemento minimo\n");
        printf("5. Cancella elemento\n");
        printf("6. Cancella Coda di Priorità");
    }
    printf("\n");
    printf("7. Esci\n");
    printf("\n\n");
	do {
		printf("SCELTA: ");
		if(((choiceMenu = io_getInteger()) < 1 || choiceMenu > 7) || (isEmpty && (choiceMenu > 2 && choiceMenu < 7)))
			printf("ATTENZIONE: Valore non valido\n\n");
	}while((choiceMenu < 1 || choiceMenu > 7) || (isEmpty && (choiceMenu > 2 && choiceMenu < 7)));
    io_clearScreen();   //cancellazione dello std::out
	return choiceMenu;
}

//Creo la struttura come Coda a Priorità
TABLEAU tableau_init()  {
    TABLEAU newCoda = NULL;
    if((newCoda = (int **)calloc(MAX_queue+1, sizeof(int *))))  {   //controllo di corretta allocazione dinamica di memoria
        if((newCoda[0] = (int *)malloc(sizeof(int))))   {   //alloco nell'indice 0...
            *(newCoda[0]) = 0;                              //il valore dell'heapSize
            for(int idx=1;idx<MAX_queue+1;idx++)    //pongo i restanti puntatori a NULL, che verranno poi allocati una volta inseriti nuovi elementi
                newCoda[idx] = NULL;
        } else
            printf("[MEM] ATTENZIONE: Problema di allocazione newCoda[0] heapSize - tableau_init\n");
    } else  //in caso di errori nell'allocazione
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU - tableau_init\n");
    return newCoda;
}

//Riempimento array e creazione dell'heap
void tableau_generate(TABLEAU coda)   {
    int idx, n_elem;
    do  {
        printf("Quanti elementi vuoi inserire nell'array'? (1-%d): ", MAX_queue);
        if((n_elem = io_getInteger()) < 1 || n_elem > MAX_queue)
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(n_elem < 1 || n_elem > MAX_queue);
    for(idx=1;idx<=n_elem;idx++)   {
        if((coda[idx] = (int *)malloc(sizeof(int)))){		//controllo di corretta allocazione dinamica di memoria
			*(coda[idx]) = num_random(1, 256);  //generazione numero casuale da 1 a 256
            *(coda[0]) += 1;   //incremento l'heapSize
		} else			
			printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUel - tableau_generate\n");
    }   
    talbleau_minHeap_buildHeap(coda);  //costruisco l'heap da array dato
    //N.B.: in questo caso, usufruiamo dell'algoritmo buildHeap;
    //in alternativa è possibile aggiungere ogni chiave, mettendolo subito in ordine al momento dell'inserimento (come in 'tableau_insertKey')
    tableau_print(coda);    //stampa della Coda generata
}


//Inserimento nuovo elemento nella Coda Heap con riordino automatico
void tableau_insertKey(TABLEAU coda)  {
    int idx = *(coda[0]) += 1;   //incremento l'heapSize e assegno l'indice per la ricostruzione dell'heap
    coda[idx] = (int *)malloc(sizeof(int)); //alloco il puntatore nella nuova posizione
    
    printf("Quale valore vuoi inserire nella coda? ");
    *(coda[idx]) = io_getInteger();   //posiziono la chiave nell'ultima posizione
    while(idx > 1 && *(coda[talbleau_minHeap_padre(idx)]) > *(coda[idx]))  {    //risalgo il sottoalbero per posizionare il valore nel posto giusto
        talbleau_minHeap_swap(coda, idx, talbleau_minHeap_padre(idx));
        idx = talbleau_minHeap_padre(idx);
    }
    tableau_print(coda);    //stampa della Coda aggiornata
}

//Ricerca efficiente del valore nell'Heap (inOrder, senza la visita di valori più grandi di 'key')
int tableau_searchKey(TABLEAU coda, int idx, int key)    {
    if(coda[idx])   {   //se != NULL
        if(key >= *(coda[idx]))  {   //controllo se la chiave è più grande del valore visitato nella Coda
            int idx_ret = tableau_searchKey(coda, talbleau_minHeap_sinistro(idx), key); //ricorsivamente scendo a sinistra
            if(idx_ret != 0)    //se l'ho già trovato
                return idx_ret;   //ritorno già l'indice ai R.A. precedenti;
            if(key == *(coda[idx]))     //se ho trovato effettivamente il valore
                return idx;     //ritorno già il suo indice
            return tableau_searchKey(coda, talbleau_minHeap_destro(idx), key);  //ricorsivamente scendo a destra
        }
    }
    return 0;   //o non trovo il valore, o eccede key, oppure trovo una foglia NULL
}

//Sostituzione del valore con l'ultimo elemento dello Heap
void tableau_overwrite(TABLEAU coda, int idx)    {
    coda[idx] = tableau_free_node(coda[idx]);   //libero il puntatore in posizione 'idx'
    coda[idx] = coda[*(coda[0])];     //assegno alla posizione 'idx' il puntatore all'ultimo elemento Heap
    *(coda[0]) -= 1;   //diminuisco l'heapSize
}

//Eliminazione elemento 
void tableau_deleteKey(TABLEAU coda) {
    int idx, idx_p; //idx_p è utile per la stampa dell'indice del valore da cancellare
    tableau_print(coda);
    printf("Quale valore vuoi eliminare dalla coda? ");
    if((idx = idx_p = tableau_searchKey(coda, 1, io_getInteger())) != 0)   {  //se trovato, restituisce l'indice del valore
        int val_del = *(coda[idx]);     //salvo il valore cercato
        tableau_overwrite(coda, idx);   //sovrascrivo il valore in posizione coda[idx] con l'ultimo elemento dello Heap
        if(*(coda[*(coda[0])]) < val_del)   {  //confronto il valore eliminato con il sostituito, utile al riordino in Heap
            while(idx > 1 && *(coda[talbleau_minHeap_padre(idx)]) > *(coda[idx]))  {    //riordino a salire dalla posizione attuale
                talbleau_minHeap_swap(coda, idx, talbleau_minHeap_padre(idx));
                idx = talbleau_minHeap_padre(idx);
            }
        } else    {
            talbleau_minHeap_heapify(coda, idx); //riordino a scendere dalla posizione attuale
        }
        tableau_print(coda);    //stampa della Coda aggiornata
        printf("Valore eliminato in coda[%d] = %d\n", idx_p, val_del);  //stampa dell'indice e del valore eliminato
    } else
        printf("ATTENZIONE: valore non presente\n");   
}

//Eliminazione dello Heap
void tableau_delete(TABLEAU coda) {
    char choice;
    do  {
        printf("Sei sicuro di voler eliminare la coda? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: scelta non valida\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')
        coda = tableau_free(coda, 0);   //con '0' in parametro non elimino completamente l'array ma solo tutti i puntatori al suo interno
    printf("Coda eliminata\n");
}


//Liberazione della memoria allocata dinamicamente
TABLEAU tableau_free(TABLEAU coda, int del_complete)	{   //il parametro 'del_complete' == 1 libera memoria in fase di chiusura dell'applicazione
	for(int idx=1;idx<=*(coda[0]);idx++) {      //ciclo solo fino a heapSize
        if(coda[idx])       //controllo se effettivamente esiste un valore in 'idx'
            coda[idx] = tableau_free_node(coda[idx]);   //libero il vertice dall'array
    }
    if(del_complete) {  //se sto chiudendo l'applicazione
        free(coda[0]);  //dealloco l'heapSize
	    free(coda);     //dealloco la Coda
        coda = NULL;
    } else  //se l'utente decide di eliminare la Coda
        *(coda[0]) = 0; //resetto l'heapSize
	return coda;
}

//Deallocazione del singolo vertice della Coda con restituzione di NULL
TABLEAUel tableau_free_node(TABLEAUel coda_el) {
    free(coda_el);
    return NULL;
}


//Visualizzazione dell'elemento minimo
void tableau_min(TABLEAU coda)  {
    printf("Valore minimo della coda: %d\n", *(coda[1]));   //nell'Heap il valore minimo è situato alla radice, con accesso a tempo costante O(1)
    char choice;
    do  {
        printf("Desideri estrarlo? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: comando non riconosciuto\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')   {
        printf("Valore estratto: %d - Dimensione Heap: %d\n\n", tableau_extractMin(coda), *(coda[0]));  //estrazione/cancellazione della radice con visualizzazione del nuovo heapSize
        tableau_print(coda);
    }
}


//Estrazione dell'elemento minimo
int tableau_extractMin(TABLEAU coda) {
    int min = *(coda[1]);
    tableau_overwrite(coda, 1); //scambio con l'ultimo elemento dell'Heap
    talbleau_minHeap_heapify(coda, 1);   //riordino in Heap dalla radice

    return min;
}


//Stampa della coda a priorità
void tableau_print(TABLEAU coda)   {
    int idx;
    printf("\n");
    for(idx=1;idx<=*(coda[0]);idx++)    
        printf("coda[%d] = %d\n", idx, *(coda[idx]));
    printf("\n");
}


int tableau_isEmpty(TABLEAU coda) {
    if(*(coda[0]) == 0)   
        return 1;
    return 0;
}
