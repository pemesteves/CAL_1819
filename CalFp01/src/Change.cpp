/*
 * Change.cpp
 */

#include "Change.h"
#include <sstream>
#include <iostream>
#include <limits.h>

string calcChange(int m, int numCoins, int *coinValues)
{
	if(coinValues[0] > m && m > 0)
		return "-";

	int minCoins[m+1];
	minCoins[0] = 0;
	for(int i = 1; i <= m; i++){
		minCoins[i] = INT_MAX;
	}

	int lastCoin[m+1];
	for(int i = 0; i <= m; i++){
		lastCoin[i] = 0;
	}

	for(int i = 0; i < numCoins; i++){
		for(int k = coinValues[i]; k <= m; k++){
			if(minCoins[k-coinValues[i]] < minCoins[k]){
				minCoins[k] = minCoins[k-coinValues[i]] + 1;
				lastCoin[k] = coinValues[i];
			}
		}
	}


	ostringstream oss;

	while(m > 0){
		oss << lastCoin[m] << ";";
		m -= lastCoin[m];
	}

    return oss.str();
}


