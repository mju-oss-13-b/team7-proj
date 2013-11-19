/*
 * QBOS: tge/Widget.cpp
 *
 * Clase base abstracta para derivar todos los objetos que se
 * pintan en la pantalla usando el microtiles array.
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

#include "Widget.h"
#include "Util.h"



tge::Widget::Widget(int x,int y,int w,int h,int z)
{
	rect_.x=x;
	rect_.y=y;
	rect_.w=w;
	rect_.h=h;
	last_painted_rect_=rect_;
	z_=z;
	update_=true;
}

tge::Widget::~Widget()
{
}


const SDL_Rect& tge::Widget::getRect()
{
	return rect_;
}

int tge::Widget::getX()
{
	return rect_.x;
}

int tge::Widget::getY()
{
	return rect_.y;
}

int tge::Widget::getWidth()
{
	return rect_.w;
}

int tge::Widget::getHeight()
{
	return rect_.h;
}

int tge::Widget::getZ()
{
	return z_;
}

void tge::Widget::setX(int x)
{
	rect_.x=x;
}

void tge::Widget::setY(int y)
{
	rect_.y=y;
}

void tge::Widget::setWidth(int w)
{
	rect_.w=w;
}

void tge::Widget::setHeight(int h)
{
	rect_.h=h;
}

void tge::Widget::setZ(int z)
{
	z_=z;
}

bool tge::Widget::getUpdate()
{ 
	return update_;
}

void tge::Widget::getUpdateRects(Uta* uta,const SDL_Rect* parent_rect)
{
	if(getUpdate())
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
		}
		setUpdate(false);
	}
}


//Metodos protegidos

void tge::Widget::setUpdate(bool update)
{ 
	update_=update;
}

void tge::Widget::updateLastPaintedRect()
{
	last_painted_rect_=rect_;
}

const SDL_Rect& tge::Widget::getLastPaintedRect()
{
	return last_painted_rect_;
}

bool tge::Widget::calcAbsoluteRect(SDL_Rect rect,const SDL_Rect* parent_rect
						,SDL_Rect& absolute_position)
{
	SDL_Rect r=rect;
	r.x+=parent_rect->x;
	r.y+=parent_rect->y;
	return Util::intersectRects(*parent_rect,r,absolute_position);
}

bool tge::Widget::calcAbsolutePositionAndDimension(SDL_Rect rect,const SDL_Rect* parent_rect
						,SDL_Rect& position,SDL_Rect& dimension)
{
	position=rect;
	position.x+=parent_rect->x;
	position.y+=parent_rect->y;
	if(Util::intersectRects(*parent_rect,position,dimension))
	{
		//position.w y position.h NO son validos! Recuerda que
		//la intencion de este metodo es calcular src y dst para
		//SDL_BlitSurface(surface,src,screen,dst), donde
		//src=dimension y dst=position
		dimension.x=dimension.y=0;
		return true;
	}
	else
		return false;
}




//Clase LessZ

bool tge::LessZ::operator()(Widget* w1,Widget* w2)
{
	return w1->getZ()<w2->getZ();
}

