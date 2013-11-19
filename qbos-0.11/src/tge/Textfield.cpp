/*
 * QBOS: tge/Textfield.cpp
 *
 * Un campo de texto :p
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
#include "Textfield.h"
#include "Util.h"
#include "Gradient.h"

#define MARGIN_X 2
#define MARGIN_Y 2
#define CURSOR_DX 3
#define CURSOR_W 1
#define CURSOR_H 12
#define FONT "fonts/vera.ttf"
#define FONT_SIZE 12
#define TICKS 15
#define MAX_SIZE 50

using std::exception;
using std::runtime_error;



tge::Textfield::Textfield(int x,int y,int w,int h,int z):Widget(x,y,w,h,z)
{
	lb_=0;
	bkg_=0;
	try
	{
		SDL_Color c={240,240,240,0};
		lb_=new Label(MARGIN_X,MARGIN_Y,1,FONT,FONT_SIZE,c,Label::BLENDED);
		lb_->setText("");

		bkg_=Util::createSurface(getWidth(),getHeight());
		SDL_Color begin={0,0,0,0};
		SDL_Color end={20,20,20,0};
		Gradient gd(begin,end,Gradient::VERTICAL);
		gd.paint(bkg_);
		SDL_SetAlpha(bkg_,SDL_SRCALPHA|SDL_RLEACCEL,200);
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Textfield::Textfield(...): ")+ex.what());
	}
	state_=DISABLED;
	text_="";
	ticks_=0;
	cursor_state_=true;
	cursor_color_=SDL_MapRGB(bkg_->format,245,245,245);
	cursor_.y=getY()+(getHeight()-CURSOR_H)/2;
	cursor_.w=CURSOR_W;
	cursor_.h=CURSOR_H;
	text_update_=true;
	lb_rect_.x=getX()+MARGIN_X;
	lb_rect_.y=getY()+MARGIN_Y;
	lb_rect_.w=getWidth()-2*MARGIN_X;
	lb_rect_.h=getHeight()-2*MARGIN_Y;
}

tge::Textfield::~Textfield()
{
	if(lb_)
		delete lb_;
	if(bkg_)
		SDL_FreeSurface(bkg_);
}


const string& tge::Textfield::getText()
{
	return text_;
}

void tge::Textfield::setText(string text)
{
	try
	{
		text_=text;
		lb_->setText(text_);
		text_update_=false;
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Textfield::Textfield(...): ")+ex.what());
	}
}

void tge::Textfield::setState(State state)
{
	if(state_!=state)
	{
		state_=state;
		setUpdate(true);
	}
}

void tge::Textfield::key(SDLKey k)
{
	if(state_==ENABLED)
	{
		if(k==SDLK_BACKSPACE)
		{
			if(text_.size())
			{
				text_.erase(text_.size()-1,1);
				text_update_=true;
				setUpdate(true);
			}
		}
		else if(text_.size()<=MAX_SIZE)
		{
			switch(k)
			{
				case SDLK_SPACE:
					if(text_.size() && text_[text_.size()-1]!=' ')
						text_+=' ';
					break;
				case SDLK_a:
					text_+='A';
					break;
				case SDLK_b:
					text_+='B';
					break;
				case SDLK_c:
					text_+='C';
					break;
				case SDLK_d:
					text_+='D';
					break;
				case SDLK_e:
					text_+='E';
					break;
				case SDLK_f:
					text_+='F';
					break;
				case SDLK_g:
					text_+='G';
					break;
				case SDLK_h:
					text_+='H';
					break;
				case SDLK_i:
					text_+='I';
					break;
				case SDLK_j:
					text_+='J';
					break;
				case SDLK_k:
					text_+='K';
					break;
				case SDLK_l:
					text_+='L';
					break;
				case SDLK_m:
					text_+='M';
					break;
				case SDLK_n:
					text_+='N';
					break;
				case SDLK_o:
					text_+='O';
					break;
				case SDLK_p:
					text_+='P';
					break;
				case SDLK_q:
					text_+='Q';
					break;
				case SDLK_r:
					text_+='R';
					break;
				case SDLK_s:
					text_+='S';
					break;
				case SDLK_t:
					text_+='T';
					break;
				case SDLK_u:
					text_+='U';
					break;
				case SDLK_v:
					text_+='V';
					break;
				case SDLK_w:
					text_+='W';
					break;
				case SDLK_x:
					text_+='X';
					break;
				case SDLK_y:
					text_+='Y';
					break;
				case SDLK_z:
					text_+='Z';
					break;
				case SDLK_0:
					text_+='0';
					break;
				case SDLK_1:
					text_+='1';
					break;
				case SDLK_2:
					text_+='2';
					break;
				case SDLK_3:
					text_+='3';
					break;
				case SDLK_4:
					text_+='4';
					break;
				case SDLK_5:
					text_+='5';
					break;
				case SDLK_6:
					text_+='6';
					break;
				case SDLK_7:
					text_+='7';
					break;
				case SDLK_8:
					text_+='8';
					break;
				case SDLK_9:
					text_+='9';
					break;
				case SDLK_KP0:
					text_+='0';
					break;
				case SDLK_KP1:
					text_+='1';
					break;
				case SDLK_KP2:
					text_+='2';
					break;
				case SDLK_KP3:
					text_+='3';
					break;
				case SDLK_KP4:
					text_+='4';
					break;
				case SDLK_KP5:
					text_+='5';
					break;
				case SDLK_KP6:
					text_+='6';
					break;
				case SDLK_KP7:
					text_+='7';
					break;
				case SDLK_KP8:
					text_+='8';
					break;
				case SDLK_KP9:
					text_+='9';
					break;
				default:
					break;
			}
			text_update_=true;
			setUpdate(true);
		}
	}
}

void tge::Textfield::ticks(int t)
{
	if(state_==ENABLED)
	{
		ticks_+=t;
		if(ticks_>=TICKS)
		{
			ticks_=0;
			cursor_state_=!cursor_state_;
			setUpdate(true);
		}
	}
}



//Implementacion de tge::Widget::paint(...)

void tge::Textfield::paint(SDL_Surface* screen,const SDL_Rect* parent_rect)
{
	if(text_update_)
		try
		{
			lb_->setText(text_);
			text_update_=false;
		}
		catch(const exception& ex)
		{
			throw runtime_error(string("Textfield::paint(...): ")+ex.what());
		}
	if(parent_rect)
	{
		SDL_Rect pos,dim;
		if(calcAbsolutePositionAndDimension(getRect(),parent_rect,pos,dim))
			SDL_BlitSurface(bkg_,&dim,screen,&pos);
		if(calcAbsoluteRect(lb_rect_,parent_rect,pos))
		{
			SDL_Rect r1=pos;
			lb_->paint(screen,&r1);
			if(cursor_state_)
			{
				cursor_.x=parent_rect->x+getX()+lb_->getX()+CURSOR_DX;
				cursor_.y=parent_rect->y+getY()+(getHeight()-CURSOR_H)/2;
				if(text_.size())
					cursor_.x+=lb_->getWidth();
				if(Util::pointInRect(cursor_.x,cursor_.y,pos))
				{
					SDL_Rect rc=cursor_;
					SDL_FillRect(screen,&rc,cursor_color_);
				}
			}
		}
	}
	else
	{
		SDL_Rect rect=getRect();
		SDL_BlitSurface(bkg_,0,screen,&rect);
		lb_->paint(screen,&lb_rect_);
		if(cursor_state_)
		{
			cursor_.x=parent_rect->x+getX()+lb_->getX()+CURSOR_DX;
			cursor_.y=parent_rect->y+getY()+(getHeight()-CURSOR_H)/2;
			if(text_.size())
				cursor_.x+=lb_->getWidth();
			if(Util::pointInRect(cursor_.x,cursor_.y,getRect()))
			{
				SDL_Rect rc=cursor_;
				SDL_FillRect(screen,&rc,cursor_color_);
			}
		}
	}
	updateLastPaintedRect();
}

