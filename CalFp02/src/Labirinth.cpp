/*
 * labirinth.cpp
 */

#include "Labirinth.h"

#include <iostream>
using namespace std;



Labirinth::Labirinth(int values[10][10])
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			labirinth[i][j] = values[i][j];

}


void Labirinth::initializeVisited()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			visited[i][j] = false;
}




void  Labirinth::printLabirinth()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			cout << labirinth[i][j] << " ";

		cout << endl;
	}
}


bool Labirinth::findGoal(int x, int y)
{
	if(x == 1 && y == 1)
		initializeVisited();

	visited[x][y] = true;

	if(labirinth[x][y] == 0)
		return false;
	else if(labirinth[x][y] == 2)
		return true;

	bool w = false, a = false, s = false, d = false;
	if(x < 9 && !visited[x+1][y])
		s = true;
	if(x > 0 && !visited[x-1][y])
		w = true;
	if(y < 9 && !visited[x][y+1])
		d = true;
	if(y > 0 && !visited[x][y-1])
		a = true;

	if(d){
		if(!visited[x][y+1]){
			if(findGoal(x, y+1))
				return true;
		}
	}

	if(s){
		if(!visited[x+1][y]){
			if(findGoal(x+1, y))
				return true;
		}
	}

	if(a){
		if(!visited[x][y-1]){
			if(findGoal(x, y-1))
				return true;
		}
	}

	if(w){
		if(!visited[x-1][y]){
			if(findGoal(x-1, y))
				return true;
		}
	}

	return false;
}


