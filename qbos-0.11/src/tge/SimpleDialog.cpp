/*
 * QBOS: tge/SimpleDialog.cpp
 *
 * Dialogo con un boton
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

#include <list>
#include <stdexcept>
#include "SimpleDialog.h"
#include "Label.h"
#include "Util.h"
#include "Cursors.h"

#define MARGIN 5
#define FONT "fonts/vera.ttf"
#define FONT_SIZE 14
#define SPACING 2

using std::list;
using std::exception;
using std::runtime_error;



tge::SimpleDialog::SimpleDialog(string text,string button
		,int x,int y,int w,int h,int z):Window(x,y,w,h,z)
{
	Label* lb=0;
	btn_=0;
	SDL_Surface* tile=0;
	try
	{
		tile=Util::loadImage("images/dlgtile.png");
		Brush brush=Brush(tile);
		setBackground(brush);

		setBackgroundAlpha(150);

		SDL_Color c={235,235,235,0};
		lb=new Label(MARGIN,MARGIN,1,FONT,FONT_SIZE,c,Label::BLENDED);
		lb->setText(text);

		btn_=new Button(0,0,1);
		btn_->setText(button);
		btn_->setX(w-MARGIN-btn_->getWidth());
		btn_->setY(h-MARGIN-btn_->getHeight());
		btn_rect_=btn_->getRect();
		btn_rect_.x+=getX();
		btn_rect_.y+=getY();
	}
	catch(const exception& ex)
	{
		if(lb)
			delete lb;
		if(btn_)
			delete btn_;
		throw runtime_error(string("tge::SimpleDialog::SimpleDialog(...): ")+ex.what());
	}
	addWidget(lb);
	addWidget(btn_);
}

tge::SimpleDialog::SimpleDialog(vector<string>& text,string button
		,int x,int y,int w,int h,int z):Window(x,y,w,h,z)
{
	list<Label*> labels;
	btn_=0;
	try
	{
		setBackgroundAlpha(128);

		SDL_Color c={235,235,235,0};
		int ly=MARGIN;
		vector<string>::iterator k;
		for(k=text.begin();k!=text.end();k++)
		{
			Label* lb=new Label(MARGIN,ly,1,FONT,FONT_SIZE,c,Label::BLENDED);
			labels.push_back(lb);
			lb->setText(*k);
			ly+=lb->getHeight()+SPACING;
		}

		btn_=new Button(0,0,1);
		btn_->setText(button);
		btn_->setX(w-MARGIN-btn_->getWidth());
		btn_->setY(h-MARGIN-btn_->getHeight());
		btn_rect_=btn_->getRect();
		btn_rect_.x+=getX();
		btn_rect_.y+=getY();
	}
	catch(const exception& ex)
	{
		list<Label*>::iterator k;
		for(k=labels.begin();k!=labels.end();k++)
			delete *k;
		if(btn_)
			delete btn_;
		throw runtime_error(string("tge::SimpleDialog::SimpleDialog(...): ")+ex.what());
	}
	list<Label*>::iterator k;
	for(k=labels.begin();k!=labels.end();k++)
		addWidget(*k);
	addWidget(btn_);
}

tge::SimpleDialog::~SimpleDialog()
{
	SDL_SetCursor(Cursors::getDefaultCursor());
}


void tge::SimpleDialog::ticks(int t)
{
}

bool tge::SimpleDialog::processEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					return true;
				case SDLK_RETURN:
					return true;
				default:
					break;
			}
		}
		else if(event.type==SDL_MOUSEMOTION)
		{
			if(Util::pointInRect(event.motion.x,event.motion.y,btn_rect_))
			{
				btn_->setState(Button::MOUSEOVER);
				SDL_SetCursor(Cursors::getHandCursor());
			}
			else
			{
				btn_->setState(Button::NORMAL);
				SDL_SetCursor(Cursors::getDefaultCursor());
			}
		}
		else if(event.type==SDL_MOUSEBUTTONUP)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
			{
				if(Util::pointInRect(event.button.x,event.button.y,btn_rect_))
				{
					btn_->setState(Button::MOUSEOVER);
					return true;
				}
			}
		}
		else if(event.type==SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
			{
				if(Util::pointInRect(event.button.x,event.button.y,btn_rect_))
					btn_->setState(Button::CLICKED);
			}
		}
	return false;
}

