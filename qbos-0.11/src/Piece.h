/*
 * QBOS: Piece.h
 *
 * Una pieza :p
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

#ifndef _PIECE_H
#define _PIECE_H

#include "FallingObject.h"
#include "SafeMatrix.h"
#include "PieceDescriptor.h"


class Piece:public FallingObject
{
public:
	//los 4 estados de una pieza
	enum State{NORMAL,LEFT,INVERSE,RIGHT};
protected:
	//matriz de rotacion; da el estado resultante al hacer
	//un giro a la derecha o izquierda, por ejemplo:
	//rotateMatrix[NORMAL][LEFT] es RIGHT
	int rotateMatrix[4][4];

	SDL_Surface* block_; //bloque con que se compone la pieza
	//block_index_ es un numero en [0,4] que identifica al bloque,
	//sirve para marcar la matriz de bloques cuando la pieza choca
	int block_index_;

	PieceDescriptor* descriptor_;

	State state_; //estado actual

	bool canInsert(SafeMatrix& matrix,int row);
	void refreshRects();
	void putIn(int row,int col,SafeMatrix& matrix);
	int getMinimumDeltaY(SafeMatrix& matrix);
public:
	Piece(int x,int y,int z,State state,int block_index
			,SDL_Surface* block,PieceDescriptor* descriptor);
	virtual ~Piece();

	//Los metodos de movimiento tienen todos la misma dinamica, con
	//un pseudo codigo queda claro:
	//
	// pieza->left();
	// if(quedo la caga con la movida)
	// 	pieza->right();
	// else
	// 	pieza->okMove();
	//
	void left(); //muevo a la izquierda (compañeros!)
	void right(); //muevo a la derecha (compatriotas!)
	void rotateLeft(); //roto a la izquierda
	void rotateRight(); // 

	SDL_Surface* getSurface();
	void okMove(); //valido el ultimo movimiento

	bool canInsert(SafeMatrix& matrix);
	SDL_Rect insertOnTop(SafeMatrix& matrix);

	void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0);
};

#endif
