/*
 * QBOS: SafeMatrix.h
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

#ifndef _SAFE_MATRIX_H
#define _SAFE_MATRIX_H


class SafeMatrix
{
	int** matrix_;
	int rows_,cols_;

	void reverseRow(int row);
	void reverseCol(int col);
	void transpose();
public:
	SafeMatrix();
	SafeMatrix(int row,int cols);
	SafeMatrix(const SafeMatrix& matrix);
	~SafeMatrix();

	int get(int x,int y);
	void set(int x,int y,int value);
	int getRows();
	int getCols();

	void rotateLeft();
	void rotateRight();

	int getLowerRow(int from_row,int col);
	void print();

	SafeMatrix& operator=(SafeMatrix& matrix);
	SafeMatrix& operator=(SafeMatrix matrix);
};

#endif
