/*
 * QBOS: HighScoresScreen.cpp
 *
 * Pantalla de los mas altos puntajes.
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
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include "constants.h"
#include "HighScoresScreen.h"
#include "HighScore.h"
#include "MenuScreen.h"
#include "tge/Util.h"
#include "tge/Cursors.h"
#include "tge/Window.h"
#include "tge/Brush.h"
#include "tge/Label.h"

using std::exception;
using std::runtime_error;
using std::list;
using std::string;
using std::ostringstream;
using std::ifstream;
using std::ofstream;
using tge::Util;
using tge::Cursors;
using tge::Window;
using tge::Brush;
using tge::Label;



HighScoresScreen::HighScoresScreen(SDLScreen* sdl_screen):Screen(sdl_screen)
{
	SDL_Surface* bkg=0;
	lb_title_=0;
	lb_totexcl_=0;

	ifstream in;

	Brush* brushs[2];
	brushs[0]=brushs[1]=0;
	list<Window*> rows;
	Label* nick=0,*points=0;
	Window* row=0;

	try
	{
		bkg=Util::loadImage("images/bkg_highscores.png");

		SDL_Color c1={230,230,230,0};
		lb_totexcl_=new Label(0,0,1,"fonts/vera.ttf",12,c1,tge::Label::BLENDED);
		lb_totexcl_->setText("www.totex.cl");

		SDL_Color c2={110,150,230,0};
		lb_title_=new Label(0,0,1,"fonts/polosemi.ttf",40,c2,tge::Label::BLENDED);
		lb_title_->setText("Mejores Puntajes");

		in.open(getHomeFile("high_scores").c_str(),std::ios::binary);
		if(!in.is_open())
			throw runtime_error("no se pudo abrir archivo \"high_scores\"");
		list<HighScore> high_scores;
		while(in.good())
		{
			HighScore hs;
			hs.readFrom(in);
			if(!in.eof())
				high_scores.push_back(hs);
		}
		in.close();

		SDL_Color cr0={27,27,105,0},cr1={92,92,191,0};
		brushs[0]=new Brush(cr0);
		brushs[1]=new Brush(cr1);
		list<HighScore>::iterator i;
		int c=0,row_y=HS_ROW_Y0;
		ostringstream ostr;
		for(i=high_scores.begin();i!=high_scores.end();i++,c++)
		{
			nick=new Label(HS_ROW_MARGIN_X,HS_ROW_MARGIN_Y,1,"fonts/vera.ttf",16,c1,tge::Label::BLENDED);
			points=new Label(0,HS_ROW_MARGIN_Y,1,"fonts/vera.ttf",16,c1,tge::Label::BLENDED);
			nick->setText((*i).getNick());
			ostr<<(*i).getPoints();
			points->setText(ostr.str());
			ostr.str("");
			row=new Window(HS_ROW_X,row_y,getWidth()-2*HS_ROW_X,2*HS_ROW_MARGIN_Y+nick->getHeight(),1);
			row_y+=row->getHeight();
			row->setBackground(*brushs[c%2]);
			row->setBackgroundAlpha(100);
			row->addWidget(nick);
			row->addWidget(points);
			points->setX(row->getWidth()-points->getWidth()-HS_ROW_MARGIN_X);
			rows.push_back(row);
			nick=points=0;
			row=0;
		}
		delete brushs[0];
		delete brushs[1];
	}
	catch(const exception& ex)
	{
		if(bkg)
			SDL_FreeSurface(bkg);
		if(lb_totexcl_)
			delete lb_totexcl_;
		if(lb_title_)
			delete lb_title_;

		if(in.is_open())
			in.close();

		if(nick)
			delete nick;
		if(points)
			delete points;
		if(row)
			delete row;

		if(brushs[0])
			delete brushs[0];
		if(brushs[1])
			delete brushs[1];

		list<Window*>::iterator i;
		for(i=rows.begin();i!=rows.end();i++)
			delete *i;

		throw runtime_error(string("HighScoresScreen::HighScoresScreen(...): ")+ex.what());
	}

	setBackground(bkg);

	lb_totexcl_->setX(5);
	lb_totexcl_->setY(getHeight()-lb_totexcl_->getHeight()-3);
	addWidget(lb_totexcl_);
	lb_title_->setX((getWidth()-lb_title_->getWidth())/2);
	lb_title_->setY(10);
	addWidget(lb_title_);

	list<Window*>::iterator i;
	for(i=rows.begin();i!=rows.end();i++)
		addWidget(*i);
}

HighScoresScreen::~HighScoresScreen()
{
}

void HighScoresScreen::ticks(int t)
{
}

Screen* HighScoresScreen::processEvents(bool& quit)
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
						throw runtime_error(string("HighScoresScreen::processEvents(...): ")+ex.what());
					}
				default:
					return 0;
					break;
			}
		}
		else if(event.type==SDL_QUIT)
			quit=true;
	return 0;
}


void HighScoresScreen::makeDefaultRanking(string file)
{
	ofstream os;
	os.open(file.c_str(),std::ios::binary|std::ios::out);
	if(!os.is_open())
		throw runtime_error("no se pudo abrir archivo \"high_scores\"");
	HighScore hs=HighScore("Sourcecode",2671);
	hs.writeTo(os);
	hs=HighScore("Maliq",2415);
	hs.writeTo(os);
	hs=HighScore("Rhene",2336);
	hs.writeTo(os);
	hs=HighScore("Sami",2312);
	hs.writeTo(os);
	hs=HighScore("ToTeX",2150);
	hs.writeTo(os);
	hs=HighScore("Kamon",1351);
	hs.writeTo(os);
	hs=HighScore("Dany",1250);
	hs.writeTo(os);
	hs=HighScore("Knight",1500);
	hs.writeTo(os);
	hs=HighScore("Hasbun",695);
	hs.writeTo(os);
	os.close();
}
