/*
	Name: Tableau di Young
	Copyright: Università degli Studi di Napoli "Federico II"
	Authors: Gruppo 32
			Domenico Pirone N86/1006
			Claudio Renza N86/727
	Date: 06/04/17 18:12
	Description: Librerie di Gestione e Ordinamento del Tableau di Young
*/

#include <stdio.h>
#include <time.h>

#include "tableau.h"

int main()  {
    srand(time(NULL));				//funzione per la generazione casuale di valori

	TABLEAU T_young = tableau_init();	//inizializzazione della struttura

	int choiceMenu;
	do	{
		io_clearScreen();	//cancellazione dello schermo del terminale
		switch(choiceMenu = tableau_menu(tableau_isEmpty(T_young)))	{	//chiamata del menu principale con scelta della funzione per il Tableau
			case 1:
				printf("GENERAZIONE TABLEAU DI YOUNG\n\n");
				if(!tableau_isEmpty(T_young))	{	//se è già presente, chiedo al'utente di eliminare il Tableau precedente
					printf("ATTENZIONE: Tableau già presente\n\n");
					tableau_delete(T_young);
				}
				if(tableau_isEmpty(T_young))	//se il Tableau è vuoto,
					tableau_generate(T_young);	//lo genero
				break;
			case 2:
				printf("INSERIMENTO NUOVO ELEMENTO NEL TABLEAU DI YOUNG\n\n");
				if(tableau_isEmpty(T_young))	//se il Tableau è vuoto
					tableau_setLimits(T_young);	//imposto i margini di riga e colonna
                if(!tableau_isFull(T_young))    {	//se il Tableau è pieno
					tableau_insertKey(T_young, 0);	//inserisco il valore; con il parametro '0' specifico che inserisco manualmente il valore
					tableau_print(T_young);    //stampa del Tableau generata
				}
				else 
        			printf("ATTENZIONE: il Tableau è pieno\n\n");
				break;
			case 3:
				printf("STAMPA TABLEAU DI YOUNG\n\n");
                tableau_print(T_young);
				break;
			case 4:
				printf("VISUALIZZAZIONE/ESTRAZIONE ELEMENTO MINIMO\n\n");
				tableau_min(T_young);
				break;
            case 5:
				printf("CANCELLAZIONE ELEMENTO DAL TABLEAU DI YOUNG\n\n");
                tableau_deleteKey(T_young);
				break;
			case 6:
				printf("CANCELLAZIONE TABLEAU DI YOUNG\n\n");
				tableau_delete(T_young);
				break;
            case 7:
				printf("SESSIONE TERMINATA\n\n");
		}
		io_pressKey();	//"Premere Invio per continuare..."
	}while(choiceMenu != 7);	//se ho scelto dal menu l'opzione 7, esco dal programma

	T_young = tableau_free(T_young, 1); //con il parametro '1', dealloco completamente il Tableau
	io_clearScreen();	//Pulisco lo schermo del terminale
	return 1;
}
