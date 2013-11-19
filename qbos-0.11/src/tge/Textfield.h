/*
 * QBOS: tge/Textfield.h
 *
 * Un campo de texto :p
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

#ifndef TGE_TEXTFIELD_H
#define TGE_TEXTFIELD_H

#include <string>
#include <SDL/SDL.h>
#include "Widget.h"
#include "Label.h"

using std::string;


namespace tge
{

class Textfield:public Widget
{
public:
	enum State{ENABLED,DISABLED};
private:
	State state_;

	string text_;
	int ticks_;
	bool text_update_,cursor_state_;
	Uint32 cursor_color_;
	SDL_Rect cursor_;

	Label* lb_;
	SDL_Rect lb_rect_;

	SDL_Surface* bkg_;
public:
	Textfield(int x,int y,int w,int h,int z);
	virtual ~Textfield();

	const string& getText();
	void setText(string text);
	void setState(State state);
	void key(SDLKey k);
	void ticks(int t);

	void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0);
};

}

#endif
