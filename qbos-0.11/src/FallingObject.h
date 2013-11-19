/*
 * QBOS: FallingObject.h
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

#ifndef _FALLING_OBJECT_H
#define _FALLING_OBJECT_H

#include <vector>
#include "SafeMatrix.h"
#include "tge/Widget.h"

using std::vector;
using tge::Widget;


class FallingObject:public Widget
{
protected:
	SafeMatrix matrix_;
	vector<SDL_Rect> rects_;

	virtual void refreshRects()=0;
	virtual void putIn(int row,int col,SafeMatrix& matrix)=0;
	virtual int getMinimumDeltaY(SafeMatrix& matrix)=0;
public:
	FallingObject(int x,int y,int w,int h,int z);
	virtual ~FallingObject();

	void down(int pixs);
	const vector<SDL_Rect>& getRects();
	SDL_Rect collapse(SafeMatrix& matrix);

	virtual void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0)=0;
};

#endif
