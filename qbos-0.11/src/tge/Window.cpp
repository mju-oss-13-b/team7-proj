/*
 * QBOS: tge/Window.h
 *
 * Una ventana, un Widget contenedor de Widgets con fondo :p
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

#include <algorithm>
#include <stdexcept>
#include "Window.h"
#include "Util.h"

using std::exception;
using std::runtime_error;



tge::Window::Window(int x,int y,int w,int h,int z):Widget(x,y,w,h,z)
{
	try
	{
		surface_=Util::createSurface(w,h);
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("tge::Window(...): ")+ex.what());
	}
	bkg_update_=false;
}

tge::Window::~Window()
{
	list<Widget*>::iterator i;
	for(i=widgets_.begin();i!=widgets_.end();i++)
		delete *i;
	SDL_FreeSurface(surface_);
}


void tge::Window::setBackground(Brush& brush)
{
	use_brush_=true;
	brush_=brush;
	brush_.paint(surface_);
	bkg_update_=true;
	setUpdate(true);
}

void tge::Window::setBackground(Gradient& gradient)
{
	use_brush_=false;
	gradient_=gradient;
	gradient_.paint(surface_);
	bkg_update_=true;
	setUpdate(true);
}

void tge::Window::setBackgroundAlpha(Uint8 alpha)
{
	alpha_=alpha;
	SDL_SetAlpha(surface_,SDL_SRCALPHA|SDL_RLEACCEL,alpha_);
	bkg_update_=true;
	setUpdate(true);
}


void tge::Window::addWidget(Widget* widget)
{
	list<Widget*>::iterator i=upper_bound(widgets_.begin(),widgets_.end(),widget,LessZ());
	widgets_.insert(i,widget);
	setUpdate(true);
}

void tge::Window::removeWidget(Widget* widget)
{
	widgets_.remove(widget);
	setUpdate(true);
}



//Implementacion tge::Widget::getUpdateRects(...)

void tge::Window::getUpdateRects(Uta* uta,const SDL_Rect* parent_rect)
{
	bool child_update=false;
	list<Widget*>::iterator i;
	for(i=widgets_.begin();i!=widgets_.end();i++)
		if((*i)->getUpdate())
		{
			child_update=true;
			break;
		}
	if(getUpdate() || child_update)
	{
		if(bkg_update_)
		{
			if(parent_rect)
			{
				SDL_Rect rect;
				if(calcAbsoluteRect(getRect(),parent_rect,rect))
					uta->addRect(rect);
				if(calcAbsoluteRect(getLastPaintedRect(),parent_rect,rect))
					uta->addRect(rect);
			}
			else
			{
				uta->addRect(getRect());
				uta->addRect(getLastPaintedRect());
				bkg_update_=false;
			}
		}
		for(i=widgets_.begin();i!=widgets_.end();i++)
			(*i)->getUpdateRects(uta,&getRect());
		setUpdate(false);
	}
}


//Implementacion tge::Widget::paint(...)

void tge::Window::paint(SDL_Surface* screen,const SDL_Rect* parent_rect)
{
	if(parent_rect)
	{
		SDL_Rect pos,dim;
		if(calcAbsolutePositionAndDimension(getRect(),parent_rect,pos,dim))
		{
			SDL_Rect new_parent;
			new_parent.x=pos.x;
			new_parent.y=pos.y;
			new_parent.w=dim.w;
			new_parent.h=dim.h;
			SDL_BlitSurface(surface_,&dim,screen,&pos);
			list<Widget*>::iterator i;
			for(i=widgets_.begin();i!=widgets_.end();i++)
				(*i)->paint(screen,&new_parent);
		}
	}
	else
	{
		SDL_Rect rect=getRect();
		SDL_BlitSurface(surface_,0,screen,&rect);
		list<Widget*>::iterator i;
		for(i=widgets_.begin();i!=widgets_.end();i++)
			(*i)->paint(screen,&getRect());
	}
	updateLastPaintedRect();
}

