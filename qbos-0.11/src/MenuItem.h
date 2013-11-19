/*
 * QBOS: MenuItem.h
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

#ifndef _MENUITEM_H
#define _MENUITEM_H

#include <string>
#include "tge/SurfaceWidget.h"

using std::string;
using tge::SurfaceWidget;
using tge::Uta;


class MenuItem:public SurfaceWidget
{
	SDL_Surface* eimg_,*dimg_;
	bool enabled_;
protected:
	SDL_Surface* getSurface();
public:
	MenuItem(int x,int y,int z,bool enabled
			,string eimg,string dimg);
	~MenuItem();

	void setEnabled(bool enabled);
};

#endif
