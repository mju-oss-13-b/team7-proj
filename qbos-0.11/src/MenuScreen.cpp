/*
 * QBOS: MenuScreen.cpp
 *
 * Pantalla inicial con el menu principal.
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
#include "MenuScreen.h"
#include "GameScreen.h"
#include "SetupScreen.h"
#include "HighScoresScreen.h"
#include "CreditsScreen.h"
#include "tge/Util.h"
#include "tge/Cursors.h"

using std::exception;
using std::runtime_error;
using tge::Util;
using tge::Cursors;



int MenuScreen::item_=0;

MenuScreen::MenuScreen(SDLScreen* sdl_screen):Screen(sdl_screen)
{
	SDL_Surface* bkg=0;
	for(int i=0;i<4;i++)
		items_[i]=0;
	arrow_=0;
	lb_totexcl_=0;

	try
	{
		bkg=Util::loadImage("images/bkg_menu.png");

		items_[0]=new MenuItem(78,132,1,true
				,"images/play1.png","images/play0.png");
		items_[1]=new MenuItem(160,220,1,false
				,"images/setup1.png","images/setup0.png");
		items_[2]=new MenuItem(112,316,1,false
				,"images/highscores1.png","images/highscores0.png");
		items_[3]=new MenuItem(66,436,1,false
				,"images/credits1.png","images/credits0.png");

		arrow_=new MenuArrow(0,0,1,"images/arrow.png");

		SDL_Color color;
		color.r=230;
		color.g=230;
		color.b=230;
		lb_totexcl_=new Label(0,0,1,"fonts/vera.ttf",12,color,tge::Label::BLENDED);
		lb_totexcl_->setText("www.totex.cl");
	}
	catch(const exception& ex)
	{
		if(bkg)
			SDL_FreeSurface(bkg);
		for(int i=0;i<4;i++)
			if(items_[i])
				delete items_[i];
		if(arrow_)
			delete arrow_;
		if(lb_totexcl_)
			delete lb_totexcl_;

		throw runtime_error(string("MenuScreen::MenuScreen(...): ")+ex.what());
	}

	setBackground(bkg);

	addWidget(items_[0]);
	addWidget(items_[1]);
	addWidget(items_[2]);
	addWidget(items_[3]);

	arrow_->setPosition(items_[item_]->getX()
			,items_[item_]->getY()+(items_[item_]->getHeight()-arrow_->getHeight())/2);
	addWidget(arrow_);

	lb_totexcl_->setX(5);
	lb_totexcl_->setY(getHeight()-lb_totexcl_->getHeight()-3);
	addWidget(lb_totexcl_);
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::ticks(int t)
{
	arrow_->ticks(t);
}

Screen* MenuScreen::processEvents(bool& quit)
{
	SDL_Event event;
	int ex_item;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_DOWN:
					ex_item=item_;
					item_++;
					item_%=4;
					refreshItems(ex_item);
					break;
				case SDLK_UP:
					ex_item=item_;
					item_--;
					if(item_==-1)
						item_=3;
					item_%=4;
					refreshItems(ex_item);
					break;
				case SDLK_RETURN:
					try
					{
						SDL_SetCursor(Cursors::getDefaultCursor());
						return makeNextScreen();
					}
					catch(const exception& ex)
					{
						throw runtime_error(string("MenuScreen::processEvents(...):")+ex.what());
					}
				case SDLK_ESCAPE:
					quit=true;
					return 0;
				default:
					break;
			}
		}
		else if(event.type==SDL_MOUSEMOTION)
		{
			int i;
			for(i=0;i<4;i++)
				if(Util::pointInRect(event.motion.x,event.motion.y,items_[i]->getRect()))
				{
					SDL_SetCursor(Cursors::getHandCursor());
					if(i!=item_)
					{
						ex_item=item_;
						item_=i;
						refreshItems(ex_item);
					}
					break;
				}
			if(i==4)
				SDL_SetCursor(Cursors::getDefaultCursor());
		}
		else if(event.type==SDL_MOUSEBUTTONUP)
		{
			if(event.button.button==SDL_BUTTON_LEFT)
				for(int i=0;i<4;i++)
					if(Util::pointInRect(event.button.x,event.button.y,items_[i]->getRect()))
					{
						item_=i;
						try
						{
							SDL_SetCursor(Cursors::getDefaultCursor());
							return makeNextScreen();
						}
						catch(const exception& ex)
						{
							throw runtime_error(string("MenuScreen::processEvents(...):")+ex.what());
						}
					}
		}
		else if(event.type==SDL_QUIT)
			quit=true;
	return 0;
}



//Metodos privados

Screen* MenuScreen::makeNextScreen()
{
	try
	{
		switch(item_)
		{
			case 0:
				return new GameScreen(getSDLScreen());
			case 1:
				return new SetupScreen(getSDLScreen());
			case 2:
				return new HighScoresScreen(getSDLScreen());
			case 3:
				return new CreditsScreen(getSDLScreen());
			default:
				throw runtime_error("item_ invalido");
		}
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("MenuScreen::makeNextScreen(...): ")+ex.what());
	}
}

void MenuScreen::refreshItems(int ex_item)
{
	items_[ex_item]->setEnabled(false);
	items_[item_]->setEnabled(true);

	arrow_->setPosition(items_[item_]->getX()
			,items_[item_]->getY()+(items_[item_]->getHeight()-arrow_->getHeight())/2);
}

