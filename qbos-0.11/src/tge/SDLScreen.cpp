/*
 * QBOS: tge/SDLScreen.cpp
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

#include <stdexcept>
#include <SDL/SDL_ttf.h>
#include "SDLScreen.h"
#include "Cursors.h"

using std::runtime_error;



tge::SDLScreen::SDLScreen(int w,int h,int bpp)
{

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE)<0)
		throw runtime_error("SDLScreen::initSDL: no se pudo iniciar SDL");
	w_=w;
	h_=h;
	screen_=SDL_SetVideoMode(w,h,bpp,0);
	if(!screen_)
		throw runtime_error("SDLScreen::initSDL: no se pudo iniciar el modo de video");

	if(TTF_Init())
		throw runtime_error("SDLScreen::initSDL: no se pudo iniciar SDL_ttf");

	Cursors::init();
}

tge::SDLScreen::~SDLScreen()
{
	Cursors::destroy();
	if(TTF_WasInit())
		TTF_Quit();
	if(SDL_WasInit(SDL_INIT_EVERYTHING))
		SDL_Quit();
}


SDL_Surface* tge::SDLScreen::getScreen()
{
	return screen_;
}

int tge::SDLScreen::getWidth()
{
	return w_;
}

int tge::SDLScreen::getHeight()
{
	return h_;
}

void tge::SDLScreen::setTitle(string title)
{
	SDL_WM_SetCaption(title.c_str(),0);
}

