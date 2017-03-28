#include "prQueue.h"

//Interfaccia utente
int prQueue_menu(int isEmpty)	{
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
PRQUEUE prQueue_init()  {
    PRQUEUE newCoda = NULL;
    if((newCoda = (int **)calloc(MAX_queue+1, sizeof(int *))))  {   //controllo di corretta allocazione dinamica di memoria
        if((newCoda[0] = (int *)malloc(sizeof(int))))   {   //alloco nell'indice 0...
            *(newCoda[0]) = 0;                              //il valore dell'heapSize
            for(int idx=1;idx<MAX_queue+1;idx++)    //pongo i restanti puntatori a NULL, che verranno poi allocati una volta inseriti nuovi elementi
                newCoda[idx] = NULL;
        } else
            printf("[MEM] ATTENZIONE: Problema di allocazione newCoda[0] heapSize - prQueue_init\n");
    } else  //in caso di errori nell'allocazione
        printf("[MEM] ATTENZIONE: Problema di allocazione PRQUEUE - prQueue_init\n");
    return newCoda;
}

//Riempimento array e creazione dell'heap
void prQueue_generate(PRQUEUE coda)   {
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
			printf("[MEM] ATTENZIONE: Problema di allocazione PRQUEUEel - prQueue_generate\n");
    }   
    matrix_minHeap_buildHeap(coda);  //costruisco l'heap da array dato
    //N.B.: in questo caso, usufruiamo dell'algoritmo buildHeap;
    //in alternativa è possibile aggiungere ogni chiave, mettendolo subito in ordine al momento dell'inserimento (come in 'prQueue_insertKey')
    prQueue_print(coda);    //stampa della Coda generata
}


//Inserimento nuovo elemento nella Coda Heap con riordino automatico
void prQueue_insertKey(PRQUEUE coda)  {
    int idx = *(coda[0]) += 1;   //incremento l'heapSize e assegno l'indice per la ricostruzione dell'heap
    coda[idx] = (int *)malloc(sizeof(int)); //alloco il puntatore nella nuova posizione
    
    printf("Quale valore vuoi inserire nella coda? ");
    *(coda[idx]) = io_getInteger();   //posiziono la chiave nell'ultima posizione
    while(idx > 1 && *(coda[matrix_minHeap_padre(idx)]) > *(coda[idx]))  {    //risalgo il sottoalbero per posizionare il valore nel posto giusto
        matrix_minHeap_swap(coda, idx, matrix_minHeap_padre(idx));
        idx = matrix_minHeap_padre(idx);
    }
    prQueue_print(coda);    //stampa della Coda aggiornata
}

//Ricerca efficiente del valore nell'Heap (inOrder, senza la visita di valori più grandi di 'key')
int prQueue_searchKey(PRQUEUE coda, int idx, int key)    {
    if(coda[idx])   {   //se != NULL
        if(key >= *(coda[idx]))  {   //controllo se la chiave è più grande del valore visitato nella Coda
            int idx_ret = prQueue_searchKey(coda, matrix_minHeap_sinistro(idx), key); //ricorsivamente scendo a sinistra
            if(idx_ret != 0)    //se l'ho già trovato
                return idx_ret;   //ritorno già l'indice ai R.A. precedenti;
            if(key == *(coda[idx]))     //se ho trovato effettivamente il valore
                return idx;     //ritorno già il suo indice
            return prQueue_searchKey(coda, matrix_minHeap_destro(idx), key);  //ricorsivamente scendo a destra
        }
    }
    return 0;   //o non trovo il valore, o eccede key, oppure trovo una foglia NULL
}

//Sostituzione del valore con l'ultimo elemento dello Heap
void prQueue_overwrite(PRQUEUE coda, int idx)    {
    coda[idx] = prQueue_free_node(coda[idx]);   //libero il puntatore in posizione 'idx'
    coda[idx] = coda[*(coda[0])];     //assegno alla posizione 'idx' il puntatore all'ultimo elemento Heap
    *(coda[0]) -= 1;   //diminuisco l'heapSize
}

