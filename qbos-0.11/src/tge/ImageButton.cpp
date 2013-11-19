/*
 * QBOS: tge/ImageButton.cpp
 *
 * boton con una imagen
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

#include <stdexcept>
#include "ImageButton.h"

using std::exception;
using std::runtime_error;



tge::ImageButton::ImageButton(int x,int y,int z,SDL_Surface* normal,SDL_Surface* mouseover,SDL_Surface* clicked)
							:SurfaceWidget(x,y,normal->w,normal->h,z)
{
	normal_=normal;
	mouseover_=mouseover;
	clicked_=clicked;
	state_=NORMAL;
}

tge::ImageButton::~ImageButton()
{
	if(normal_)
		SDL_FreeSurface(normal_);
	if(mouseover_)
		SDL_FreeSurface(mouseover_);
	if(clicked_)
		SDL_FreeSurface(clicked_);
}


void tge::ImageButton::setState(State state)
{
	if(state_!=state)
	{
		state_=state;
		setUpdate(true);
	}
}



//Implementacion metodo abstracto SurfaceWidget

SDL_Surface* tge::ImageButton::getSurface()
{
	switch(state_)
	{
		case MOUSEOVER:
			return mouseover_;
		case CLICKED:
			return clicked_;
		default:
			return normal_;
	}
}

