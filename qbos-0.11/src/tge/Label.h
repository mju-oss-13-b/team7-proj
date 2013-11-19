/*
 * QBOS: tge/Label.h
 *
 * Clase Label; un widget que muestra texto con cierta fuente,
 * tamaño y color.
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

#ifndef TGE_LABEL_H
#define TGE_LABEL_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include "SurfaceWidget.h"
#include "Uta.h"

using std::string;


namespace tge
{

class Label:public SurfaceWidget
{
public:
	enum RenderType{SOLID,SHADED,BLENDED};
private:
	string text_;
	SDL_Color color_;
	TTF_Font* font_;
	RenderType render_type_;
	SDL_Surface* surface_;
protected:
	SDL_Surface* getSurface();
public:
	Label(int x,int y,int z,const string& font,int size
			,SDL_Color& color,RenderType render_type);
	~Label();

	void setText(const string& text);
	const string& getText();
};

}

#endif
