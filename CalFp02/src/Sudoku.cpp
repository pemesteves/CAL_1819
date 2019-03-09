/*
 * Sudoku.cpp
 *
 */

#include "Sudoku.h"
#include "limits.h"

/** Inicia um Sudoku vazio.
 */
Sudoku::Sudoku()
{
	this->initialize();
}

/**
 * Inicia um Sudoku com um conteúdo inicial.
 * Lança excepção IllegalArgumentException se os valores
 * estiverem fora da gama de 1 a 9 ou se existirem números repetidos
 * por linha, coluna ou bloc 3x3.
 *
 * @param nums matriz com os valores iniciais (0 significa por preencher)
 */
Sudoku::Sudoku(int nums[9][9])
{
	this->initialize();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (nums[i][j] != 0)
			{
				int n = nums[i][j];
				numbers[i][j] = n;
				lineHasNumber[i][n] = true;
				columnHasNumber[j][n] = true;
				block3x3HasNumber[i / 3][j / 3][n] = true;
				countFilled++;
			}
		}
	}
}

void Sudoku::initialize()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int n = 0; n < 10; n++)
			{
				numbers[i][j] = 0;
				lineHasNumber[i][n] = false;
				columnHasNumber[j][n] = false;
				block3x3HasNumber[i / 3][j / 3][n] = false;
			}
		}
	}

	this->countFilled = 0;
}

/**
 * Obtem o conteúdo actual (só para leitura!).
 */
int** Sudoku::getNumbers()
{
	int** ret = new int*[9];

	for (int i = 0; i < 9; i++)
	{
		ret[i] = new int[9];

		for (int a = 0; a < 9; a++)
			ret[i][a] = numbers[i][a];
	}

	return ret;
}

/**
 * Verifica se o Sudoku já está completamente resolvido
 */
bool Sudoku::isComplete()
{
	return countFilled == 9 * 9;
}


tuple<int, int> Sudoku::getPositionWithLessNumbers(){
	int row = INT_MAX, column = INT_MAX;
	int min = 10;

	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			if(numbers[r][c] != 0)
				continue;

			int num_possibilities = 0;
			for(int n = 1; n <= 9; n++){
				if(!(lineHasNumber[r][n] || columnHasNumber[c][n] || block3x3HasNumber[r/3][c/3][n]))
					num_possibilities++;
			}

			if(num_possibilities == 0)
				return make_tuple(INT_MAX, INT_MAX);

			if(num_possibilities < min){
				min = num_possibilities;
				row = r;
				column = c;
			}
			if(num_possibilities == 1)
				return make_tuple(r, c);
		}
	}

	return make_tuple(row, column);
}

bool Sudoku::putNumberInPosition(int row, int column){
	for(int num = 1; num <= 9; num++){
		if(lineHasNumber[row][num] || columnHasNumber[column][num] || block3x3HasNumber[row/3][column/3][num])
			continue;

		numbers[row][column] = num;
		lineHasNumber[row][num] = true;
		columnHasNumber[column][num] = true;
		block3x3HasNumber[row/3][column/3][num] = true;
		countFilled++;

		if(solve())
			return true;

		numbers[row][column] = 0;
		lineHasNumber[row][num] = false;
		columnHasNumber[column][num] = false;
		block3x3HasNumber[row/3][column/3][num] = false;
		countFilled--;
	}
	return false;
}

/**
 * Resolve o Sudoku.
 * Retorna indicação de sucesso ou insucesso (sudoku impossível).
 */
bool Sudoku::solve()
{
	if(isComplete())
		return true;

	tuple<int, int> t = getPositionWithLessNumbers();
	int row = get<0>(t), column = get<1>(t);
	if(row == INT_MAX && column == INT_MAX)
		return false;

	if(putNumberInPosition(row, column))
		return true;

	return false;
}



/**
 * Imprime o Sudoku.
 */
void Sudoku::print()
{
	for (int i = 0; i < 9; i++)
	{
		for (int a = 0; a < 9; a++)
			cout << this->numbers[i][a] << " ";

		cout << endl;
	}
}
