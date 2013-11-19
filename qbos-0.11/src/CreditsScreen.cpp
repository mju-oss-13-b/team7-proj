/*
 * QBOS: CreditsScreen.cpp
 *
 * Pantalla con los creditos.
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
#include "CreditsScreen.h"
#include "MenuScreen.h"
#include "tge/Util.h"
#include "tge/Cursors.h"

using std::exception;
using std::runtime_error;
using tge::Util;
using tge::Cursors;



CreditsScreen::CreditsScreen(SDLScreen* sdl_screen):Screen(sdl_screen)
{
	SDL_Surface* bkg=0;
	lb_title_=0;
	lb_totexcl_=0;
	for(int i=0;i<7;i++)
		lb_text_[i]=0;
	for(int i=0;i<2;i++)
		lb_gpl_[i]=0;
	for(int i=0;i<3;i++)
		lb_dev_[i]=0;
	lb_mail1_=0;
	for(int i=0;i<4;i++)
		lb_testers_[i]=0;
	lb_mail1_=0;

	try
	{
		bkg=Util::loadImage("images/bkg_credits.png");

		SDL_Color c1={230,230,230,0};
		lb_totexcl_=new Label(0,0,1,"fonts/vera.ttf",12,c1,tge::Label::BLENDED);
		lb_totexcl_->setText("www.totex.cl");

		SDL_Color c2={28,224,24,0};
		lb_title_=new Label(0,0,1,"fonts/polosemi.ttf",40,c2,tge::Label::BLENDED);
		lb_title_->setText("Creditos");

		SDL_Color c3={240,240,240,0};
		for(int i=0;i<7;i++)
			lb_text_[i]=new Label(0,0,1,"fonts/vera.ttf",16,c3,tge::Label::BLENDED);
		lb_text_[0]->setText("QBOS es un simple clon de tetris, producto de mis ");
		lb_text_[1]->setText("horas de ocio. Es la realización \"en serio\" de un");
		lb_text_[2]->setText("trabajo anterior llamado \"TetriX\". En el futuro quiero");
		lb_text_[3]->setText("añadirle caracteristicas que lo hagan más entrenido");
		lb_text_[4]->setText("y menos \"clásico\". Esta version está dedicada a mi");
		lb_text_[5]->setText("prima Dany, por ser el único especimen sobre la faz");
		lb_text_[6]->setText("de la tierra que pesca en serio mis juegos chantas :)");

		for(int i=0;i<2;i++)
			lb_gpl_[i]=new Label(0,0,1,"fonts/vera.ttf",16,c3,tge::Label::BLENDED);
		lb_gpl_[0]->setText("QBOS es software libre, liberado bajo licencia GPL,");
		lb_gpl_[1]->setText("por favor ve el archivo COPYNG para más detalles.");

		for(int i=0;i<3;i++)
			lb_dev_[i]=new Label(0,0,1,"fonts/vera.ttf",16,c3,tge::Label::BLENDED);
		lb_dev_[0]->setText("Fondos por nocturnal-devil");
		lb_dev_[1]->setText("Menus y logo por Mr Gimp :p");
		lb_dev_[2]->setText("Diseño y programación por ToTeX (yo O_o)");

		SDL_Color c4={180,180,180,0};
		lb_mail1_=new Label(0,0,1,"fonts/vera.ttf",11,c4,tge::Label::BLENDED);
		lb_mail1_->setText("(sheharzad_arshad@hotmail.com)");

		for(int i=0;i<4;i++)
			lb_testers_[i]=new Label(0,0,1,"fonts/vera.ttf",16,c3,tge::Label::BLENDED);
		lb_testers_[0]->setText("Gracias a:");
		lb_testers_[1]->setText("Dany, Hasbun, Kamon, kkroto, Knight, Maliq,");
		lb_testers_[2]->setText("Rhene, Sami, SouceCode y demas personas");
		lb_testers_[3]->setText("que aportaron con testing y sugerencias.");
	}
	catch(const exception& ex)
	{
		if(bkg)
			SDL_FreeSurface(bkg);
		if(lb_totexcl_)
			delete lb_totexcl_;
		if(lb_title_)
			delete lb_title_;
		for(int i=0;i<4;i++)
			if(lb_text_[i])
				delete lb_text_[i];
		for(int i=0;i<2;i++)
			if(lb_gpl_[i])
				delete lb_gpl_[i];
		for(int i=0;i<3;i++)
			if(lb_dev_[i])
				delete lb_dev_[i];
		if(lb_mail1_)
			delete lb_mail1_;
		for(int i=0;i<4;i++)
			if(lb_testers_[i])
				delete lb_testers_[i];

		throw runtime_error(string("CreditsScreen::CreditsScreen(...): ")+ex.what());
	}

	setBackground(bkg);

	lb_totexcl_->setX(5);
	lb_totexcl_->setY(getHeight()-lb_totexcl_->getHeight()-3);
	addWidget(lb_totexcl_);
	lb_title_->setX((getWidth()-lb_title_->getWidth())/2);
	lb_title_->setY(10);
	addWidget(lb_title_);

	int ly=80;
	for(int i=0;i<7;i++)
	{
		lb_text_[i]->setX(10);
		lb_text_[i]->setY(ly);
		ly+=lb_text_[i]->getHeight()+5;
		addWidget(lb_text_[i]);
	}

	ly=lb_text_[6]->getY()+lb_text_[6]->getHeight()+20;
	for(int i=0;i<2;i++)
	{
		lb_gpl_[i]->setX(10);
		lb_gpl_[i]->setY(ly);
		ly+=lb_gpl_[i]->getHeight()+5;
		addWidget(lb_gpl_[i]);
	}

	ly=lb_gpl_[1]->getY()+lb_gpl_[1]->getHeight()+30;
	for(int i=0;i<3;i++)
	{
		lb_dev_[i]->setX(10);
		lb_dev_[i]->setY(ly);
		ly+=lb_dev_[i]->getHeight()+5;
		addWidget(lb_dev_[i]);
	}
	lb_mail1_->setX(lb_dev_[0]->getX()+lb_dev_[0]->getWidth()+3);
	lb_mail1_->setY(lb_dev_[0]->getY()+lb_dev_[0]->getHeight()-lb_mail1_->getHeight()-3);
	addWidget(lb_mail1_);

	ly=lb_dev_[2]->getY()+lb_dev_[2]->getHeight()+20;
	for(int i=0;i<4;i++)
	{
		lb_testers_[i]->setX(i?40:10);
		lb_testers_[i]->setY(ly);
		ly+=lb_testers_[i]->getHeight()+5;
		addWidget(lb_testers_[i]);
	}
}

CreditsScreen::~CreditsScreen()
{
}

void CreditsScreen::ticks(int t)
{
}

Screen* CreditsScreen::processEvents(bool& quit)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_DOWN:
					break;
				case SDLK_UP:
					break;
				case SDLK_RETURN:
					break;
				case SDLK_ESCAPE:
					try
					{
						return new MenuScreen(getSDLScreen());
					}
					catch(const exception& ex)
					{
						throw runtime_error(string("CreditsScreen::processEvents(...): ")+ex.what());
					}
				default:
					return 0;
					break;
			}
		}
		/*
		else if(event.type==SDL_MOUSEMOTION)
		{
		}
		else if(event.type==SDL_MOUSEBUTTONUP)
		{
		}
		*/
		else if(event.type==SDL_QUIT)
			quit=true;
	return 0;
}

