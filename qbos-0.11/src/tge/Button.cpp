/*
 * QBOS: tge/Button.cpp
 *
 * Un boton :p
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

#include <stdexcept>
#include "Button.h"
#include "Gradient.h"
#include "Util.h"

#define MARGIN_X 4
#define MARGIN_Y 2
#define FONT "fonts/vera.ttf"
#define FONT_SIZE 12

using std::exception;
using std::runtime_error;



tge::Button::Button(int x,int y,int z):SurfaceWidget(x,y,0,0,z)
{
	lb_normal_=lb_mouseover_=0;
	bkg_normal_=bkg_mouseover_=bkg_clicked_=0;
	state_=NORMAL;
	//DEBES llamar a setText despues de contruir el boton
}

tge::Button::~Button()
{
	if(lb_normal_)
		delete lb_normal_;
	if(lb_mouseover_)
		delete lb_mouseover_;
	if(bkg_normal_)
		SDL_FreeSurface(bkg_normal_);
	if(bkg_mouseover_)
		SDL_FreeSurface(bkg_mouseover_);
	if(bkg_clicked_)
		SDL_FreeSurface(bkg_clicked_);
}


void tge::Button::setText(string text)
{
	try
	{
		if(lb_normal_)
			lb_normal_->setText(text);
		else
		{
			SDL_Color c={220,220,220,0};
			lb_normal_=new Label(MARGIN_X,MARGIN_Y,1,FONT,FONT_SIZE,c,Label::BLENDED);
			lb_normal_->setText(text);
		}
		if(lb_mouseover_)
			lb_mouseover_->setText(text);
		else
		{
			SDL_Color c={240,240,240,0};
			lb_mouseover_=new Label(MARGIN_X,MARGIN_Y,1,FONT,FONT_SIZE,c,Label::BLENDED);
			lb_mouseover_->setText(text);
		}
		setWidth(2*MARGIN_X+lb_normal_->getWidth());
		setHeight(2*MARGIN_Y+lb_normal_->getHeight());

		if(bkg_normal_)
			SDL_FreeSurface(bkg_normal_);
		bkg_normal_=Util::createSurface(getWidth(),getHeight());
		SDL_Color begin1={70,70,70,0};
		SDL_Color end1={0,0,0,0};
		Gradient gd(begin1,end1,Gradient::VERTICAL);
		gd.paint(bkg_normal_);
		lb_normal_->paint(bkg_normal_);

		if(bkg_mouseover_)
			SDL_FreeSurface(bkg_mouseover_);
		bkg_mouseover_=Util::createSurface(getWidth(),getHeight());
		SDL_Color begin2={80,80,80,0};
		SDL_Color end2={10,10,10,0};
		gd.setBeginColor(begin2);
		gd.setEndColor(end2);
		gd.paint(bkg_mouseover_);
		lb_mouseover_->paint(bkg_mouseover_);

		if(bkg_clicked_)
			SDL_FreeSurface(bkg_clicked_);
		bkg_clicked_=Util::createSurface(getWidth(),getHeight());
		SDL_Color begin3={10,10,10,0};
		SDL_Color end3={80,80,80,0};
		gd.setBeginColor(begin3);
		gd.setEndColor(end3);
		gd.paint(bkg_clicked_);
		lb_mouseover_->paint(bkg_clicked_);

		setUpdate(true);
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("tge::Button::setText(...): ")+ex.what());
	}
}

void tge::Button::setState(State state)
{
	if(state_!=state)
	{
		state_=state;
		setUpdate(true);
	}
}



//Implementacion metodo abstracto SurfaceWidget

SDL_Surface* tge::Button::getSurface()
{
	switch(state_)
	{
		case MOUSEOVER:
			return bkg_mouseover_;
		case CLICKED:
			return bkg_clicked_;
		default:
			return bkg_normal_;
	}
}

