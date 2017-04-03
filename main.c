#include <stdio.h>
#include <time.h>

#include "tableau.h"

int main()  {
    srand(time(NULL));				//funzione per la generazione casuale di valori

	TABLEAU T_young = tableau_init();	//inizializzazione della struttura

	int choiceMenu;
	do	{
		io_clearScreen();
		switch(choiceMenu = tableau_menu(tableau_isEmpty(T_young)))	{	//chiamata del menu principale con scelta dell'albero con controllo di esistenza della Tableau
			case 1:
				printf("GENERAZIONE TABLEAU DI YOUNG IN HEAP\n\n");
				if(!tableau_isEmpty(T_young))	{
					printf("ATTENZIONE: Tableu già presente\n\n");
					tableau_delete(T_young);
				}
				if(tableau_isEmpty(T_young))	//nel caso non decidessi di eliminare la tableu
					tableau_generate(T_young);
				break;
			case 2:
				printf("INSERIMENTO NUOVO ELEMENTO NELLA TABLEAU DI YOUNG\n\n");
				if(tableau_isEmpty(T_young))
					tableau_setLimits(T_young);
                if(!tableau_isFull(T_young))    {
					tableau_insertKey(T_young, 0);	//con il parametro '0' specifico che inserisco manualmente il valore
					tableau_print(T_young);    //stampa della Tableau generata
				}
				else 
        			printf("ATTENZIONE: la Tableu è piena\n\n");
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
				printf("CANCELLAZIONE ELEMENTO DALLA TABLEAU DI YOUNG\n\n");
                tableau_deleteKey(T_young);
				break;
			case 6:
				printf("CANCELLAZIONE TABLEAU DI YOUNG\n\n");
				tableau_delete(T_young);
				break;
            case 7:
				printf("SESSIONE TERMINATA\n\n");
		}
		io_pressKey();
	}while(choiceMenu != 7);

	T_young = tableau_free(T_young, 1); //con il parametro '1', dealloco completamente la Tableau
	io_clearScreen();
	return 1;
}
