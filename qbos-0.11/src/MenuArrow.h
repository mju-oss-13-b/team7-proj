/*
 * QBOS: MenuArrow.h
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

#ifndef _MENU_ARROW_H
#define _MENU_ARROW_H

#include <string>
#include "tge/SurfaceWidget.h"
#include "tge/Uta.h"
#include "tge/Rapidity.h"

using std::string;
using tge::SurfaceWidget;
using tge::Uta;
using tge::Rapidity;


class MenuArrow:public SurfaceWidget
{
	SDL_Surface* img_;
	int xo_,xf_,dx_;
	Rapidity rapidity_;
	int ticks_;
protected:
	SDL_Surface* getSurface();
public:
	MenuArrow(int x,int y,int z,string img);
	~MenuArrow();

	void ticks(int t);
	void setPosition(int x,int y);
};

#endif
