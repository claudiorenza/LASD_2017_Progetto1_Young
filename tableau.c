#include "tableau.h"

//Interfaccia utente
int tableau_menu(int isEmpty)	{
	int choiceMenu;
	printf("\t\t\tTableau - MENÙ PRINCIPALE\n");
	printf("1. Genera Tableau\n");
	printf("2. Inserisci nuovo elemento\n");
    if(!isEmpty)    {   //se il Tableau esiste, mostro gli altri elementi del menù
        printf("3. Stampa Tableau\n");
        printf("4. Visualizza/Estrai elemento minimo\n");
        printf("5. Cancella elemento\n");
        printf("6. Cancella il Tableau");
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
            *(newTableau[0][0]) = 0;            //il valore del numero di elementi del Tableau/numero degli elementi
        else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU elements - tableau_init\n");
            exit(1);
        }
        if((newTableau[1][0] = (int *)malloc(sizeof(int))) && (newTableau[0][1] = (int *)malloc(sizeof(int))))  {
            *(newTableau[1][0]) = 0;    //Preparo l'indice massimo di riga
            *(newTableau[0][1]) = 0;    //e l'indice massimo di colonna
        } else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU elements - tableau_init\n");
            exit(1);
        }
        if((newTableau[2][0] = (int *)malloc(sizeof(int))) && (newTableau[0][2] = (int *)malloc(sizeof(int))))  {
            *(newTableau[2][0]) = 0;    //Preparo l'indice di riga
            *(newTableau[0][2]) = 0;    //e l'indice di colonna dell'ultimo elemento inserito
        } else  {
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU ultimo elemento - tableau_init\n");
            exit(1);
        }            
        if(!(newTableau[0][3] = (int *)calloc(MAX_matrix*MAX_matrix, sizeof(int))))  {  //array hash per il controllo dei valori preesistenti nel Tableau a tempo costante
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU hash - tableau_generate\n");
            exit(1);
        }
    
    } else {//in caso di errori nell'allocazione
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU righe - tableau_init\n");
        exit(1);
    }
    return newTableau;
}

void tableau_setLimits(TABLEAU T_young) {
    int idx_row, idx_col;
    do  {
        printf("Quante righe vuoi nel Tableau? (2-%d): ", MAX_matrix);
        if((idx_row = io_getInteger()) < 2 || idx_row > MAX_matrix)
            printf("ATTENZIONE: Valore non valido\n\n");
    }while(idx_row < 2 || idx_row > MAX_matrix);
    
    do  {
        printf("Quante colonne vuoi nel Tableau? (2-%d): ", MAX_matrix);
        if((idx_col = io_getInteger()) < 2 || idx_col > MAX_matrix)
            printf("ATTENZIONE: Valore non valido\n\n");
    }while(idx_col < 2 || idx_col > MAX_matrix);

    *(T_young[1][0]) = idx_row;    //Pongo l'indice massimo di riga
    *(T_young[0][1]) = idx_col;    //e l'indice massimo di colonna
}

