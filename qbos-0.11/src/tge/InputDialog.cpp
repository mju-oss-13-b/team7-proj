/*
 * QBOS: tge/InputDialog.cpp
 *
 * Dialogo para ingresar texto
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
#include <algorithm>
#include "InputDialog.h"
#include "Label.h"
#include "Util.h"
#include "Cursors.h"

#define MARGIN 5
#define FONT "fonts/vera.ttf"
#define FONT_SIZE 14
#define SPACING 2

using std::exception;
using std::runtime_error;
using std::find;



tge::InputDialog::InputDialog(string text,int x,int y,int w,int h,int z,vector<int>* alphabet):Window(x,y,w,h,z)
{
	alphabet_=alphabet;

	textfield_=0;
	Label* lb=0;
	btn_ok_=0;
	btn_cancel_=0;
	SDL_Surface* tile;

	try
	{
		tile=Util::loadImage("images/dlgtile.png");
		Brush brush=Brush(tile);
		setBackground(brush);

		setBackgroundAlpha(128);

		SDL_Color c={235,235,235,0};
		lb=new Label(MARGIN,MARGIN,1,FONT,FONT_SIZE,c,Label::BLENDED);
		lb->setText(text);

		textfield_=new Textfield(MARGIN,lb->getY()+lb->getHeight()+MARGIN,w-2*MARGIN,20,1);
		textfield_->setState(Textfield::ENABLED);

		btn_cancel_=new Button(0,0,1);
		btn_cancel_->setText("No");
		btn_cancel_->setX(w-MARGIN-btn_cancel_->getWidth());
		btn_cancel_->setY(h-MARGIN-btn_cancel_->getHeight());
		cancel_rect_=btn_cancel_->getRect();
		cancel_rect_.x+=getX();
		cancel_rect_.y+=getY();

		btn_ok_=new Button(0,0,1);
		btn_ok_->setText("Si");
		btn_ok_->setX(btn_cancel_->getX()-MARGIN-btn_ok_->getWidth());
		btn_ok_->setY(btn_cancel_->getY());
		ok_rect_=btn_ok_->getRect();
		ok_rect_.x+=getX();
		ok_rect_.y+=getY();
	}
	catch(const exception& ex)
	{
		if(lb)
			delete lb;
		if(textfield_)
			delete textfield_;
		if(btn_ok_)
			delete btn_ok_;
		if(btn_cancel_)
			delete btn_cancel_;
		throw runtime_error(string("tge::InputDialog::InputDialog(...): ")+ex.what());
	}
	addWidget(lb);
	addWidget(textfield_);
	addWidget(btn_ok_);
	addWidget(btn_cancel_);
}

tge::InputDialog::~InputDialog()
{
	delete alphabet_;
	SDL_SetCursor(Cursors::getDefaultCursor());
}


const string& tge::InputDialog::getText()
{
	return textfield_->getText();
}

void tge::InputDialog::ticks(int t)
{
	textfield_->ticks(t);
}

tge::InputDialog::ActionCode tge::InputDialog::processEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					return CANCEL;
				case SDLK_RETURN:
					return OK;
				case SDLK_BACKSPACE:
					textfield_->key(event.key.keysym.sym);
					break;
				default:
					if(!alphabet_)
						textfield_->key(event.key.keysym.sym);
					else if(alphabet_ && find(alphabet_->begin()
								,alphabet_->end()
								,event.key.keysym.sym)!=alphabet_->end())
						textfield_->key(event.key.keysym.sym);
					break;
			}
		}
		else if(event.type==SDL_MOUSEMOTION)
		{
			if(Util::pointInRect(event.motion.x,event.motion.y,ok_rect_))
			{
				btn_ok_->setState(Button::MOUSEOVER);
				SDL_SetCursor(Cursors::getHandCursor());
			}
			else if(Util::pointInRect(event.motion.x,event.motion.y,cancel_rect_))
			{
				btn_cancel_->setState(Button::MOUSEOVER);
				SDL_SetCursor(Cursors::getHandCursor());
			}
			else
			{
				btn_ok_->setState(Button::NORMAL);
				btn_cancel_->setState(Button::NORMAL);
				SDL_SetCursor(Cursors::getDefaultCursor());
			}
		}
		else if(event.type==SDL_MOUSEBUTTONUP)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
			{
				if(Util::pointInRect(event.button.x,event.button.y,ok_rect_))
				{
					btn_ok_->setState(Button::MOUSEOVER);
					return OK;
				}
				else if(Util::pointInRect(event.button.x,event.button.y,cancel_rect_))
				{
					btn_cancel_->setState(Button::MOUSEOVER);
					return CANCEL;
				}
			}
		}
		else if(event.type==SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
			{
				if(Util::pointInRect(event.button.x,event.button.y,ok_rect_))
					btn_ok_->setState(Button::CLICKED);
				else if(Util::pointInRect(event.button.x,event.button.y,cancel_rect_))
					btn_cancel_->setState(Button::CLICKED);
			}
		}
	return NOTHING;
}

