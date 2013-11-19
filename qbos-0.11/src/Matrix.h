/*
 * QBOS: Matrix.h
 *
 * Si hay una clase importante, esa es Matrix. Representa la
 * matriz de bloques, crea y destruye las piezas, bloques y
 * lineas.
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

#ifndef _MATRIX_H
#define _MATRIX_H

#include <list>
#include <vector>
#include <utility>
#include "SafeMatrix.h"
#include "Piece.h"
#include "PieceDescriptor.h"
#include "Block.h"
#include "Line.h"
#include "Configuration.h"
#include "tge/Widget.h"
#include "tge/Uta.h"
#include "tge/Rapidity.h"
#include "randomc/randomc.h"

using std::list;
using std::vector;
using std::pair;
using tge::Widget;
using tge::Uta;
using tge::Rapidity;


class Matrix:public Widget
{
 	//Arreglo con 5 surfaces que corresponden a los 5 bloques
	//con que se construyen las piezas
	SDL_Surface* blocks_[5];

	//Matriz que representa las piezas que ya cayeron. Los espacios
	//vacios estan representado con -1, luego se usa de 0 a 4 para
	//representar espacios con los bloques de blocks_[5]
	SafeMatrix matrix_;

	pair<int,Rapidity> piece_rapidity_,blocks_rapidity_;
	Rapidity fade_rapidity_;

	PieceDescriptor descriptor_[7];
	Piece* falling_piece_,*next_piece_;
	list<SDL_Rect> refresh_areas_;
	list<Block*> falling_blocks_;
	list<Line*> lines_;

	list<Widget*> widgets_;

	TRandomMersenne* rnd_;
	int ticks_;
	int level_,points_,num_lines_;
	bool dead_;

	Configuration config_;

	bool collidesWithMatrix(const vector<SDL_Rect>& rects);
	void downFallingPiece(int pixs);
	void downFallingBlocks(int pixs);
	void collapseFallingPiece();
	int findLines(int row,int rows);
	Piece* makeRandomPiece();
	void updatePointsAndLevel(int lines);
public:
	Matrix(Configuration& config,int z);
	~Matrix();

	bool isDead();
	int getLevel();
	int getPoints();
	SDL_Surface* getNextPiece();

	void key(SDLKey k);
	void ticks(int t);

	void getUpdateRects(Uta* uta,const SDL_Rect* parent_rect=0);
	void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0);
};

#endif