//Riempimento matrice e creazione dell'Heap
void tableau_generate(TABLEAU T_young)   {
    int n_elem, choice;
    do  {
        printf("Vuoi inserire manualmente le grandezze del Tableau o vuoi generarla automaticamente?\n\t1. Manuale\t 2. Automatica\n\nSCELTA: ");
        if((choice = io_getInteger()) < 1 || choice > 2)
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(choice < 1 || choice > 2);
    
    if(choice == 1)    //in caso di inserimento manuale del numero di righe e colonne
        tableau_setLimits(T_young);

    do  {
        printf("Quanti elementi vuoi inserire nel Tableau? ");
        if(choice == 1)
            printf("(1-%d): ", *(T_young[1][0]) * *(T_young[0][1]));
        if(choice == 2)
            printf("(1-%d): ", MAX_matrix*MAX_matrix);
        if((n_elem = io_getInteger()) < 1 || (choice == 1 && n_elem > *(T_young[1][0]) * *(T_young[0][1])) || (choice == 2 && n_elem > MAX_matrix*MAX_matrix)) //si possono inserire in input idx_row*idx_col elementi
			printf("ATTENZIONE: Valore non valido\n\n");
	}while(n_elem < 1 || (choice == 1 && n_elem > *(T_young[1][0]) * *(T_young[0][1])) || (choice == 2 && n_elem > MAX_matrix*MAX_matrix));
    
    if(choice == 2) //in caso di generazione automatica del numero di righe e colonne in formato quadratico
        if(sqrt(n_elem) != (*(T_young[1][0]) = *(T_young[0][1]) = (int)sqrt(n_elem)) || sqrt(n_elem) == 1)   //se avviene un troncamento con il cast oppure si inserisce un solo elemento
            *(T_young[1][0]) = *(T_young[0][1]) += 1;           //aumento l'indice di riga e colonna
    
    for(int i=0;i<n_elem;i++)   
        tableau_insertKey(T_young, tableau_generate_rndCheck(T_young[0][3], 1, MAX_matrix*MAX_matrix)); //inserimento di valori casuali e controllo con i già presenti
    tableau_print(T_young);    //stampa del Tableau generata
}

//Controllo elementi disponibili per il ritorno di valori casuali sempre diversi
int tableau_generate_rndCheck(int *hash, int min, int max)   {
    int val;
    while(hash[(val = random_num(min, max))]) //cerca un valore casuale disponibile
        ;       //il ciclo si ferma appena trova nell'hash una posizione 'val' vuota
    hash[val] = 1;
    return val;
}


//Inserimento nuovo elemento a tempo costante nell'ultima posizione del Tableau con riordino in swap
void tableau_insertKey(TABLEAU T_young, int random)  {
    tableau_insertKey_setLast(T_young); //aggiorno gli indici di posizionamento dell'ultimo elemento dell'albero pieno per l'inserimento a tempo costante
    *(T_young[0][0]) += 1;   //incremento il numero di elementi del Tableau/numero degli elementi
    if((T_young[*(T_young[2][0])][*(T_young[0][2])] = (int *)malloc(sizeof(int)))) {		//controllo di corretta allocazione dinamica di memoria
        if(!random)  {  //random == 0, quindi inserisco il valore manualmente
            int val;
            do  {
                printf("Quale valore vuoi inserire nel Tableau? (1-%d): ", MAX_matrix*MAX_matrix);
                if((val = io_getInteger()) < 1 || val > MAX_matrix*MAX_matrix)
                    printf("ATTENZIONE: Valore non valido\n\n");
                if((T_young[0][3])[val])
                    printf("ATTENZIONE: Valore già presente nel Tableau\n\n");
            }while((val < 1 || val > MAX_matrix*MAX_matrix) || ((T_young[0][3])[val]));
            (T_young[0][3])[val] = 1;       //pongo a 1 il valore nell'indice 'val' dell'hash per il controllo dei numeri già presenti
            *T_young[*(T_young[2][0])][*(T_young[0][2])] = val;   //posiziono il valore nell'ultima posizione
        } else  {
            *T_young[*(T_young[2][0])][*(T_young[0][2])] = random;   //pongo un valore casuale
        }
        tableau_minHeap_orderPadre(T_young, *(T_young[2][0]), *(T_young[0][2])); //riordino il Tableau dall'ultima posizione a salire
    } else  {
        printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAUptr - tableau_generate\n");
        exit(1);
    }
}

//Aggiorna i valori dell'indice di riga e di colonna utile all'inserimento di un nuovo elemento nel Tableau come ultima foglia dell'albero
void tableau_insertKey_setLast(TABLEAU T_young) {
    if(tableau_isEmpty(T_young))    {   //primo elemento da inserire
        *(T_young[2][0]) = 1;   //pongo l'indice di riga a 1
        *(T_young[0][2]) = 1;   //e di colonna a 1 per identificare il posto di inserimento del primo elemento
    } else if(*(T_young[2][0]) > 1 && *(T_young[0][2]) < *(T_young[0][1])) {   //se l'ultimo elemento è posizionato all'interno della matrice
        *(T_young[2][0]) -= 1;  //aggiorno la posizione dell'ultimo elemento alla riga precedente
        *(T_young[0][2]) += 1;  //e alla colonna successiva
    } else   {  //se sto alla prima riga o all'ultima colonna
        int delta = *(T_young[2][0]) + *(T_young[0][2]);
        if(delta <= *(T_young[1][0])) {    //se non ho raggiunto già l'ultima riga (sto sulla triangolare superiore)
            *(T_young[2][0]) = delta;     //pongo l'indice di riga all'ultima disponibile
            *(T_young[0][2]) = 1;                                       //e l'indice di colonna alla prima
        } else {    //se invece sto sulla triangolare inferiore
            *(T_young[2][0]) = *(T_young[1][0]);            //pongo l'indice di riga all'ultima
            *(T_young[0][2]) = delta + 1 - *(T_young[1][0]);    //e l'indice di colonna alla prima disponibile
        }
    }
}

//Ricerca del valore nel Tableau
//N.B.: i parametri per riferimento degli indici permette alla funzione chiamante di avere la posizione del valore trovato nel Tableau
TABLEAUptr tableau_searchKey(TABLEAU T_young, int *p_idx_row, int *p_idx_col, int key)    {
    for(int idx_row=1;idx_row<=*(T_young[1][0]) && T_young[idx_row][1];idx_row++) {         //ciclo solo fino all'ultima riga 
        for(int idx_col=1;idx_col<=*(T_young[0][1]) && T_young[idx_row][idx_col] && key >= *(T_young[idx_row][idx_col]);idx_col++)  { //e all'ultima colonna disponibile, controllando di non superare il valore richiesto
            if(key == *(T_young[idx_row][idx_col]))    { //se ho trovato effettivamente il valore
                *p_idx_row = idx_row;   //passo al puntatore l'indice di riga
                *p_idx_col = idx_col;   //e di colonna
                return T_young[idx_row][idx_col];     //ritorno il suo puntatore
            }
        }
    }
    return NULL;    //se non trovo il valore
}

//Sostituzione del valore con l'ultimo elemento del Tableau
void tableau_overwrite(TABLEAU T_young, int idx_row, int idx_col)    {
    T_young[idx_row][idx_col] = tableau_free_node(T_young[idx_row][idx_col]);   //libero il puntatore in posizione [idx_row][idx_col]
    *(T_young[0][0]) -= 1;    //diminuisco il numero di elementi del Tableau
    if(!tableau_isEmpty(T_young))   {   //se esiste ancora il Tableau
        if(T_young[*(T_young[2][0])][*(T_young[0][2])]) {   //controllo se non ho eliminato l'elemento in ultima posizione
            T_young[idx_row][idx_col] = T_young[*(T_young[2][0])][*(T_young[0][2])];     //assegno alla posizione appena cancellata il puntatore all'ultimo elemento Heap
            T_young[*(T_young[2][0])][*(T_young[0][2])] = NULL;     //tolgo il riferimento all'ultima posizione
        }
        tableau_overwrite_setLast(T_young); //aggiorno gli indici di posizionamento dell'ultimo elemento per l'inserimento a tempo costante
    }
}

//Aggiorna i valori dell'indice di riga e di colonna utile alla cancellazione di un elemento nel Tableau
void tableau_overwrite_setLast(TABLEAU T_young) {
    if(*(T_young[0][2]) > 1 && *(T_young[2][0]) < *(T_young[1][0])) {   //se l'ultimo elemento è posizionato all'interno della matrice
        *(T_young[2][0]) += 1;  //aggiorno la posizione dell'ultimo elemento alla riga successiva
        *(T_young[0][2]) -= 1;  //e alla colonna precedente
    } else   {  //se sto alla ultima riga o alla prima colonna
        int delta = *(T_young[2][0]) + *(T_young[0][2]);
        if((*(T_young[1][0]) >= *(T_young[0][1]) && delta <= *(T_young[0][1])+1) || (*(T_young[1][0]) < *(T_young[0][1]) && delta <= *(T_young[1][0])+1)) {    //se mi trovo sulla triangolare superiore
            *(T_young[0][2]) = *(T_young[2][0]) - 1;    //pongo l'indice di colonna all'ultimo disponibile
            *(T_young[2][0]) = 1;                       //e pongo l'indice di riga alla prima
        } 
        else {    //se invece sto sulla triangolare inferiore
            if(*(T_young[1][0]) >= *(T_young[0][1]) || (*(T_young[1][0]) < *(T_young[0][1]) && delta > *(T_young[0][1]) + 1)) {  //quadrato o rettangolo verticale
                *(T_young[0][2]) = *(T_young[0][1]);    //pongo l'indice di colonna all'ultima posizione
                *(T_young[2][0]) = delta - 1 - *(T_young[0][1]);    //e l'indice di riga al primo disponibile
            } else  {
                *(T_young[0][2]) = delta - 2;    //pongo l'indice di colonna alla all'ultimo disponibile
                *(T_young[2][0]) = 1;    //e l'indice di riga a 1
            }
        }
    }
}

//Eliminazione elemento con riordino dalla posizione
void tableau_deleteKey(TABLEAU T_young) {
    int idx_row=1, idx_col=1;
    TABLEAUptr elem;
    tableau_print(T_young);
    printf("Quale valore vuoi eliminare dal Tableau? ");
    if((elem = tableau_searchKey(T_young, &idx_row, &idx_col, io_getInteger())))   {  //se trovato, restituisce il puntatore contenente il valore e gli indici di posizione
        int val_del = *(elem);     //salvo il valore trovato
        (T_young[0][3])[val_del] = 0;
        tableau_overwrite(T_young, idx_row, idx_col);   //sovrascrivo il valore in posizione Tableau[idx_row][idx_col] con l'ultimo elemento del Tableau
        if(!tableau_isEmpty(T_young)) {   //controllo se ci sono elementi
            if(T_young[idx_row][idx_col])   {   //se non ho cancellato proprio l'elemento nell'ultima posizione
                if(*(T_young[idx_row][idx_col]) < val_del)   {  //confronto il valore eliminato con il sostituito, utile al riordino in "Heap"
                    tableau_minHeap_orderPadre(T_young, idx_row, idx_col);
                } else    {
                    tableau_minHeap_heapify(T_young, idx_row, idx_col); //riordino a scendere dalla posizione attuale
                }
            }
            tableau_print(T_young);    //stampa del Tableau aggiornata
        } else  {   //se ho eliminato l'ultimo elemento'
            tableau_free(T_young, 0);   //con '0' in parametro non elimino completamente l'matrice ma solo tutti i puntatori al suo interno
            printf("Tableau eliminato\n\n");
        }
        printf("Valore eliminato in Tableau[%d][%d] = %d\n", idx_row, idx_col, val_del);  //stampa dell'indice e del valore eliminato
    } else
        printf("ATTENZIONE: valore non presente\n");   
}

//Eliminazione dello Heap
void tableau_delete(TABLEAU T_young) {
    char choice;
    do  {
        printf("Sei sicuro di voler eliminare il Tableau? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: scelta non valida\n\n");
    }while(choice != 'S' && choice != 'N');

    if(choice == 'S')   {
        T_young = tableau_free(T_young, 0);   //con '0' in parametro non elimino completamente l'matrice ma solo tutti i puntatori al suo interno
        if(tableau_isEmpty(T_young))
            printf("Tableau eliminato\n\n");
    }
}


//Liberazione efficiente della memoria allocata dinamicamente
TABLEAU tableau_free(TABLEAU T_young, int del_complete)	{   //il parametro 'del_complete' == 1 libera memoria in fase di chiusura dell'applicazione
    for(int idx_row=1;idx_row<=*(T_young[1][0]) && T_young[idx_row][1];idx_row++) {         //ciclo solo fino all'ultima riga 
        for(int idx_col=1;idx_col<=*(T_young[0][1]) && T_young[idx_row][idx_col];idx_col++) //e all'ultima colonna disponibile
            T_young[idx_row][idx_col] = tableau_free_node(T_young[idx_row][idx_col]);   //libero il vertice dall'matrice
    }
    if(del_complete) {  //se sto chiudendo l'applicazione
        free(T_young[0][0]);  //dealloco il numero di elementi del Tableau
        free(T_young[1][0]);  //dealloco l'indice di riga massimo
        free(T_young[0][1]);  //dealloco l'indice di colonna massimo
        free(T_young[2][0]);  //dealloco l'indice di riga dell'ultimo elemento
        free(T_young[0][2]);  //dealloco l'indice di colonna dell'ultimo elemento
        free(T_young[0][3]);  //dealloco l'hash
	    free(T_young);     //dealloco il Tableau
        return NULL;
    } else  //se l'utente decide di eliminare il Tableau
        *(T_young[0][0]) = 0; //resetto il numero di elementi del Tableau
        *(T_young[1][0]) = 0; //resetto l'indice di riga massimo
        *(T_young[0][1]) = 0; //resetto l'indice di colonna massimo
        *(T_young[2][0]) = 0; //resetto l'indice di riga dell'ultimo elemento
        *(T_young[0][2]) = 0; //resetto l'indice di colonna dell'ultimo elemento
        free(T_young[0][3]);
        if(!(T_young[0][3] = (int *)calloc(MAX_matrix*MAX_matrix, sizeof(int))))  {  //array hash per il controllo dei valori preesistenti nel Tableau a tempo costante
            printf("[MEM] ATTENZIONE: Problema di allocazione TABLEAU hash - tableau_generate\n");
            exit(1);
        }
	return T_young;
}

//Deallocazione del singolo vertice del Tableau con restituzione di NULL
TABLEAUptr tableau_free_node(TABLEAUptr T_young_el) {
    free(T_young_el);
    return NULL;
}


//Visualizzazione dell'elemento minimo
void tableau_min(TABLEAU T_young)  {
    printf("Valore minimo del Tableau: %d\n", *(T_young[1][1]));   //nell'Heap il valore minimo è situato alla radice, con accesso a tempo costante O(1)
    char choice;
    do  {
        printf("Desideri estrarlo? (S/N): ");
        choice = toupper(io_getChar());
        if(choice != 'S' && choice != 'N')
            printf("ATTENZIONE: comando non riconosciuto\n\n");
    }while(choice != 'S' && choice != 'N');
    printf("\n");
    if(choice == 'S')   {
        printf("Valore estratto: %d - Dimensione Tableau: %d\n\n", tableau_extractMin(T_young), *(T_young[0][0]));  //estrazione/cancellazione della radice con visualizzazione del nuovo heapSize
        tableau_print(T_young);
    }
}


//Estrazione dell'elemento minimo
int tableau_extractMin(TABLEAU T_young) {
    int min = *(T_young[1][1]);
    tableau_overwrite(T_young, 1, 1); //scambio con l'ultimo elemento del Tableau
    if(!tableau_isEmpty(T_young))
        tableau_minHeap_heapify(T_young, 1, 1);   //riordino in Heap dalla radice
    return min;
}


//Stampa del Tableau 
void tableau_print(TABLEAU T_young)   {
    int idx_row, idx_col;
    printf("\n");
    for(idx_col=1;idx_col<=*(T_young[0][1]);idx_col++)  //stampo tutti gli indici di colonna
        printf("\t%d", idx_col);
    printf("\n\n\n");
    for(idx_row=1;idx_row<=*(T_young[1][0]);idx_row++) {         //ciclo fino all'ultima riga 
        printf("%d", idx_row);                                  //stampo l'indice di riga  
        for(idx_col=1;idx_col<=*(T_young[0][1]);idx_col++)      //ciclo fino all'ultima colonna
            if(T_young[idx_row][idx_col])   //se è presente il puntatore e quindi il valore
                printf("\t%d", *T_young[idx_row][idx_col]); //lo stampo
        printf("\n\n");
    }
    printf("\n");
}


int tableau_isEmpty(TABLEAU T_young) {
    if(*(T_young[0][0]) == 0 && !T_young[1][1])
        return 1;
    return 0;
}

int tableau_isFull(TABLEAU T_young) {
    if(*(T_young[0][0]) == ((*(T_young[1][0]) * *(T_young[0][1]))) && T_young[*(T_young[1][0])][*(T_young[0][1])])   
        return 1;   //controlla se il numero di elementi corrisponde al numero massimo possibile da inserire e se è presente un valore infondo alla matrice
    return 0;
}
