/*
 * QBOS: tge/Button.h
 *
 * Un boton :p
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

#ifndef TGE_BUTTON_H
#define TGE_BUTTON_H

#include <string>
#include "SurfaceWidget.h"
#include "Label.h"

using std::string;


namespace tge
{

class Button:public SurfaceWidget
{
public:
	enum State{NORMAL,MOUSEOVER,CLICKED};
private:
	State state_;
	Label* lb_normal_,*lb_mouseover_;
	SDL_Surface* bkg_normal_,*bkg_mouseover_,*bkg_clicked_;
protected:
	SDL_Surface* getSurface();
public:
	Button(int x,int y,int z);
	virtual ~Button();

	void setText(string text);
	void setState(State state);
};

}

#endif
