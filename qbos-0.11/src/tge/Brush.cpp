/*
 * QBOS: tge/Brush.cpp
 *
 * Una "brocha" para pintar fondos, encapsula un color o una
 * surface que sirve como patron de relleno.
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

#include <string>
#include <stdexcept>
#include "Brush.h"

using std::string;
using std::runtime_error;



tge::Brush::Brush()
{
	color_.r=color_.g=color_.b=0;
	use_color_=true;
}

tge::Brush::Brush(const SDL_Color& color)
{
	color_=color;
	use_color_=true;
}

tge::Brush::Brush(SDL_Surface* tile)
{
	tile_=tile;
	if(!tile_)
		throw runtime_error("Brush::Brush(SDL_Surface*): tile nulo");
	use_color_=false;
}

tge::Brush::Brush(const Brush& brush)
{
	if(brush.use_color_)
	{
		color_=brush.color_;
		use_color_=true;
	}
	else
	{
		tile_=SDL_ConvertSurface(brush.tile_
				,brush.tile_->format,brush.tile_->flags);
		if(!tile_)
			throw runtime_error(string("Brush::Brush(const Brush&): ")+SDL_GetError());
		use_color_=false;
	}
}

tge::Brush::~Brush()
{
	if(!use_color_)
	{
		SDL_FreeSurface(tile_);
	}
}

void tge::Brush::paint(SDL_Surface* surface,const SDL_Rect* rect)
{
	if(use_color_)
	{
		if(rect)
		{
			SDL_Rect r=*rect;
			SDL_FillRect(surface,&r,SDL_MapRGB(surface->format,color_.r,color_.g,color_.b));
		}
		else
			SDL_FillRect(surface,0,SDL_MapRGB(surface->format,color_.r,color_.g,color_.b));
	}
	else
	{
		SDL_Rect r1,r2,clip;
		SDL_GetClipRect(surface,&clip);
		if(rect)
			r1=*rect;
		else
		{
			r1.x=r1.y=0;
			r1.w=surface->w;
			r1.h=surface->h;
		}
		SDL_SetClipRect(surface,&r1);
		int xf=r1.x+r1.w;
		int yf=r1.y+r1.h;
		for(r2.x=r1.x;r2.x<xf;r2.x+=tile_->w)
			for(r2.y=r1.y;r2.y<yf;r2.y+=tile_->h)
				SDL_BlitSurface(tile_,0,surface,&r2);
		SDL_SetClipRect(surface,&clip);
	}
}


tge::Brush::Brush& tge::Brush::operator=(const Brush& brush)
{
	if(!use_color_)
		SDL_FreeSurface(tile_);

	if(brush.use_color_)
	{
		color_=brush.color_;
		use_color_=true;
	}
	else
	{
		tile_=SDL_ConvertSurface(brush.tile_
				,brush.tile_->format,brush.tile_->flags);
		if(!tile_)
			throw runtime_error(string("Brush::operator=(...): ")+SDL_GetError());
		use_color_=false;
	}

	return *this;
}

