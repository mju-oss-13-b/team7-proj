/*
 * QBOS: tge/Gradient.h
 *
 * Un degradado para rellenar areas
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

#include "Gradient.h"



tge::Gradient::Gradient()
{
}

tge::Gradient::Gradient(const SDL_Color& begin,const SDL_Color& end,Orientation orientation)
{
	begin_=begin;
	end_=end;
	orientation_=orientation;
}

tge::Gradient::~Gradient()
{
}


void tge::Gradient::setBeginColor(const SDL_Color& color)
{
	begin_=color;
}

void tge::Gradient::setEndColor(const SDL_Color& color)
{
	end_=color;
}

void tge::Gradient::setOrientation(Orientation orientation)
{
	orientation_=orientation;
}


void tge::Gradient::paint(SDL_Surface* surface,const SDL_Rect* rect)
{
	SDL_Rect r1,r2;
	if(!rect)
	{
		r1.x=r1.y=0;
		r1.w=surface->w;
		r1.h=surface->h;
	}
	else
		r1=*rect;

	r2.x=r1.x;
	r2.y=r1.y;
	if(orientation_==HORIZONTAL)
	{
		r2.w=1;
		r2.h=r1.h;
	}
	else
	{
		r2.w=r1.w;
		r2.h=1;
	}

	float r=(float)begin_.r;
	float g=(float)begin_.g;
	float b=(float)begin_.b;
	if(orientation_==HORIZONTAL)
	{
		float dr=(float)(end_.r-begin_.r)/(float)(r1.w);
		float dg=(float)(end_.g-begin_.g)/(float)(r1.w);
		float db=(float)(end_.b-begin_.b)/(float)(r1.w);
		for(int xf=r1.x+r1.w;r2.x<xf;r2.x++)
		{
			SDL_FillRect(surface,&r2
					,SDL_MapRGB(surface->format,(int)r,(int)g,(int)b));
			r+=dr;
			g+=dg;
			b+=db;
		}
	}
	else
	{
		float dr=(float)(end_.r-begin_.r)/(float)(r1.h);
		float dg=(float)(end_.g-begin_.g)/(float)(r1.h);
		float db=(float)(end_.b-begin_.b)/(float)(r1.h);
		for(int yf=r1.y+r1.h;r2.y<yf;r2.y++)
		{
			SDL_FillRect(surface,&r2
					,SDL_MapRGB(surface->format,(int)r,(int)g,(int)b));
			r+=dr;
			g+=dg;
			b+=db;
		}
	}
}

