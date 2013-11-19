/*
 * QBOS: tge/Animation.h
 *
 * Una animacion; es solo un conjunto de surfaces que
 * van cambiando con cierta rapidez de forma ciclica.
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

#ifndef TGE_ANIMATION_H
#define TGE_ANIMATION_H

#include <vector>
#include "SurfaceWidget.h"
#include "Uta.h"
#include "Rapidity.h"

using std::vector;


namespace tge
{

class Animation:public SurfaceWidget
{
public:
	enum Type{LIFO,FIFO};
private:
	vector<SDL_Surface*>* surfaces_;
	Rapidity rapidity_;
	int index_,ticks_,di_;
	Type type_;
protected:
	SDL_Surface* getSurface();
public:
	Animation(int x,int y,int w,int h,int z,Type type
			,vector<SDL_Surface*>* surfaces,Rapidity& rapidity);
	virtual ~Animation();

	void ticks(int t);
};

}

#endif
