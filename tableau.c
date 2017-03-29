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
    if((newTableau = (int ***)calloc(MAX_matrix+1, sizeof(int **))))  {   //controllo di corretta allocazione dinamica di memoria
        for(int idx_row=0;idx_row<MAX_matrix+1;idx_row++)   {
            if((newTableau[idx_row] = (int **)calloc(MAX_matrix+1, sizeof(int *)))) { //allocazione delle colonne per la riga attuale
                for(int idx_col=0;idx_col<MAX_matrix+1;idx_col++)    //pongo i puntatori a NULL, che verranno poi allocati una volta inseriti nuovi elementi
                        newTableau[idx_row][idx_col] = NULL;
            } else  {
                printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU colonne - tableau_init\n");
                free(newTableau);   //pulisco la memoria appena allocata
                return NULL;
            }
        }
        if((newTableau[0][0] = (int *)malloc(sizeof(int))))      //alloco negli indici...
            *(newTableau[0][0]) = 0;            //il valore dell'heapSize/numero degli elementi
        else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU elements - tableau_init\n");
            //[IMPL] funzione di deallocazione completa della matrice
            return NULL;
        }
    } else {//in caso di errori nell'allocazione
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU righe - tableau_init\n");
        return NULL;
    }
    return newTableau;
}

//Riempimento matrice e creazione dell'Heap
void tableau_generate(TABLEAU T_young)   {
    int idx_row, idx_col, n_elem, 
    int idx_i, idx_j;   //indici di riga e colonna provvisori
    do  {
        printf("Quante righe vuoi nella Tableau? (1-%d): ", MAX_matrix);
        if((idx_row = io_getInteger()) < 1 || idx_row > MAX_matrix)
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(idx_row < 1 || idx_row > MAX_matrix);
    
    do  {
        printf("Quante colonne vuoi nella Tableau? (1-%d): ", MAX_matrix);
        if((idx_col = io_getInteger()) < 1 || idx_col > MAX_matrix)
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(idx_col < 1 || idx_col > MAX_matrix);
    
    if((newTableau[1][0] = (int *)malloc(sizeof(int))) && (newTableau[0][1] = (int *)malloc(sizeof(int))))  {
        *(newTableau[1][0]) = idx_row;    //Pongo l'indice massimo di riga
        *(newTableau[0][1]) = idx_col;    //e l'indice massimo di colonna
    } else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU elements - tableau_init\n");
            //[IMPL] funzione di deallocazione completa della matrice
            return NULL;
    }

    do  {
        printf("Quanti elementi vuoi inserire nella Tableau? (1-%d): ", idx_row*idx_col);
        if((n_elem = io_getInteger()) < 1 || n_elem > idx_row*idx_col)  //si possono inserire in input idx_row*idx_col elementi
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(n_elem < 1 || n_elem > idx_row*idx_col);

    //Riempimento triangolare superiore
    for(idx_row=1;idx_row<=*(T_young[1][0]) && *(T_young[0][0]) <= n_elem;idx_row++)    { //se ho raggiunto il numero di elementi richiesto, fermo il ciclo esterno
        idx_i = idx_row;  //assegno all'indice di riga da decrementare il valore di quello fissato in questo ciclo
        for(idx_j=1;idx_j<=idx_row && idx_j<=*(T_young[0][1]) && *(T_young[0][0])<=n_elem;idx_j++)   { //se ho raggiunto il numero di elementi richiesto o l'indice di colonna massimo, fermo il ciclo interno
            //N.B.: 'idx_j<=*(T_young[0][1])' è utile nel caso in cui il numero di righe 'idx_row' possono essere maggiori delle colonne
            if((T_young[idx_i][idx_j] = (int *)malloc(sizeof(int)))){		//controllo di corretta allocazione dinamica di memoria
                *(T_young[idx_i][idx_j]) = num_random(1, 256);  //generazione numero casuale da 1 a 256
                *(T_young[0][0]) += 1;   //incremento l'heapSize/numero di elementi
                idx_i--;    //l'indice di riga provvisoria diminuisce mentre l'indice di colonna aumenta
            } else  {
                printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUval - tableau_generate\n");
                return;
            }
        }
    }

    //Eventuale riempimento triangolare inferiore
    if(idx_row > *(T_young[1][0]) && *(T_young[0][0]) < n_elem) {    //nel caso debba inserire ancora altri elementi
        idx_row -= 1; //decremento per tornare al giusto indice di ultima riga [== *(T_young[1][0])]
        for(idx_col=2;idx_col<=*(T_young[0][1]) && *(T_young[0][0]) <= n_elem;idx_col++)    { //se ho raggiunto il numero di elementi richiesto, fermo il ciclo esterno
            idx_j = idx_col;  //assegno all'indice di colonna da incrementare il valore di quello fissato in questo ciclo
            for(idx_i=*(T_young[1][0]);idx_i>=1 && idx_j<=*(T_young[0][1]) && *(T_young[0][0])<=n_elem;idx_i--)   { //se ho raggiunto il numero di elementi richiesto o l'indice di colonna massimo, fermo il ciclo interno
                //N.B.: 'idx_i<=*(T_young[1][0])' è utile nel caso in cui il numero di righe 'idx_row' possono essere maggiori delle colonne
                if((T_young[idx_i][idx_j] = (int *)malloc(sizeof(int)))){		//controllo di corretta allocazione dinamica di memoria
                    *(T_young[idx_i][idx_j]) = num_random(1, 256);  //generazione numero casuale da 1 a 256
                    *(T_young[0][0]) += 1;   //incremento l'heapSize/numero di elementi
                    idx_j++;    //l'indice di colonna provvisoria aumenta mentre l'indice di riga diminuisce
                } else  {
                    printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUval - tableau_generate\n");
                    return;
                }
            }
        }
    }   

    //tableau_minHeap_buildHeap(T_young);  //costruisco l'heap da matrice data
    //N.B.: in questo caso, usufruiamo dell'algoritmo buildHeap;
    //in alternativa è possibile aggiungere ogni chiave, mettendolo subito in ordine al momento dell'inserimento (come in 'tableau_insertKey')
    tableau_print(T_young);    //stampa della Tableau generata
}


//Inserimento nuovo elemento nella Tableau Heap con riordino automatico
void tableau_insertKey(TABLEAU T_young)  {
    *(T_young[0][0]) += 1;   //incremento l'heapSize e assegno l'indice per la ricostruzione dell'heap
    int *new_elem = (int *)malloc(sizeof(int)); //alloco il puntatore nella nuova posizione
    
    printf("Quale valore vuoi inserire nella Tableau? ");
    *new_elem = io_getInteger();   //posiziono la chiave nell'ultima posizione
    
    //heapify con nuovo elemento

    tableau_print(T_young);    //stampa della Tableau aggiornata
}

//Ricerca efficiente del valore nell'Heap (inOrder, senza la visita di valori più grandi di 'key')
int tableau_searchKey(TABLEAU T_young, int idx, int key)    {
    if(T_young[idx_row][idx_col])   {   //se != NULL
        if(key >= *(T_young[idx_row][idx_col]))  {   //controllo se la chiave è più grande del valore visitato nella Tableau
            int idx_ret = tableau_searchKey(T_young, tableau_minHeap_sinistro(idx), key); //ricorsivamente scendo a sinistra
            if(idx_ret != 0)    //se l'ho già trovato
                return idx_ret;   //ritorno già l'indice ai R.A. precedenti;
            if(key == *(T_young[idx_row][idx_col]))     //se ho trovato effettivamente il valore
                return idx;     //ritorno già il suo indice
            return tableau_searchKey(T_young, tableau_minHeap_destro(idx), key);  //ricorsivamente scendo a destra
        }
    }
    return 0;   //o non trovo il valore, o eccede key, oppure trovo una foglia NULL
}

//Sostituzione del valore con l'ultimo elemento dello Heap
void tableau_overwrite(TABLEAU T_young, int idx)    {
    T_young[idx_row][idx_col] = tableau_free_node(T_young[idx_row][idx_col]);   //libero il puntatore in posizione 'idx'
    T_young[idx_row][idx_col] = T_young[*(T_young[0])];     //assegno alla posizione 'idx' il puntatore all'ultimo elemento Heap
    *(T_young[0]) -= 1;   //diminuisco l'heapSize
}

//Eliminazione elemento 
void tableau_deleteKey(TABLEAU T_young) {
    int idx, idx_p; //idx_p è utile per la stampa dell'indice del valore da cancellare
    tableau_print(T_young);
    printf("Quale valore vuoi eliminare dalla Tableau? ");
    if((idx = idx_p = tableau_searchKey(T_young, 1, io_getInteger())) != 0)   {  //se trovato, restituisce l'indice del valore
        int val_del = *(T_young[idx_row][idx_col]);     //salvo il valore cercato
        tableau_overwrite(T_young, idx);   //sovrascrivo il valore in posizione Tableau[idx_row][idx_col] con l'ultimo elemento dello Heap
        if(*(T_young[*(T_young[0])]) < val_del)   {  //confronto il valore eliminato con il sostituito, utile al riordino in Heap
            while(idx_row > 1 && idx_col > 1 && *(T_young[tableau_minHeap_padre(idx)]) > *(T_young[idx_row][idx_col]))  {    //riordino a salire dalla posizione attuale
                tableau_minHeap_swap(T_young, idx, tableau_minHeap_padre(idx));
                idx = tableau_minHeap_padre(idx);
            }
        } else    {
            tableau_minHeap_heapify(T_young, idx); //riordino a scendere dalla posizione attuale
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
        T_young = tableau_free(T_young, 0);   //con '0' in parametro non elimino completamente l'matrice ma solo tutti i puntatori al suo interno
    printf("Tableau eliminata\n");
}


//Liberazione della memoria allocata dinamicamente
TABLEAU tableau_free(TABLEAU T_young, int del_complete)	{   //il parametro 'del_complete' == 1 libera memoria in fase di chiusura dell'applicazione
	for(int idx=1;idx<=*(T_young[0]);idx++) {      //ciclo solo fino a heapSize
        if(T_young[idx_row][idx_col])       //controllo se effettivamente esiste un valore in 'idx'
            T_young[idx_row][idx_col] = tableau_free_node(T_young[idx_row][idx_col]);   //libero il vertice dall'matrice
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
TABLEAUval tableau_free_node(TABLEAUval T_young_el) {
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
    tableau_minHeap_heapify(T_young, 1);   //riordino in Heap dalla radice

    return min;
}


//Stampa della Tableau a priorità
void tableau_print(TABLEAU T_young)   {
    int idx;
    printf("\n");
    for(idx=1;idx<=*(T_young[0]);idx++)    
        printf("T_young[%d] = %d\n", idx, *(T_young[idx_row][idx_col]));
    printf("\n");
}


int tableau_isEmpty(TABLEAU T_young) {
    if(*(T_young[0]) == 0)   
        return 1;
    return 0;
}
