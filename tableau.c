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
                exit(1);
            }
        }
        if((newTableau[0][0] = (int *)malloc(sizeof(int))))      //alloco negli indici...
            *(newTableau[0][0]) = 0;            //il valore dell'heapSize/numero degli elementi
        else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU elements - tableau_init\n");
            exit(1);
        }
    } else {//in caso di errori nell'allocazione
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU righe - tableau_init\n");
        exit(1);
    }
    return newTableau;
}

//Riempimento matrice e creazione dell'Heap
void tableau_generate(TABLEAU T_young)   {
    int idx_row, idx_col, n_elem, 
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
        exit(1);
    }
    if((newTableau[2][0] = (int *)malloc(sizeof(int))) && (newTableau[0][2] = (int *)malloc(sizeof(int))))  {
        *(newTableau[2][0]) = 0;    //Pongo l'indice di riga dell'ultimo elemento
        *(newTableau[0][2]) = 0;    //e l'indice di colonna dell'ultimo elemento
    } else  {
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU ultimo elemento - tableau_init\n");
        exit(1);
    }

    do  {
        printf("Quanti elementi vuoi inserire nella Tableau? (1-%d): ", idx_row*idx_col);
        if((n_elem = io_getInteger()) < 1 || n_elem > idx_row*idx_col)  //si possono inserire in input idx_row*idx_col elementi
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(n_elem < 1 || n_elem > idx_row*idx_col);

    for(int i=0;i<n_elem;i++)   
        tableau_insertKey(T_young, random_num(1, 256));
    tableau_print(T_young);    //stampa della Tableau generata
}


//Inserimento nuovo elemento a tempo costante nell'ultima posizione della Tableau con riordino in swap
void tableau_insertKey(TABLEAU T_young, int random)  {
    *(T_young[0][0]) += 1;   //incremento l'heapSize/numero degli elementi
    tableau_insertKey_setLast(T_young); //aggiorno gli indici di posizionamento dell'ultimo elemento per l'inserimento a tempo costante
    if((T_young[*(T_young[2][0])][*(T_young[0][2])] = (int *)malloc(sizeof(int)))) {		//controllo di corretta allocazione dinamica di memoria
        if(!random)  {  //random == 0, quindi inserisco il valore manualmente
            printf("Quale valore vuoi inserire nella Tableau? ");
            T_young[*(T_young[2][0])][*(T_young[0][2])] = io_getInteger();   //posiziono il valore nell'ultima posizione
        } else {
            T_young[*(T_young[2][0])][*(T_young[0][2])] = random;   //pongo un valore casuale
        }
        printf("DEBUG: Last matrix[%d][%d] = %d\n", *(T_young[2][0]), *(T_young[0][2]), T_young[*(T_young[2][0])][*(T_young[0][2])]);
        tableau_minHeap_orderPadre(T_young, *(T_young[2][0]), *(T_young[0][2])); //riordino la tableu dall'ultima posizione
    } else  {
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUval - tableau_generate\n");
        exit(1);
    }
}

//Aggiorna i valori dell'indice di riga e di colonna utile all'inserimento di un nuovo elemento nella Tableau
void tableau_insertKey_setLast(TABLEAU T_young) {
    if(tableau_isEmpty(T_young))    {   //primo elemento da inserire
        *(T_young[2][0]) = 1;   //pongo l'indice di riga a 1
        *(T_young[0][2]) = 1;   //e di colonna a 1 per identificare il posto di inserimento del primo elemento
    } else if(*(T_young[2][0]) > 1 && *(T_young[0][2]) < *(T_young[0][1])) {   //se l'ultimo elemento è posizionato all'interno della matrice
        *(T_young[2][0]) -= 1;  //aggiorno la posizione dell'ultimo elemento alla riga precedente
        *(T_young[0][2]) += 1;  //e alla colonna successiva
    } else   {  //se sto alla prima riga o all'ultima colonna
        if(*(T_young[2][0]) + *(T_young[0][2]) < *(T_young[1][0])) {    //se non ho raggiunto già l'ultima riga (sto sulla triangolare superiore)
            *(T_young[2][0]) = *(T_young[2][0]) + *(T_young[0][2]);     //pongo l'indice di riga all'ultima disponibile (1 + pos. attuale indice colonna)
            *(T_young[0][2]) = 1;                                       //e l'indice di colonna alla prima
        } else {    //se invece sto sulla triangolare inferiore
            int delta = *(T_young[2][0]) + *(T_young[0][2]) + 1;
            *(T_young[2][0]) = *(T_young[1][0]);            //pongo l'indice di riga all'ultima
            *(T_young[0][2]) = delta - *(T_young[2][0]);    //e l'indice di colonna alla prima disponibile
        }
    }
    printf("DEBUG: Last val in [%d][%d]n", *(T_young[2][0]), *(T_young[0][2]));
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
void tableau_overwrite(TABLEAU T_young, int idx_row, int idx_col)    {
    T_young[idx_row][idx_col] = tableau_free_node(T_young[idx_row][idx_col]);   //libero il puntatore in posizione [idx_row][idx_col]
    //T_young[idx_row][idx_col] = T_young[*(T_young[0])];     //CHECK: assegno alla posizione 'idx' il puntatore all'ultimo elemento Heap
    *(T_young[0][0]) -= 1;   //diminuisco l'heapSize
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
            tableau_minHeap_orderPadre(T_young, idx_row, idx_col)
        } else    {
            tableau_minHeap_heapify(T_young, idx_row, idx_col); //riordino a scendere dalla posizione attuale
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
    int min = *(T_young[1][1]);
    tableau_overwrite(T_young, 1); //scambio con l'ultimo elemento dell'Heap
    tableau_minHeap_heapify(T_young, 1, 1);   //riordino in Heap dalla radice

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
    if(*(T_young[0][0]) == 0)   
        return 1;
    return 0;
}

int tableau_isFull(TABLEAU T_young) {
    if((*(T_young[0][0]) == (*(T_young[1][0]) * *(T_young[0][1]))) &&  T_young[*(T_young[1][0])][*(T_young[0][1])])   
        return 1;   //controlla se il numero di elementi corrisponde al numero massimo possibile da inserire e se è presente un valore nell'ultima posizione
    return 0;
}
