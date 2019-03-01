/*
 * Sum.cpp
 */

#include "Sum.h"
#include <chrono>
#include <sstream>
#include <limits.h>

string calcSum(int* sequence, int size)
{
	ostringstream oss;

	for(int k = 1; k <= size; k++){
		int s = INT_MAX, i = 0;
		for(int j = 0; j <= size - k; j++){
			int sub_soma = 0;
			for (int w = j; w < j + k; w++){
				sub_soma += sequence[w];
			}
			if(sub_soma < s){
				s = sub_soma;
				i = j;
			}
		}
		oss << s << "," << i << ";";
	}

	return oss.str();
}

