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

#ifndef TGE_GRADIENT_H
#define TGE_GRADIENT_H

#include <SDL/SDL.h>


namespace tge
{

class Gradient
{
public:
	enum Orientation{VERTICAL,HORIZONTAL};
private:
	SDL_Color begin_,end_;
	Orientation orientation_;
public:
	Gradient();
	Gradient(const SDL_Color& begin,const SDL_Color& end,Orientation orientation);
	virtual ~Gradient();

	void setBeginColor(const SDL_Color& color);
	void setEndColor(const SDL_Color& color);
	void setOrientation(Orientation orientation);

	void paint(SDL_Surface* surface,const SDL_Rect* rect=0);
};

}

#endif
