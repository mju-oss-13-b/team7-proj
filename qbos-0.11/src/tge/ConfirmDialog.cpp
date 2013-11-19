/*
 * QBOS: tge/ConfirmDialog.cpp
 *
 * Dialogo con si/no
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
#include "ConfirmDialog.h"
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



tge::ConfirmDialog::ConfirmDialog(int flags,string text
		,int x,int y,int w,int h,int z):Window(x,y,w,h,z)
{
	Label* lb=0;
	btn_yes_=0;
	btn_no_=0;
	SDL_Surface* tile=0;

	try
	{
		tile=Util::loadImage("images/dlgtile.png");
		Brush brush=Brush(tile);
		setBackground(brush);

		setBackgroundAlpha(128);

		SDL_Color c={235,235,235,0};
		lb=new Label(MARGIN,MARGIN,1,FONT,FONT_SIZE,c,Label::BLENDED);
		lb->setText(text);

		btn_no_=new Button(0,0,1);
		btn_no_->setText("No");
		btn_no_->setX(w-MARGIN-btn_no_->getWidth());
		btn_no_->setY(h-MARGIN-btn_no_->getHeight());
		no_rect_=btn_no_->getRect();
		no_rect_.x+=getX();
		no_rect_.y+=getY();

		btn_yes_=new Button(0,0,1);
		btn_yes_->setText("Si");
		btn_yes_->setX(btn_no_->getX()-MARGIN-btn_yes_->getWidth());
		btn_yes_->setY(btn_no_->getY());
		yes_rect_=btn_yes_->getRect();
		yes_rect_.x+=getX();
		yes_rect_.y+=getY();
	}
	catch(const exception& ex)
	{
		if(lb)
			delete lb;
		if(btn_yes_)
			delete btn_yes_;
		if(btn_no_)
			delete btn_no_;
		throw runtime_error(string("tge::ConfirmDialog::ConfirmDialog(...): ")+ex.what());
	}
	addWidget(lb);
	addWidget(btn_yes_);
	addWidget(btn_no_);
	flags_=flags;
}

tge::ConfirmDialog::ConfirmDialog(int flags,vector<string>& text
		,int x,int y,int w,int h,int z):Window(x,y,w,h,z)
{
	list<Label*> labels;
	btn_yes_=0;
	btn_no_=0;
	SDL_Surface* tile=0;

	try
	{
		tile=Util::loadImage("images/dlgtile.png");
		Brush brush=Brush(tile);
		setBackground(brush);

		setBackgroundAlpha(150);

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

		btn_no_=new Button(0,0,1);
		btn_no_->setText("No");
		btn_no_->setX(w-MARGIN-btn_no_->getWidth());
		btn_no_->setY(h-MARGIN-btn_no_->getHeight());
		no_rect_=btn_no_->getRect();
		no_rect_.x+=getX();
		no_rect_.y+=getY();

		btn_yes_=new Button(0,0,1);
		btn_yes_->setText("Si");
		btn_yes_->setX(btn_no_->getX()-MARGIN-btn_yes_->getWidth());
		btn_yes_->setY(btn_no_->getY());
		yes_rect_=btn_yes_->getRect();
		yes_rect_.x+=getX();
		yes_rect_.y+=getY();
	}
	catch(const exception& ex)
	{
		list<Label*>::iterator k;
		for(k=labels.begin();k!=labels.end();k++)
			delete *k;
		if(btn_yes_)
			delete btn_yes_;
		if(btn_no_)
			delete btn_no_;
		throw runtime_error(string("tge::ConfirmDialog::ConfirmDialog(...): ")+ex.what());
	}
	list<Label*>::iterator k;
	for(k=labels.begin();k!=labels.end();k++)
		addWidget(*k);
	addWidget(btn_yes_);
	addWidget(btn_no_);
	flags_=flags;
}

tge::ConfirmDialog::~ConfirmDialog()
{
	SDL_SetCursor(Cursors::getDefaultCursor());
}


int tge::ConfirmDialog::getFlags()
{
	return flags_;
}

void tge::ConfirmDialog::ticks(int t)
{
}

tge::ConfirmDialog::ActionCode tge::ConfirmDialog::processEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					return NO;
				case SDLK_RETURN:
					return YES;
				default:
					break;
			}
		}
		else if(event.type==SDL_MOUSEMOTION)
		{
			if(Util::pointInRect(event.motion.x,event.motion.y,yes_rect_))
			{
				btn_yes_->setState(Button::MOUSEOVER);
				SDL_SetCursor(Cursors::getHandCursor());
			}
			else if(Util::pointInRect(event.motion.x,event.motion.y,no_rect_))
			{
				btn_no_->setState(Button::MOUSEOVER);
				SDL_SetCursor(Cursors::getHandCursor());
			}
			else
			{
				btn_yes_->setState(Button::NORMAL);
				btn_no_->setState(Button::NORMAL);
				SDL_SetCursor(Cursors::getDefaultCursor());
			}
		}
		else if(event.type==SDL_MOUSEBUTTONUP)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
			{
				if(Util::pointInRect(event.button.x,event.button.y,yes_rect_))
				{
					btn_yes_->setState(Button::MOUSEOVER);
					return YES;
				}
				else if(Util::pointInRect(event.button.x,event.button.y,no_rect_))
				{
					btn_no_->setState(Button::MOUSEOVER);
					return NO;
				}
			}
		}
		else if(event.type==SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
			{
				if(Util::pointInRect(event.button.x,event.button.y,yes_rect_))
					btn_yes_->setState(Button::CLICKED);
				else if(Util::pointInRect(event.button.x,event.button.y,no_rect_))
					btn_no_->setState(Button::CLICKED);
			}
		}
	return NOTHING;
}

