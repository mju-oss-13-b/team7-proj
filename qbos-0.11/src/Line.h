/*
 * QBOS: Line.h
 *
 * Clase que representa una linea. Es un Widget que se hace
 * transparente con cierta rapidez.
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

#ifndef _LINE_H
#define _LINE_H

#include "SafeMatrix.h"
#include "tge/SurfaceWidget.h"
#include "tge/Rapidity.h"

using tge::SurfaceWidget;
using tge::Rapidity;


class Line:public SurfaceWidget
{
	static Rapidity* fade_rapidity_;

	SDL_Surface* surface_;
	int ticks_,alpha_;
protected:
	SDL_Surface* getSurface();
public:
	Line(int row,int z,SafeMatrix& matrix,SDL_Surface* (&blocks)[5]);
	~Line();

	bool dead();
	void ticks(int t);

	static void setFadeRapidity(Rapidity* fade_rapidity);
};

#endif
