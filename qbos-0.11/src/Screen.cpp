/*
 * QBOS: Screen.cpp
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

#include <algorithm>
#include <cstdlib>
#include "Screen.h"
#include "constants.h"
#include "tge/SDLScreen.h"

using tge::SDLScreen;
using tge::LessZ;



Screen::Screen(SDLScreen* sdl_screen)
{
	sdl_screen_=sdl_screen;

	uta_=new Uta(getWidth(),getHeight());
	SDL_Rect rect;
	rect.x=rect.y=0;
	rect.w=getWidth();
	rect.h=getHeight();
	uta_->addRect(rect);

	bkg_=0;
}
Screen::~Screen()
{
	list<Widget*>::iterator i;
	for(i=widgets_.begin();i!=widgets_.end();i++)
		delete *i;
	SDL_FreeSurface(bkg_);
	delete uta_;
}

void Screen::update()
{
	list<Widget*>::iterator w;
	for(w=widgets_.begin();w!=widgets_.end();w++)
		(*w)->getUpdateRects(uta_);

	pair<int,SDL_Rect*> rects=uta_->getRects();
	if(rects.first)
	{
		for(int i=0;i<rects.first;i++)
		{
			SDL_SetClipRect(getScreen(),&(rects.second)[i]);
			SDL_BlitSurface(bkg_,0,getScreen(),0);
			for(w=widgets_.begin();w!=widgets_.end();w++)
				(*w)->paint(getScreen());
		}
		SDL_UpdateRects(getScreen(),rects.first,rects.second);
		free(rects.second);
		uta_->clear();
	}
}

void Screen::setTitle(string title)
{
	sdl_screen_->setTitle(title);
}



//Metodos protegidos

void Screen::addWidget(Widget* widget)
{
	list<Widget*>::iterator i=upper_bound(widgets_.begin(),widgets_.end(),widget,LessZ());
	widgets_.insert(i,widget);
}

void Screen::removeWidget(Widget* widget)
{
	widgets_.remove(widget);
}


SDL_Surface* Screen::getScreen()
{
	return sdl_screen_->getScreen();
}

SDLScreen* Screen::getSDLScreen()
{
	return sdl_screen_;
}

int Screen::getWidth()
{
	return sdl_screen_->getWidth();
}

int Screen::getHeight()
{
	return sdl_screen_->getHeight();
}

SDL_Surface* Screen::getBackground()
{
	return bkg_;
}

void Screen::setBackground(SDL_Surface* surface)
{
	if(bkg_)
		SDL_FreeSurface(bkg_);
	bkg_=surface;
}

void Screen::refreshRect(const SDL_Rect& rect)
{
	uta_->addRect(rect);
}

string Screen::getHomeFile(string file)
{
	return  (string)getenv("HOME")+"/.qbos/"+file;
}

