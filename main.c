#include <stdio.h>
#include <time.h>

#include "tableau.h"

int main()  {
    srand(time(NULL));				//funzione per la generazione casuale di valori

	TABLEAU coda = tableau_init();	//inizializzazione della struttura

	int choiceMenu;
	do	{
		io_clearScreen();
		switch(choiceMenu = tableau_menu(tableau_isEmpty(coda)))	{	//chiamata del menu principale con scelta dell'albero con controllo di esistenza della coda
			case 1:
				printf("GENERAZIONE CODA DI PRIORITA' IN HEAP BINARIO\n\n");
				tableau_generate(coda);
				break;
			case 2:
				printf("INSERIMENTO NUOVO ELEMENTO NELLA CODA\n\n");
                tableau_insertKey(coda);
				break;
			case 3:
				printf("STAMPA CODA DI PRIORITA'\n\n");
                tableau_print(coda);
				break;
			case 4:
				printf("VISUALIZZAZIONE/ESTRAZIONE ELEMENTO MINIMO\n\n");
				tableau_min(coda);
				break;
            case 5:
				printf("CANCELLAZIONE ELEMENTO DA HEAP\n\n");
                tableau_deleteKey(coda);
				break;
			case 6:
				printf("CANCELLAZIONE CODA DI PRIORITA'\n\n");
				tableau_delete(coda);
				break;
            case 7:
				printf("SESSIONE TERMINATA\n\n");
		}
		io_pressKey();
	}while(choiceMenu != 7);

	coda = tableau_free(coda, 1); //con il parametro '1', dealloco completamente la coda
	return 1;
}
