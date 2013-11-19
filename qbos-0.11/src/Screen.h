/*
 * QBOS: Screen.h
 *
 * Clase base para derivar "pantallas".
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

#ifndef _SCREEN_H
#define _SCREEN_H

#include <string>
#include <list>
#include "tge/SDLScreen.h"
#include "tge/Uta.h"
#include "tge/Widget.h"

using std::string;
using std::list;
using tge::SDLScreen;
using tge::Uta;
using tge::Widget;


class Screen
{
	SDL_Surface* bkg_;
	Uta* uta_;
	list<Widget*> widgets_;
	SDLScreen* sdl_screen_;
protected:
	void addWidget(Widget* widget);
	void removeWidget(Widget* widget);
	SDL_Surface* getBackground();
	void setBackground(SDL_Surface* surface);
	void refreshRect(const SDL_Rect& rect);

	int getWidth();
	int getHeight();
	SDL_Surface* getScreen();
	SDLScreen* getSDLScreen();
	string getHomeFile(string file);
public:
	Screen(SDLScreen* sdl_screen);
	virtual ~Screen();

	virtual void update();

	virtual void ticks(int t)=0;
	virtual Screen* processEvents(bool& quit)=0;

	void setTitle(string title);
};

#endif
