/*
 * QBOS: qbos.cpp
 *
 * Clase base para derivar objectos que caen en Matrix y que
 * "chocan"; piezas y bloques en este minuto.
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

#include "FallingObject.h"
#include "constants.h"
#include "tge/Util.h"

using tge::Util;



FallingObject::FallingObject(int x,int y,int w,int h,int z):Widget(x,y,w,h,z)
{
}

FallingObject::~FallingObject()
{
}


void FallingObject::down(int pixs)
{
	if(pixs)
	{
		vector<SDL_Rect>::iterator i;
		for(i=rects_.begin();i!=rects_.end();i++)
			(*i).y+=pixs;
		setY(getY()+pixs);
		setUpdate(true);
	}
}

SDL_Rect FallingObject::collapse(SafeMatrix& matrix)
{
	int min=getMinimumDeltaY(matrix);
	int dy=min*BLOCK_DIM;
	setY(getY()+dy);
	refreshRects();

	int row=(getY()-MATRIX_Y)/BLOCK_DIM;
	int col=(getX()-MATRIX_X)/BLOCK_DIM;
	putIn(row,col,matrix);

	return Util::mergeRects(getLastPaintedRect(),getRect());
}

const vector<SDL_Rect>& FallingObject::getRects()
{
	return rects_;
}

