/*
 * QBOS: MenuArrow.cpp
 *
 * Flechita del menu principal.
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
#include "MenuArrow.h"
#include "tge/Util.h"

#define BOX_W 76

using std::exception;
using std::runtime_error;
using tge::Util;



MenuArrow::MenuArrow(int x,int y,int z,string img):SurfaceWidget(x,y,0,0,z)
{
	try
	{
		img_=Util::loadImage(img,true);
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("MenuArrow::MenuArrow(...): ")+ex.what());
	}

	setWidth(img_->w);
	setHeight(img_->h);

	//Hay que llamar a setPosition inmediatamente despues del
	//constructor!
	xo_=xf_=0;
	dx_=-1;
	rapidity_.setRapidity(1,1);
	ticks_=0;
}
MenuArrow::~MenuArrow()
{
	SDL_FreeSurface(img_);
}

void MenuArrow::ticks(int t)
{
	ticks_+=t;
	int pix=rapidity_.calcUnits(ticks_);
	setX(getX()+dx_*pix);

	if(getX()<=xo_)
	{
		setX(xo_);
		dx_*=-1;
	}
	else if(getX()>=xf_)
	{
		setX(xf_);
		dx_*=-1;
	}
	setUpdate(true);
}

void MenuArrow::setPosition(int x,int y)
{
	xo_=x-BOX_W;
	xf_=xo_+BOX_W-getWidth();
	setX(x-BOX_W);
	setY(y);
	dx_=1;
	ticks_=0;
	setUpdate(true);
}



//Implementacion metodo abstracto tge::SurfaceWidget

SDL_Surface* MenuArrow::getSurface()
{
	return img_;
}

