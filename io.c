#include "io.h"

//Input di stringa robusto, con troncamento nel caso si inseriscano più caratteri del dovuto
void io_getString(char *string, int length) {
	int idx;
	char c;
	for(idx=0;(idx<length && (c = getchar())!='\n' && c!=EOF);idx++)
		string[idx] = c;
	string[idx] = '\0';
}

//Input di interi robusto fino a 9 cifre
int io_getInteger() {
	char string[9];
	io_getString(string, 9);
	return atoi(string);	//conversione in valore intero
}

//Input di char
char io_getChar() {
	char string[2];
	io_getString(string, 2);
	return string[0];
}

//Pulizia del terminale
void io_clearScreen()	{
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

//Pressione del tasto per continuare
void io_pressKey()	{
	printf("\n");
	printf("Premere un tasto per continuare...");
	getchar();
}