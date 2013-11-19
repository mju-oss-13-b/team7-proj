/*
 * QBOS: Block.h
 *
 * Un bloque de cubos que cae. Los pedazos de matriz que quedan
 * en el aire despues de que se hace una linea se hacen caer en
 * forma de Block.
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

#ifndef _BLOCK_H
#define _BLOCK_H

#include "FallingObject.h"


class Block:public FallingObject
{
	int getNext(int row,int col,bool free=false);
protected:
	void refreshRects();
	void putIn(int row,int col,SafeMatrix& matrix);
	int getMinimumDeltaY(SafeMatrix& matrix);
public:
	static SDL_Surface* (*blocks_)[5];

	Block(int row,int z,SafeMatrix& matrix);
	~Block();

	void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0);
};

#endif
