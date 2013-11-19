/*
 * QBOS: tge/Brush.h
 *
 * Una "brocha" para pintar fondos, encapsula un color o una
 * pequeña surface que sirve como patron de relleno.
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

#ifndef TGE_BRUSH_H
#define TGE_BRUSH_H

#include <SDL/SDL.h>


namespace tge
{

class Brush
{
	bool use_color_;
	union
	{
		SDL_Color color_;
		SDL_Surface* tile_;
	};
public:
	Brush();
	Brush(const SDL_Color& color);
	Brush(SDL_Surface* tile); //Se adueña de surface
	Brush(const Brush& brush);
	virtual ~Brush();

	void paint(SDL_Surface* surface,const SDL_Rect* rect=0);

	Brush& operator=(const Brush& brush);
};

}

#endif
