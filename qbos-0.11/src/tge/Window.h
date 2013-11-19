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

#ifndef TGE_WINDOW_H
#define TGE_WINDOW_H

#include <list>
#include "Widget.h"
#include "Uta.h"
#include "Brush.h"
#include "Gradient.h"

using std::list;


namespace tge
{

class Window:public Widget
{
	bool use_brush_;
	Brush brush_;
	Gradient gradient_;

	SDL_Surface* surface_;
	Uint8 alpha_;

	bool bkg_update_;
	list<Widget*> widgets_;
public:
	Window(int x,int y,int w,int h,int z);
	virtual ~Window();

	void setBackground(Brush& brush);
	void setBackground(Gradient& gradient);
	void setBackgroundAlpha(Uint8 alpha);

	void addWidget(Widget* widget);
	void removeWidget(Widget* widget);

	void getUpdateRects(Uta* uta,const SDL_Rect* parent_rect=0);
	void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0);
};

}

#endif
