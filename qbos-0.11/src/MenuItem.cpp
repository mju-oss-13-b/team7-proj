/*
 * QBOS: MenuItem.cpp
 *
 * Un item del menu principal.
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
#include "MenuItem.h"
#include "tge/Util.h"

using std::exception;
using std::runtime_error;
using tge::Util;



MenuItem::MenuItem(int x,int y,int z,bool enabled
		,string eimg,string dimg):SurfaceWidget(x,y,0,0,z)
{
	enabled_=enabled;

	eimg_=0;
	dimg_=0;
	try
	{
		eimg_=Util::loadImage(eimg,true);
		dimg_=Util::loadImage(dimg,true);
	}
	catch(const exception& ex)
	{
		if(eimg_)
			SDL_FreeSurface(eimg_);
		throw runtime_error(string("MenuItem::MenuItem(...): ")+ex.what());
	}

	setWidth(dimg_->w);
	setHeight(dimg_->h);
}
MenuItem::~MenuItem()
{
	SDL_FreeSurface(eimg_);
	SDL_FreeSurface(dimg_);
}

void MenuItem::setEnabled(bool enabled)
{
	if(enabled_!=enabled)
		setUpdate(true);
	enabled_=enabled;
}



//Implementacion metodo abstracto tge::SurfaceWidget

SDL_Surface* MenuItem::getSurface()
{
	return enabled_?eimg_:dimg_;
}

