/*
 * QBOS: PieceDescriptor.cpp
 *
 * Descriptor de una pieza. Esta clase describe el comportamiento
 * de una pieza: su constitucion y como gira en la pantalla.
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

#include <stdexcept>
#include "PieceDescriptor.h"

using std::runtime_error;



PieceDescriptor::PieceDescriptor()
{
}

PieceDescriptor::~PieceDescriptor()
{
}

SafeMatrix PieceDescriptor::getSafeMatrix(int state)
{
	if(!state)
		return matrix_;
	else
	{
		SafeMatrix m=matrix_;
		switch(state)
		{
			case 1:
				m.rotateLeft();
				break;
			case 2:
				m.rotateLeft();
				m.rotateLeft();
				break;
			case 3:
				m.rotateRight();
				break;
			default:
				throw runtime_error("PieceDescriptor::getSafeMatrix(int): estado invalido");
		}
		return m;
	}
}


float PieceDescriptor::getLeftDx(int state)
{
	return left_delta_[state][0];
}

float PieceDescriptor::getLeftDy(int state)
{
	return left_delta_[state][1];
}

float PieceDescriptor::getRightDx(int state)
{
	return right_delta_[state][0];
}

float PieceDescriptor::getRightDy(int state)
{
	return right_delta_[state][1];
}


istream& operator>>(istream& is,PieceDescriptor& pd)
{
	int rows,cols;
	is>>rows>>cols;

	pd.matrix_=SafeMatrix(rows,cols);
	for(int r=0;r<rows;r++)
		for(int c=0;c<cols;c++)
		{
			int led;
			is>>led;
			pd.matrix_.set(r,c,led);
		}
	for(int i=0;i<4;i++)
		is>>pd.left_delta_[i][0]>>pd.left_delta_[i][1];
	for(int i=0;i<4;i++)
		is>>pd.right_delta_[i][0]>>pd.right_delta_[i][1];

	return is;
}

