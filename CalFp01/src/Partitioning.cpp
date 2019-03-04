/*
 * Partioning.cpp
 */

#include "Partitioning.h"


int s_recursive(int n,int k)
{
	if(k == 1 || k == n)
		return 1;
	return s_recursive(n-1, k-1)+ k*s_recursive(n-1, k);
}

int s_dynamic(int n,int k)
{
	int c[n][k];
	for(int i = 0; i < n; i++){
		for(int j = 0; j < k; j++){
			c[i][j] = 1;
		}
	}

	for(int i = 0; i < n; i++){
		for(int j = 0; j < k && j < n; j++){
			if(j + 1 == 1 || i == j){
				c[i][j] = 1;
			}
			else if (i == 0)
				continue;
			else{
				c[i][j] = c[i-1][j-1] + (j + 1)*c[i-1][j];
			}
		}
	}

	return c[n-1][k-1];
}


int b_recursive(int n)
{
	int soma = 0;

	for(int k = 1; k <= n; k++){
		soma += s_recursive(n, k);
	}

	return soma;
}

int b_dynamic(int n)
{
	int soma = 0;

		for(int k = 1; k <= n; k++){
			soma += s_dynamic(n, k);
		}

		return soma;
}


