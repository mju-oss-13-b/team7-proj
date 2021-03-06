/*
 * QBOS: tge/SDLScreen.h
 *
 * Clase SDLScreen, que representa a la pantalla. Tiene sus dimensiones
 * y el framebuffer surface.
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

#ifndef TGE_SDLSCREEN_H
#define TGE_SDLSCREEN_H

#include <string>
#include <SDL/SDL.h>

using std::string;


namespace tge
{

class SDLScreen
{
	int w_,h_;
	SDL_Surface* screen_;
public:
	SDLScreen(int w,int h,int bpp);
	~SDLScreen();

	SDL_Surface* getScreen();
	int getWidth();
	int getHeight();
	void setTitle(string title);
};

}

#endif
