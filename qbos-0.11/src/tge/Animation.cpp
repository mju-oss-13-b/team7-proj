/*
 * QBOS: tge/Animation.cpp
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

#include "Animation.h"



tge::Animation::Animation(int x,int y,int w,int h,int z,Type type
		,vector<SDL_Surface*>* surfaces,Rapidity& rapidity):SurfaceWidget(x,y,w,h,z)
{
	surfaces_=surfaces;
	rapidity_=rapidity;
	index_=ticks_=0;
	type_=type;
	di_=1;
}

tge::Animation::~Animation()
{
	vector<SDL_Surface*>::iterator i;
	for(i=surfaces_->begin();i!=surfaces_->end();i++)
		SDL_FreeSurface(*i);
	delete surfaces_;
}


void tge::Animation::ticks(int t)
{
	ticks_+=t;
	int delta=rapidity_.calcUnits(ticks_);
	if(type_==FIFO)
	{
		index_+=delta;
		index_%=surfaces_->size();
	}
	else
	{
		index_+=di_*(delta%surfaces_->size());
		if(index_<0)
		{
			di_=1;
			index_=0;
		}
		else if((Uint32)index_>=surfaces_->size())
		{
			di_=-1;
			index_=surfaces_->size()-1;
		}
	}
	setUpdate(true);
}



//Implementacion tge::SurfaceWidget::getSurface

SDL_Surface* tge::Animation::getSurface()
{
	return surfaces_->at(index_);
}

