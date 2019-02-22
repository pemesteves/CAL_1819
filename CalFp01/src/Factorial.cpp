/*
 * Factorial.cpp
 */

#include "Factorial.h"

int factorialRecurs(int n)
{
	if(n == 0)
		return 1;
	else
		return n*factorialRecurs(n-1);
}

int factorialDinam(int n)
{
	int fatorial = 1;

	while(n > 0){
		fatorial *= n;
		n--;
	}

	return fatorial;
}
