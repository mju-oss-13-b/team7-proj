/*
 * QBOS: tge/SurfaceWidget.h
 *
 * Clase base abstracta para derivar todos los objetos que se
 * pintan en la pantalla usando el microtiles array.
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

#include "SurfaceWidget.h"



tge::SurfaceWidget::SurfaceWidget(int x,int y,int w,int h,int z):Widget(x,y,w,h,z)
{
}

tge::SurfaceWidget::~SurfaceWidget()
{
}



//Implementacion de tge::Widget::paint(...)

void tge::SurfaceWidget::paint(SDL_Surface* screen,const SDL_Rect* parent_rect)
{
	SDL_Surface* sf=getSurface();
	if(sf)
	if(parent_rect)
	{
		SDL_Rect pos,dim;
		if(calcAbsolutePositionAndDimension(getRect(),parent_rect,pos,dim))
			//SDL_BlitSurface(getSurface(),&dim,screen,&pos);
			SDL_BlitSurface(sf,&dim,screen,&pos);
	}
	else
	{
		SDL_Rect rect=getRect();
		//SDL_BlitSurface(getSurface(),0,screen,&rect);
		SDL_BlitSurface(sf,0,screen,&rect);
	}
	updateLastPaintedRect();
}

