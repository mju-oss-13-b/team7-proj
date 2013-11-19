/*
 * QBOS: tge/Label.cpp
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

#include <stdexcept>
#include "Label.h"
#include "Util.h"

using std::runtime_error;



tge::Label::Label(int x,int y,int z,const string& font,int size
		,SDL_Color& color,RenderType render_type):SurfaceWidget(x,y,0,0,z)
{
	font_=TTF_OpenFont(font.c_str(),size);
	if(!font_)
		throw runtime_error(string("tge::Label::Label(...): ")+TTF_GetError());
	color_=color;
	render_type_=render_type;
	text_="";
	surface_=0;
}

tge::Label::~Label()
{
	TTF_CloseFont(font_);
	if(surface_)
		SDL_FreeSurface(surface_);
}

void tge::Label::setText(const string& text)
{
	if(text_==text)
		return;

	if(!Util::trim(text).size())
	{
		surface_=0;
		return;
	}

	SDL_Surface* tmp=0;
	switch(render_type_)
	{
		case SOLID:
			tmp=TTF_RenderUTF8_Solid(font_,text.c_str(),color_);
			break;
		case SHADED: //areglar para poner el otro color
			tmp=TTF_RenderUTF8_Shaded(font_,text.c_str(),color_,color_);
			break;
		case BLENDED:
			tmp=TTF_RenderUTF8_Blended(font_,text.c_str(),color_);
			break;
		default:
			throw runtime_error("tge::Label::setText(...): [-1] error render_type_");
			break;
	}
	if(!tmp)
		throw runtime_error(string("tge::Label::setText(...): [0] ")+TTF_GetError());
	else
	{
		SDL_Surface* tmp2=SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);
		if(!tmp2)
			throw runtime_error(string("tge::Label::setText(...): [1] ")+SDL_GetError());
		if(surface_)
			SDL_FreeSurface(surface_);
		surface_=tmp2;

		setWidth(surface_->w);
		setHeight(surface_->h);
		text_=text;
		setUpdate(true);
	}
}

const string& tge::Label::getText()
{
	return text_;
}



//Implementacion de tge::SurfaceWidget::getSurface()

SDL_Surface* tge::Label::getSurface()
{
	return surface_;
}
