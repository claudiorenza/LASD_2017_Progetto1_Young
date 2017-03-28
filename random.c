#include "random.h"

//Genera un valore casuale tra a e b
int random_num(int a, int b) {
	return a+(rand() % (b-a+1));
}

