/*
 * QBOS: qbos.cpp
 *
 * funcion main del juego.
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

#include "NextPiece.h"

#include <iostream>
using namespace std;


NextPiece::NextPiece(int x,int y,int z,SDL_Surface* surface):SurfaceWidget(x,y,0,0,z)
{
	surface_=0;
	setSurface(surface);
}

NextPiece::~NextPiece()
{
	if(surface_)
		SDL_FreeSurface(surface_);
}

void NextPiece::setPosition(int x,int y)
{
	setX(x);
	setY(y);
	setUpdate(true);
}

void NextPiece::setSurface(SDL_Surface* surface)
{
	if(surface)
	{
		if(surface_)
			SDL_FreeSurface(surface_);
		surface_=surface;
		setWidth(surface_->w);
		setHeight(surface_->h);
		setUpdate(true);
	}
}



//Implementacion metodo abstracto tge::SurfaceWidget

SDL_Surface* NextPiece::getSurface()
{
	return surface_;
}

