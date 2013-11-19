/*
 * QBOS: Line.cpp
 *
 * Clase que representa una linea. Es un Widget que se hace
 * transparente con cierta rapidez.
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

#include <stdexcept>
#include "Line.h"
#include "constants.h"
#include "tge/Util.h"

using std::exception;
using std::runtime_error;
using tge::Util;



Rapidity* Line::fade_rapidity_=0;

Line::Line(int row,int z,SafeMatrix& matrix,SDL_Surface* (&blocks)[5])
				:SurfaceWidget(MATRIX_X,row*BLOCK_DIM+MATRIX_Y,MATRIX_W,BLOCK_DIM,z)
{
	try
	{
		surface_=Util::createSurface(BLOCK_DIM*MATRIX_COLS,BLOCK_DIM);
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Line::Line(...): ")+ex.what());
	}

	SDL_Rect r;
	r.x=r.y=0;
	for(int i=0;i<MATRIX_COLS;i++,r.x+=BLOCK_DIM)
	{
		SDL_BlitSurface(blocks[matrix.get(row,i)],0,surface_,&r);
		matrix.set(row,i,-1);
	}

	alpha_=SDL_ALPHA_OPAQUE;
	SDL_SetAlpha(surface_,SDL_SRCALPHA|SDL_RLEACCEL,alpha_);

	ticks_=0;
}

Line::~Line()
{
	SDL_FreeSurface(surface_);
}

bool Line::dead()
{
	return alpha_==SDL_ALPHA_TRANSPARENT;
}

void Line::ticks(int t)
{
	ticks_+=t;
	alpha_-=fade_rapidity_->calcUnits(ticks_);
	if(alpha_<SDL_ALPHA_TRANSPARENT)
		alpha_=SDL_ALPHA_TRANSPARENT;
	setUpdate(true);
}

void Line::setFadeRapidity(Rapidity* fade_rapidity)
{
	fade_rapidity_=fade_rapidity;
}



//Implementacion metodo abstracto tge::SurfaceWidget

SDL_Surface* Line::getSurface()
{
	SDL_SetAlpha(surface_,SDL_SRCALPHA|SDL_RLEACCEL,alpha_);
	return surface_;
}

