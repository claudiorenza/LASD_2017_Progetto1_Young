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
            *(newTableau[0][0]) = 0;            //il valore del numero di elementi della Tableau/numero degli elementi
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
    
    if((T_young[1][0] = (int *)malloc(sizeof(int))) && (T_young[0][1] = (int *)malloc(sizeof(int))))  {
        *(T_young[1][0]) = idx_row;    //Pongo l'indice massimo di riga
        *(T_young[0][1]) = idx_col;    //e l'indice massimo di colonna
    } else  {
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU elements - tableau_init\n");
        exit(1);
    }
    if((T_young[2][0] = (int *)malloc(sizeof(int))) && (T_young[0][2] = (int *)malloc(sizeof(int))))  {
        *(T_young[2][0]) = 0;    //Pongo l'indice di riga dell'ultimo elemento
        *(T_young[0][2]) = 0;    //e l'indice di colonna dell'ultimo elemento
    } else  {
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU ultimo elemento - tableau_init\n");
        exit(1);
    }

    do  {
        printf("Quanti elementi vuoi inserire nella Tableau? (1-%d): ", idx_row*idx_col);
        if((n_elem = io_getInteger()) < 1 || n_elem > idx_row*idx_col)  //si possono inserire in input idx_row*idx_col elementi
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(n_elem < 1 || n_elem > idx_row*idx_col);

    //Riempimento diagonale della matrice triangolare superiore
    for(idx_row=1;idx_row<=*(T_young[1][0]) && *(T_young[0][0]) <= n_elem;idx_row++)    { //se ho raggiunto il numero di elementi richiesto, fermo il ciclo esterno
        for(idx_col=1;idx_col<=*(T_young[0][1]) && idx_col<=idx_row && *(T_young[0][0])<=n_elem;idx_col++)   {  //se ho raggiunto l'indice di colonna massimo, o ho completato la diagonale 
                                                                                                                //oppure ho raggiunto il numero di elementi richiesto, fermo il ciclo interno
            if((T_young[idx_row-(idx_col-1)][idx_col] = (int *)malloc(sizeof(int)))){		//controllo di corretta allocazione dinamica di memoria
                *(T_young[idx_row-(idx_col-1)][idx_col]) = num_random(1, 256);  //generazione numero casuale da 1 a 256
                *(T_young[0][0]) += 1;   //incremento il numero di elementi della Tableau/numero di elementi
                *(T_young[2][0]) = idx_row-(idx_col-1); //aggiorno l'indice di riga dell'ultimo elemento
                *(T_young[0][2]) = idx_col;             //e l'indice di colonna dell'ultimo elemento
                tableau_minHeap_orderPadre(T_young, idx_row-(idx_col-1), idx_col); //riordino la tableu da questa posizione
            } else  {
                printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUptr - tableau_generate\n");
                exit(1);
            }
        }
    }

    //Eventuale riempimento diagonale della matrice triangolare inferiore
    if(idx_row > *(T_young[1][0]) && *(T_young[0][0]) < n_elem) {    //nel caso debba inserire ancora altri elementi
        idx_row -= 1; //decremento per tornare al giusto indice di ultima riga [== *(T_young[1][0])]
        for(idx_col=2;idx_col<=*(T_young[0][1]) && *(T_young[0][0]) <= n_elem;idx_col++)    { //se ho raggiunto il numero di elementi richiesto, fermo il ciclo esterno
            for(idx_row=*(T_young[1][0]); idx_row>=1 && (*(T_young[1][0])+idx_col)-idx_row<=*(T_young[0][1]) && *(T_young[0][0])<=n_elem;idx_row--)   { //se ho raggiunto l'indice di riga minimo, o ho completato la diagonale 
                                                                                                                                                        //oppure ho raggiunto il numero di elementi richiesto, fermo il ciclo interno
                if((T_young[idx_row][(*(T_young[1][0])+idx_col)-idx_row] = (int *)malloc(sizeof(int)))){		//controllo di corretta allocazione dinamica di memoria
                    *(T_young[idx_row][(*(T_young[1][0])+idx_col)-idx_row]) = num_random(1, 256);  //generazione numero casuale da 1 a 256
                    *(T_young[0][0]) += 1;   //incremento il numero di elementi della Tableau/numero di elementi
                    *(T_young[2][0]) = idx_row;                             //aggiorno l'indice di riga dell'ultimo elemento
                    *(T_young[0][2]) = (*(T_young[1][0])+idx_col)-idx_row;  //e l'indice di colonna dell'ultimo elemento
                    tableau_minHeap_orderPadre(T_young, idx_row, (*(T_young[1][0])+idx_col)-idx_row); //riordino la tableu da questa posizione
                } else  {
                    printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUptr - tableau_generate\n");
                    exit(1);
                }
            }
        }
    }   
    tableau_print(T_young);    //stampa della Tableau generata
}


