/*
 * QBOS: qbos.cpp
 *
 * Matriz bidimensional "segura", y con hartas comodidades que
 * facilitan tareas del juego.
 *
 *
 *
 * Copyright (C) 2006, Jorge Riquelme Santana <totex@totex.cl>
 * 
 * QBOS is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 * 
 * QBOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QBOS; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include <iostream>
#include "SafeMatrix.h"
#include "constants.h"

using std::cout;
using std::endl;



SafeMatrix::SafeMatrix()
{
	rows_=cols_=0;
	matrix_=0;
}

SafeMatrix::SafeMatrix(int rows,int cols)
{
	rows_=rows;
	cols_=cols;
	matrix_=new int*[rows_];
	for(int i=0;i<rows_;i++)
		matrix_[i]=new int[cols_];
}

SafeMatrix::SafeMatrix(const SafeMatrix& matrix)
{
	rows_=matrix.rows_;
	cols_=matrix.cols_;
	matrix_=new int*[rows_];
	for(int i=0;i<rows_;i++)
	{
		matrix_[i]=new int[cols_];
		for(int j=0;j<cols_;j++)
			matrix_[i][j]=matrix.matrix_[i][j];
	}
}

SafeMatrix::~SafeMatrix()
{
	if(matrix_)
	{
		for(int i=0;i<rows_;i++)
			delete [] matrix_[i];
		delete [] matrix_;
	}
}


int SafeMatrix::get(int r,int c)
{
	return matrix_[r][c];
}

void SafeMatrix::set(int r,int c,int value)
{
	if(r>=0 && r<MATRIX_ROWS && c>=0 && c<MATRIX_COLS)
		matrix_[r][c]=value;
}

int SafeMatrix::getRows()
{
	return rows_;
}

int SafeMatrix::getCols()
{
	return cols_;
}


void SafeMatrix::rotateLeft()
{
	for(int i=0;i<cols_;i++)
		reverseCol(i);
	transpose();
}

void SafeMatrix::rotateRight()
{

	for(int i=0;i<rows_;i++)
		reverseRow(i);
	transpose();

}


int SafeMatrix::getLowerRow(int from_row,int col)
{
	int i;
	for(i=from_row;i<rows_;i++)
		if(matrix_[i][col]!=-1)
			break;
	return i;
}

void SafeMatrix::print()
{
	for(int i=0;i<rows_;i++,cout<<endl)
		for(int j=0;j<cols_;j++)
			cout<<matrix_[i][j]<<"\t";
}


SafeMatrix& SafeMatrix::operator=(SafeMatrix& matrix)
{
	rows_=matrix.rows_;
	cols_=matrix.cols_;
	matrix_=new int*[rows_];
	for(int i=0;i<rows_;i++)
	{
		matrix_[i]=new int[cols_];
		for(int j=0;j<cols_;j++)
			matrix_[i][j]=matrix.matrix_[i][j];
	}
	return *this;
}

SafeMatrix& SafeMatrix::operator=(SafeMatrix matrix)
{
	rows_=matrix.rows_;
	cols_=matrix.cols_;
	matrix_=new int*[rows_];
	for(int i=0;i<rows_;i++)
	{
		matrix_[i]=new int[cols_];
		for(int j=0;j<cols_;j++)
			matrix_[i][j]=matrix.matrix_[i][j];
	}
	return *this;
}


//Metodos privados

void SafeMatrix::reverseRow(int row)
{
	int i,j;
	i=0;
	j=cols_-1;
	while(i<j)
	{
		int aux=matrix_[row][i];
		matrix_[row][i]=matrix_[row][j];
		matrix_[row][j]=aux;
		i++;
		j--;
	}
}

void SafeMatrix::reverseCol(int col)
{
	int i,j;
	i=0;
	j=rows_-1;
	while(i<j)
	{
		int aux=matrix_[i][col];
		matrix_[i][col]=matrix_[j][col];
		matrix_[j][col]=aux;
		i++;
		j--;
	}
}

void SafeMatrix::transpose()
{
	int** matrix;
	matrix=new int*[cols_];
	for(int i=0;i<cols_;i++)
		matrix[i]=new int[rows_];

	for(int i=0;i<cols_;i++)
		for(int j=0;j<rows_;j++)
			matrix[i][j]=matrix_[j][i];

	for(int i=0;i<rows_;i++)
		delete [] matrix_[i];
	delete [] matrix_;

	int aux=rows_;
	rows_=cols_;
	cols_=aux;

	matrix_=matrix;
}

