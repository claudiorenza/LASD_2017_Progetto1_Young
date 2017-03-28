#include "tableau.h"

//Interfaccia utente
int tableau_menu(int isEmpty)	{
	int choiceMenu;
	printf("\t\t\tTableau - MENÙ PRINCIPALE\n");
	printf("1. Genera Tableau\n");
	printf("2. Inserisci nuovo elemento\n");
    if(!isEmpty)    {   //se la Tableau esiste, mostro gli altri elementi del menù
        printf("3. Stampa Tableau\n");
        printf("4. Visualizza/Estrai elemento minimo\n");
        printf("5. Cancella elemento\n");
        printf("6. Cancella Tableau");
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

//Creo la struttura come Tableau
TABLEAU tableau_init()  {
    TABLEAU newTableau = NULL;
    if((newTableau = (int **)calloc(MAX_queue+1, sizeof(int *))))  {   //controllo di corretta allocazione dinamica di memoria
        if((newTableau[0] = (int *)malloc(sizeof(int))))   {   //alloco nell'indice 0...
            *(newTableau[0]) = 0;                              //il valore dell'heapSize
            for(int idx=1;idx<MAX_queue+1;idx++)    //pongo i restanti puntatori a NULL, che verranno poi allocati una volta inseriti nuovi elementi
                newTableau[idx] = NULL;
        } else
            printf("[MEM] ATTENZIONE: Problema di allocazione newTableau[0] heapSize - tableau_init\n");
    } else  //in caso di errori nell'allocazione
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU - tableau_init\n");
    return newTableau;
}

//Riempimento array e creazione dell'heap
void tableau_generate(TABLEAU T_young)   {
    int idx, n_elem;
    do  {
        printf("Quanti elementi vuoi inserire nell'array'? (1-%d): ", MAX_queue);
        if((n_elem = io_getInteger()) < 1 || n_elem > MAX_queue)
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(n_elem < 1 || n_elem > MAX_queue);
    for(idx=1;idx<=n_elem;idx++)   {
        if((T_young[idx] = (int *)malloc(sizeof(int)))){		//controllo di corretta allocazione dinamica di memoria
			*(T_young[idx]) = num_random(1, 256);  //generazione numero casuale da 1 a 256
            *(T_young[0]) += 1;   //incremento l'heapSize
		} else			
			printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUel - tableau_generate\n");
    }   
    talbeau_minHeap_buildHeap(T_young);  //costruisco l'heap da array dato
    //N.B.: in questo caso, usufruiamo dell'algoritmo buildHeap;
    //in alternativa è possibile aggiungere ogni chiave, mettendolo subito in ordine al momento dell'inserimento (come in 'tableau_insertKey')
    tableau_print(T_young);    //stampa della Tableau generata
}


//Inserimento nuovo elemento nella Tableau Heap con riordino automatico
void tableau_insertKey(TABLEAU T_young)  {
    int idx = *(T_young[0]) += 1;   //incremento l'heapSize e assegno l'indice per la ricostruzione dell'heap
    T_young[idx] = (int *)malloc(sizeof(int)); //alloco il puntatore nella nuova posizione
    
    printf("Quale valore vuoi inserire nella Tableau? ");
    *(T_young[idx]) = io_getInteger();   //posiziono la chiave nell'ultima posizione
    while(idx > 1 && *(T_young[talbeau_minHeap_padre(idx)]) > *(T_young[idx]))  {    //risalgo il sottoalbero per posizionare il valore nel posto giusto
        talbeau_minHeap_swap(T_young, idx, talbeau_minHeap_padre(idx));
        idx = talbeau_minHeap_padre(idx);
    }
    tableau_print(T_young);    //stampa della Tableau aggiornata
}

//Ricerca efficiente del valore nell'Heap (inOrder, senza la visita di valori più grandi di 'key')
int tableau_searchKey(TABLEAU T_young, int idx, int key)    {
    if(T_young[idx])   {   //se != NULL
        if(key >= *(T_young[idx]))  {   //controllo se la chiave è più grande del valore visitato nella Tableau
            int idx_ret = tableau_searchKey(T_young, talbeau_minHeap_sinistro(idx), key); //ricorsivamente scendo a sinistra
            if(idx_ret != 0)    //se l'ho già trovato
                return idx_ret;   //ritorno già l'indice ai R.A. precedenti;
            if(key == *(T_young[idx]))     //se ho trovato effettivamente il valore
                return idx;     //ritorno già il suo indice
            return tableau_searchKey(T_young, talbeau_minHeap_destro(idx), key);  //ricorsivamente scendo a destra
        }
    }
    return 0;   //o non trovo il valore, o eccede key, oppure trovo una foglia NULL
}

//Sostituzione del valore con l'ultimo elemento dello Heap
void tableau_overwrite(TABLEAU T_young, int idx)    {
    T_young[idx] = tableau_free_node(T_young[idx]);   //libero il puntatore in posizione 'idx'
    T_young[idx] = T_young[*(T_young[0])];     //assegno alla posizione 'idx' il puntatore all'ultimo elemento Heap
    *(T_young[0]) -= 1;   //diminuisco l'heapSize
}

//Eliminazione elemento 
void tableau_deleteKey(TABLEAU T_young) {
    int idx, idx_p; //idx_p è utile per la stampa dell'indice del valore da cancellare
    tableau_print(T_young);
    printf("Quale valore vuoi eliminare dalla Tableau? ");
    if((idx = idx_p = tableau_searchKey(T_young, 1, io_getInteger())) != 0)   {  //se trovato, restituisce l'indice del valore
        int val_del = *(T_young[idx]);     //salvo il valore cercato
        tableau_overwrite(T_young, idx);   //sovrascrivo il valore in posizione Tableau[idx] con l'ultimo elemento dello Heap
        if(*(T_young[*(T_young[0])]) < val_del)   {  //confronto il valore eliminato con il sostituito, utile al riordino in Heap
            while(idx > 1 && *(T_young[talbeau_minHeap_padre(idx)]) > *(T_young[idx]))  {    //riordino a salire dalla posizione attuale
                talbeau_minHeap_swap(T_young, idx, talbeau_minHeap_padre(idx));
                idx = talbeau_minHeap_padre(idx);
            }
        } else    {
            talbeau_minHeap_heapify(T_young, idx); //riordino a scendere dalla posizione attuale
        }
        tableau_print(T_young);    //stampa della Tableau aggiornata
        printf("Valore eliminato in Tableau[%d] = %d\n", idx_p, val_del);  //stampa dell'indice e del valore eliminato
    } else
        printf("ATTENZIONE: valore non presente\n");   
}

//Eliminazione dello Heap
void tableau_delete(TABLEAU T_young) {
    char choice;
    do  {
        printf("Sei sicuro di voler eliminare la Tableau? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: scelta non valida\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')
        T_young = tableau_free(T_young, 0);   //con '0' in parametro non elimino completamente l'array ma solo tutti i puntatori al suo interno
    printf("Tableau eliminata\n");
}


//Liberazione della memoria allocata dinamicamente
TABLEAU tableau_free(TABLEAU T_young, int del_complete)	{   //il parametro 'del_complete' == 1 libera memoria in fase di chiusura dell'applicazione
	for(int idx=1;idx<=*(T_young[0]);idx++) {      //ciclo solo fino a heapSize
        if(T_young[idx])       //controllo se effettivamente esiste un valore in 'idx'
            T_young[idx] = tableau_free_node(T_young[idx]);   //libero il vertice dall'array
    }
    if(del_complete) {  //se sto chiudendo l'applicazione
        free(T_young[0]);  //dealloco l'heapSize
	    free(T_young);     //dealloco la Tableau
        T_young = NULL;
    } else  //se l'utente decide di eliminare la Tableau
        *(T_young[0]) = 0; //resetto l'heapSize
	return T_young;
}

//Deallocazione del singolo vertice della Tableau con restituzione di NULL
TABLEAUel tableau_free_node(TABLEAUel T_young_el) {
    free(T_young_el);
    return NULL;
}


//Visualizzazione dell'elemento minimo
void tableau_min(TABLEAU T_young)  {
    printf("Valore minimo della Tableau: %d\n", *(T_young[1]));   //nell'Heap il valore minimo è situato alla radice, con accesso a tempo costante O(1)
    char choice;
    do  {
        printf("Desideri estrarlo? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: comando non riconosciuto\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')   {
        printf("Valore estratto: %d - Dimensione Heap: %d\n\n", tableau_extractMin(T_young), *(T_young[0]));  //estrazione/cancellazione della radice con visualizzazione del nuovo heapSize
        tableau_print(T_young);
    }
}


//Estrazione dell'elemento minimo
int tableau_extractMin(TABLEAU T_young) {
    int min = *(T_young[1]);
    tableau_overwrite(T_young, 1); //scambio con l'ultimo elemento dell'Heap
    talbeau_minHeap_heapify(T_young, 1);   //riordino in Heap dalla radice

    return min;
}


//Stampa della Tableau a priorità
void tableau_print(TABLEAU T_young)   {
    int idx;
    printf("\n");
    for(idx=1;idx<=*(T_young[0]);idx++)    
        printf("T_young[%d] = %d\n", idx, *(T_young[idx]));
    printf("\n");
}


int tableau_isEmpty(TABLEAU T_young) {
    if(*(T_young[0]) == 0)   
        return 1;
    return 0;
}