//Inserimento nuovo elemento a tempo costante nell'ultima posizione della Tableau con riordino in swap
void tableau_insertKey(TABLEAU T_young, int random)  {
    *(T_young[0][0]) += 1;   //incremento il numero di elementi della Tableau e assegno l'indice per la ricostruzione dell'heap
    if(isEmpty(T_young))    {   //primo elemento
        *(T_young[2][0]) = 1;  
        *(T_young[0][2]) = 1;
    } else if(*(T_young[2][0]) > 1 && *(T_young[0][2]) < *(T_young[0][1])) {   //aggiorno la posizione dell'ultimo elemento
        *(T_young[2][0]) -= 1;  //alla riga precedente
        *(T_young[0][2]) += 1;  //e alla colonna successiva
    } else if(*(T_young[2][0]) == 1 && *(T_young[0][2]) < *(T_young[0][1]))  {  //se sto alla prima riga non raggiungendo l'ultima colonna
        if(*(T_young[2][0]) + *(T_young[0][2]) < *(T_young[1][0])) { //se non ho raggiunto l'ultima riga (sto sulla triangolare superiore)
            *(T_young[2][0]) = *(T_young[2][0]) + *(T_young[0][2]);    //pongo all'ultima riga disponibile
            *(T_young[0][2]) = 1;                   //e alla prima colonna
        } else {    //sto sulla triangolare inferiore
            int delta = *(T_young[2][0]) + *(T_young[0][2]) + 1;
            *(T_young[2][0]) = *(T_young[1][0]);    //pongo all'ultima riga
            *(T_young[0][2]) = delta - *(T_young[2][0]);                   //e alla prima colonna
        }
    }
    if(!random)  {
        if((T_young[*(T_young[2][0])][*(T_young[0][2])] = (int *)malloc(sizeof(int)))) {		//controllo di corretta allocazione dinamica di memoria
            printf("Quale valore vuoi inserire nella Tableau? ");
            T_young[*(T_young[2][0])][*(T_young[0][2])] = io_getInteger();   //posiziono la chiave nell'ultima posizione
            
            tableau_minHeap_orderPadre(T_young, *(T_young[2][0]), *(T_young[0][2])); //riordino la tableu dall'ultima posizione
                    
            tableau_print(T_young);    //stampa della Tableau aggiornata
        } else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUptr - tableau_generate\n");
            exit(1);
        }
    } else {
        
    }
}

//Ricerca efficiente del valore nell'Heap (inOrder, senza la visita di valori più grandi di 'key')
//N.B.: i parametri per riferimento degli indici permette alla funzione chiamante di avere la posizione del valore trovato nella Tableau
TABLEAUptr tableau_searchKey(TABLEAU T_young, int *p_idx_row, int *p_idx_col, int key)    {
    int idx_row = *p_idx_row, idx_col = *p_idx_col;     //salvo nell'attuale R.A. gli indici di riga e colonna
    if(T_young[idx_row][idx_col])   {   //se != NULL
        if(key >= *(T_young[idx_row][idx_col]))  {   //controllo se la chiave è più grande del valore visitato nella Tableau
            *p_idx_row = idx_row+1;  //aggiorno i valori nel puntatori degli indici di riga con i valori locali
            *p_idx_col = idx_col;
            printf("\tDEBUG: sx[%d][%d]\n", *p_idx_row, *p_idx_col);
            TABLEAUptr ptr_ret = tableau_searchKey(T_young, p_idx_row, p_idx_col, key); //ricorsivamente scendo a sinistra
            printf("\t\tDEBUG: Local Visit[%d][%d] = %d\n", idx_row, idx_col, *(T_young[idx_row][idx_col]));
            if(ptr_ret)  {  //se l'ho già trovato
                printf("DEBUG: risalgo il trovato in indice[%d][%d]\n", *p_idx_row, *p_idx_col);
                return ptr_ret;   //ritorno già il puntatore ai R.A. precedenti;
            }
            if(key == *(T_young[idx_row][idx_col]))    { //se ho trovato effettivamente il valore
                printf("DEBUG: TROVATO\n");
                *p_idx_row = idx_row;
                *p_idx_col = idx_col;
                return T_young[idx_row][idx_col];     //ritorno già il suo puntatore
            }
            if(!T_young[idx_row+1][idx_col] || !T_young[idx_row][idx_col+1])    {   //se mi trovo in una foglia a scendere, risalgo al R.A. precedente
                printf("DEBUG: risalgo per foglia NULL da indice[%d][%d]\n", *p_idx_row, *p_idx_col);
                return ptr_ret;   //ritorno già il puntatore ai R.A. precedenti;
            }
            *p_idx_row = idx_row;
            *p_idx_col = idx_col+1;  //aggiorno il valore nel puntatore dell'indice di colonna con il valore locale
            printf("\tDEBUG: dx[%d][%d]\n", *p_idx_row, *p_idx_col);
            return tableau_searchKey(T_young, p_idx_row, p_idx_col, key);  //ricorsivamente scendo a destra
        }
    }
    return NULL;   //o non trovo il valore, o eccede key, oppure trovo una foglia NULL
}

//Sostituzione del valore con l'ultimo elemento dello Heap
void tableau_overwrite(TABLEAU T_young, int idx_row, int idx_col)    {
    T_young[idx_row][idx_col] = tableau_free_node(T_young[idx_row][idx_col]);   //libero il puntatore in posizione [idx_row][idx_col]
    //T_young[idx_row][idx_col] = T_young[*(T_young[0])];     //CHECK: assegno alla posizione 'idx' il puntatore all'ultimo elemento Heap
    *(T_young[0][0]) -= 1;   //diminuisco il numero di elementi della Tableau
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
        free(T_young[0]);  //dealloco il numero di elementi della Tableau
	    free(T_young);     //dealloco la Tableau
        T_young = NULL;
    } else  //se l'utente decide di eliminare la Tableau
        *(T_young[0]) = 0; //resetto il numero di elementi della Tableau
	return T_young;
}

//Deallocazione del singolo vertice della Tableau con restituzione di NULL
TABLEAUptr tableau_free_node(TABLEAUptr T_young_el) {
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
