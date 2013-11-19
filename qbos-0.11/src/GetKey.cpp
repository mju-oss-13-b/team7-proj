/*
 * QBOS: GetKey.cpp
 *
 * Dialogo para capturar una tecla en pantalla Setup
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

#include "GetKey.h"
#include "tge/Util.h"

using tge::Util;



GetKey::GetKey(int x,int y,int w,int h,int z):SimpleDialog("Presiona una tecla","Cancelar",x,y,w,h,z)
{
}

GetKey::GetKey(vector<string>& text,int x,int y,int w,int h,int z):SimpleDialog(text,"Cancelar",x,y,w,h,z)
{
}

GetKey::~GetKey()
{
}

int GetKey::processEvents()
{
	SDL_Event event;
	SDL_PumpEvents();
	while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYDOWN))==1)
		if(event.type==SDL_KEYDOWN)
		{
			if(event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_RETURN)
				return SDLK_ESCAPE;
			else
				return event.key.keysym.sym;
		}

	if(dynamic_cast<SimpleDialog*>(this)->processEvents())
		return SDLK_ESCAPE;
	else
		return -1;
}

