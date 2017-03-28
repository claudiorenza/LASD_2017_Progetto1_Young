#include "num.h"

// Genera un numero casuale tra a e b
int num_random(int a, int b) {
	return a+(rand() % (b-a+1));
}

