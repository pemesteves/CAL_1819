/*
 * matcher.cpp
 */
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

vector<int> compute_prefix_function(string pattern){
	int m = pattern.length();
	vector<int> pi(m);
	pi[0] = 0;
	int k = 0;

	for(int q = 1; q < m; q++){
		while(k > 0 && pattern[k+1] != pattern[q]){
			k = pi[k];
		}
		if(pattern[k+1] == pattern[q]){
			k++;
		}
		pi[q] = k;
	}

	return pi;
}

int kmpMatcher(string text, string pattern) {
	int n = text.length();
	int m = pattern.length();
	vector<int> pi = compute_prefix_function(pattern);
	int q = 0;
	int matches = 0;
	for(int i = 0; i < n; i++){
		while(q > 0 && pattern[q] != text[i]){
			q = pi[q-1];
		}
		if(pattern[q] == text[i])
			q++;
		if(q == m){
			matches++;
			q = pi[q-1];
		}
	}

	return matches;
}


int numStringMatching(string filename,string toSearch) {
	ifstream f(filename);
	string line;

	int matches = 0;

	while(!f.eof()){
		getline(f, line);
		matches += kmpMatcher(line, toSearch);
	}

	f.close();

	return matches;
}

int min(int val1, int val2, int val3){
	int min = val1;
	if(val2 < min)
		min = val2;
	if(val3 < min)
		min = val3;
	return min;
}


int editDistance(string pattern, string text) {
	vector<vector<int>> D(pattern.length()+1, vector<int>(text.length()+1));

	for(int i = 0; i <= pattern.length(); i++){
		D[i][0] = i;
	}
	for(int j = 0; j <= text.length(); j++){
		D[0][j] = j;
	}

	for(int i = 1; i <= pattern.length(); i++){
		for(int j = 1; j <= text.length(); j++){
			if(pattern[i-1] == text[j-1]){
				D[i][j] = D[i-1][j-1];
			}
			else{
				D[i][j] = 1 + min(D[i-1][j-1], D[i-1][j], D[i][j-1]);
			}
		}
	}

	return D[pattern.length()][text.length()];
}

float numApproximateStringMatching(string filename,string toSearch) {
	ifstream f(filename);
	string line, word;
	float avg = 0;
	int num_words = 0;

	while(!f.eof()){
		getline(f, line);
		bool cont = true;
		while(cont){
			word = line.substr(0, line.find(" "));
			if(line.find(" ") == string::npos)
				cont = false;
			line = line.substr(line.find(" ")+1);
			num_words++;
			avg += editDistance(toSearch, word);
		}
	}

	f.close();

	return avg /= num_words;;
}