//Eliminazione elemento 
void prQueue_deleteKey(PRQUEUE coda) {
    int idx, idx_p; //idx_p è utile per la stampa dell'indice del valore da cancellare
    prQueue_print(coda);
    printf("Quale valore vuoi eliminare dalla coda? ");
    if((idx = idx_p = prQueue_searchKey(coda, 1, io_getInteger())) != 0)   {  //se trovato, restituisce l'indice del valore
        int val_del = *(coda[idx]);     //salvo il valore cercato
        prQueue_overwrite(coda, idx);   //sovrascrivo il valore in posizione coda[idx] con l'ultimo elemento dello Heap
        if(*(coda[*(coda[0])]) < val_del)   {  //confronto il valore eliminato con il sostituito, utile al riordino in Heap
            while(idx > 1 && *(coda[matrix_minHeap_padre(idx)]) > *(coda[idx]))  {    //riordino a salire dalla posizione attuale
                matrix_minHeap_swap(coda, idx, matrix_minHeap_padre(idx));
                idx = matrix_minHeap_padre(idx);
            }
        } else    {
            matrix_minHeap_heapify(coda, idx); //riordino a scendere dalla posizione attuale
        }
        prQueue_print(coda);    //stampa della Coda aggiornata
        printf("Valore eliminato in coda[%d] = %d\n", idx_p, val_del);  //stampa dell'indice e del valore eliminato
    } else
        printf("ATTENZIONE: valore non presente\n");   
}

//Eliminazione dello Heap
void prQueue_delete(PRQUEUE coda) {
    char choice;
    do  {
        printf("Sei sicuro di voler eliminare la coda? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: scelta non valida\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')
        coda = prQueue_free(coda, 0);   //con '0' in parametro non elimino completamente l'array ma solo tutti i puntatori al suo interno
    printf("Coda eliminata\n");
}


//Liberazione della memoria allocata dinamicamente
PRQUEUE prQueue_free(PRQUEUE coda, int del_complete)	{   //il parametro 'del_complete' == 1 libera memoria in fase di chiusura dell'applicazione
	for(int idx=1;idx<=*(coda[0]);idx++) {      //ciclo solo fino a heapSize
        if(coda[idx])       //controllo se effettivamente esiste un valore in 'idx'
            coda[idx] = prQueue_free_node(coda[idx]);   //libero il vertice dall'array
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
PRQUEUEel prQueue_free_node(PRQUEUEel coda_el) {
    free(coda_el);
    return NULL;
}


//Visualizzazione dell'elemento minimo
void prQueue_min(PRQUEUE coda)  {
    printf("Valore minimo della coda: %d\n", *(coda[1]));   //nell'Heap il valore minimo è situato alla radice, con accesso a tempo costante O(1)
    char choice;
    do  {
        printf("Desideri estrarlo? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: comando non riconosciuto\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')   {
        printf("Valore estratto: %d - Dimensione Heap: %d\n\n", prQueue_extractMin(coda), *(coda[0]));  //estrazione/cancellazione della radice con visualizzazione del nuovo heapSize
        prQueue_print(coda);
    }
}


//Estrazione dell'elemento minimo
int prQueue_extractMin(PRQUEUE coda) {
    int min = *(coda[1]);
    prQueue_overwrite(coda, 1); //scambio con l'ultimo elemento dell'Heap
    matrix_minHeap_heapify(coda, 1);   //riordino in Heap dalla radice

    return min;
}


//Stampa della coda a priorità
void prQueue_print(PRQUEUE coda)   {
    int idx;
    printf("\n");
    for(idx=1;idx<=*(coda[0]);idx++)    
        printf("coda[%d] = %d\n", idx, *(coda[idx]));
    printf("\n");
}


int prQueue_isEmpty(PRQUEUE coda) {
    if(*(coda[0]) == 0)   
        return 1;
    return 0;
}
