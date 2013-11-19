/*
 * QBOS: NextPiece.h
 *
 * Widget que muestra la proxima pieza.
 *
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

#ifndef _NEXT_PIECE_H
#define _NEXT_PIECE_H

#include "tge/SurfaceWidget.h"

using tge::SurfaceWidget;


class NextPiece:public SurfaceWidget
{
	SDL_Surface* surface_;
protected:
	SDL_Surface* getSurface();
public:
	NextPiece(int x,int y,int z,SDL_Surface* surface);
	virtual ~NextPiece();

	void setPosition(int x,int y);
	void setSurface(SDL_Surface* surface);
};


#endif
