/*
 * QBOS: GameScreen.cpp
 *
 * Pantalla del juego.
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

#include <sstream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "GameScreen.h"
#include "MenuScreen.h"
#include "HighScoresScreen.h"
#include "HighScore.h"
#include "Configuration.h"
#include "constants.h"
#include "tge/Util.h"
#include "tge/Cursors.h"
#include "tge/Rapidity.h"
#include "tge/Brush.h"

using std::ifstream;
using std::fstream;
using std::vector;
using std::ostringstream;
using std::exception;
using std::runtime_error;
using tge::Util;
using tge::Cursors;
using tge::Rapidity;
using tge::Brush;



GameScreen::GameScreen(SDLScreen* sdl_screen):Screen(sdl_screen)
{
	SDL_Surface* bkg=0;
	matrix_shadow_=0;
	panel_shadow_=0;
	lb_next_piece_=0;
	lb_level_=0;
	lb_points_=0;
	lb_totexcl_=0;
	matrix_=0;
	next_piece_=0;
	game_over_=0;
	confirm_exit_=0;
	pause_=0;
	dlg_nick_=0;

	try
	{
		bkg=Util::loadImage("images/bkg_game.png");

		SDL_Color black={0,0,0,0};
		Brush brush(black);
		matrix_shadow_=new Window(MATRIX_X,MATRIX_Y,MATRIX_W,MATRIX_H,1);
		panel_shadow_=new Window(PANEL_X,PANEL_Y,PANEL_W,PANEL_H,1);
		matrix_shadow_->setBackground(brush);
		panel_shadow_->setBackground(brush);
		matrix_shadow_->setBackgroundAlpha(97);
		panel_shadow_->setBackgroundAlpha(151);

		SDL_Color c1={250,180,13,0};
		SDL_Color c2={55,237,72,0};
		SDL_Color c3={128,175,245,0};
		SDL_Color c4={230,230,230,0};
		lb_next_piece_=new Label(315,22,1,"fonts/polosemi.ttf",23,c1,Label::BLENDED);
		lb_level_=new Label(315,190,1,"fonts/polosemi.ttf",26,c2,Label::BLENDED);
		lb_points_=new Label(315,225,1,"fonts/polosemi.ttf",19,c3,Label::BLENDED);
		lb_totexcl_=new Label(0,0,1,"fonts/vera.ttf",12,c4,Label::BLENDED);
		lb_next_piece_->setText("Prox. Pieza");
		lb_next_piece_->setX(MATRIX_X+MATRIX_W+(PANEL_W-lb_next_piece_->getWidth())/2);
		lb_level_->setText("Nivel 0");
		lb_points_->setText("0 pts");
		lb_totexcl_->setText("www.totex.cl");

		Configuration config;
		config.load(getHomeFile("config").c_str());
		matrix_=new Matrix(config,1);
		next_piece_=new NextPiece(0,0,1,0);
	}
	catch(const exception& ex)
	{
		if(bkg)
			SDL_FreeSurface(bkg);
		if(matrix_shadow_)
			delete matrix_shadow_;
		if(panel_shadow_)
			delete panel_shadow_;
		if(lb_next_piece_)
			delete lb_next_piece_;
		if(lb_level_)
			delete lb_level_;
		if(lb_points_)
			delete lb_points_;
		if(lb_totexcl_)
			delete lb_totexcl_;
		if(matrix_)
			delete matrix_;
		if(next_piece_)
			delete next_piece_;

		throw runtime_error(string("GameScreen::GameScreen(...): ")+ex.what());
	}

	setBackground(bkg);

	addWidget(matrix_shadow_);
	addWidget(panel_shadow_);

	lb_totexcl_->setX(getWidth()-lb_totexcl_->getWidth()-10);
	lb_totexcl_->setY(getHeight()-lb_totexcl_->getHeight()-8);
	addWidget(lb_next_piece_);
	addWidget(lb_level_);
	addWidget(lb_points_);
	addWidget(lb_totexcl_);

	SDL_SetCursor(Cursors::getDefaultCursor());

	addWidget(matrix_);

	SDL_Surface* srf=matrix_->getNextPiece();
	next_piece_->setSurface(srf);
	next_piece_->setPosition(PANEL_X+(PANEL_W-srf->w)/2
			,PANEL_Y+lb_next_piece_->getY()+lb_next_piece_->getHeight()+5);
	addWidget(next_piece_);

	state_=PLAYING;
}
GameScreen::~GameScreen()
{
}

void GameScreen::confirmExit(int quit)
{
	int w=200;
	int h=80;
	vector<string> text(2);
	text[0]="Estas seguro que deseas";
	text[1]="salir?";
	confirm_exit_=new ConfirmDialog(quit,text,(getWidth()-w)/2,(getHeight()-h)/2,w,h,1);
	addWidget(confirm_exit_);
}

void GameScreen::pause()
{
	int w=200;
	int h=80;
	pause_=new SimpleDialog("Juego en pausa.","Continuar!",(getWidth()-w)/2,(getHeight()-h)/2,w,h,1);
	addWidget(pause_);
}

void GameScreen::getNick()
{
	int w=200;
	int h=80;
	dlg_nick_=new InputDialog("Nick:",(getWidth()-w)/2,(getHeight()-h)/2,w,h,1);
	addWidget(dlg_nick_);
}

void GameScreen::ticks(int t)
{
	if(game_over_)
		game_over_->ticks(t);
	if(confirm_exit_)
	{
		confirm_exit_->ticks(t);
		return;
	}
	else if(pause_)
	{
		pause_->ticks(t);
		return;
	}
	else if(dlg_nick_)
	{
		dlg_nick_->ticks(t);
		return;
	}
	else if(matrix_)
	{
		if(state_==PLAYING)
		{
			matrix_->ticks(t);
			if(matrix_->isDead())
			{
				state_=GAMEOVER;

				vector<SDL_Surface*>* surfaces=new vector<SDL_Surface*>();
				try
				{
					surfaces->reserve(6);
					ostringstream ostr;
					for(int i=0;i<6;i++)
					{
						ostr<<"images/gameover"<<i<<".png";
						SDL_Surface* tmp=Util::loadImage(ostr.str(),true);
						surfaces->push_back(tmp);
						ostr.str("");
					}

					int w=surfaces->at(0)->w;
					int h=surfaces->at(0)->h;
					Rapidity rapidity(1,5);
					game_over_=new Animation((getWidth()-w)/2,(getHeight()-h)/2,w,h
									,1,Animation::LIFO ,surfaces,rapidity);
					addWidget(game_over_);
				}
				catch(const exception& ex)
				{
					vector<SDL_Surface*>::iterator i;
					for(i=surfaces->begin();i!=surfaces->end();i++)
						SDL_FreeSurface(*i);
					throw runtime_error(string("GameScreen::ticks(...): ")+ex.what());
				}

				ifstream in;
				try
				{
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

					if(matrix_->getPoints() && (high_scores.size()<HS_NUM ||
							(*high_scores.rbegin()).getPoints()<matrix_->getPoints()))
						getNick();
				}
				catch(const exception& ex)
				{
					if(in.is_open())
						in.close();
					throw runtime_error(string("GameScreen::ticks(...): ")+ex.what());
				}

			}
		}
		try
		{
			ostringstream ostr;
			ostr<<"Nivel "<<matrix_->getLevel();
			lb_level_->setText(ostr.str());
			ostr.str("");
			ostr<<matrix_->getPoints()<<" pts";
			lb_points_->setText(ostr.str());

			if(!matrix_->isDead())
			{
				SDL_Surface* srf=matrix_->getNextPiece();
				next_piece_->setSurface(srf);
				next_piece_->setPosition(PANEL_X+(PANEL_W-srf->w)/2
						,PANEL_Y+lb_next_piece_->getY()+lb_next_piece_->getHeight()+5);
			}
		}
		catch(const exception& ex)
		{
			throw runtime_error(string("GameScreen::ticks(...): ")+ex.what());
		}
	}
}

Screen* GameScreen::processEvents(bool& quit)
{
	if(confirm_exit_)
	{
		ConfirmDialog::ActionCode code=confirm_exit_->processEvents();
		switch(code)
		{
			case ConfirmDialog::YES:
				if(confirm_exit_->getFlags())
				{
					quit=true;
					return 0;
				}
				else
					return new MenuScreen(getSDLScreen());
			case ConfirmDialog::NO:
				refreshRect(confirm_exit_->getRect());
				removeWidget(confirm_exit_);
				delete confirm_exit_;
				confirm_exit_=0;
				return 0;
			case ConfirmDialog::NOTHING:
				return 0;
		}
	}
	else if(pause_)
	{
		if(pause_->processEvents())
		{
			refreshRect(pause_->getRect());
			removeWidget(pause_);
			delete pause_;
			pause_=0;
		}
		return 0;
	}
	else if(dlg_nick_)
	{
		InputDialog::ActionCode code=dlg_nick_->processEvents();
		switch(code)
		{
			case InputDialog::OK:
				if(!Util::trim(dlg_nick_->getText()).size())
					return 0;
				else
				{
					fstream file;
					try
					{
						file.open(getHomeFile("high_scores").c_str(),std::ios::binary|std::ios::in);
						if(!file.is_open())
							throw runtime_error("no se pudo abrir archivo \"high_scores\"");

						list<HighScore> high_scores;
						while(file.good())
						{
							HighScore hs;
							hs.readFrom(file);
							if(!file.eof())
								high_scores.push_back(hs);
						}
						file.clear();
						file.close();

						HighScore hs(Util::trim(dlg_nick_->getText()),matrix_->getPoints());
						high_scores.push_back(hs);
						high_scores.sort();

						file.open(getHomeFile("high_scores").c_str(),std::ios::binary|std::ios::out);
						if(!file.is_open())
							throw runtime_error("no se pudo abrir archivo \"high_scores\"");
						list<HighScore>::reverse_iterator i;
						int c=0;
						for(i=high_scores.rbegin();i!=high_scores.rend() && c<HS_NUM;i++,c++)
							(*i).writeTo(file);
						file.close();
					}
					catch(const exception& ex)
					{
						if(file.is_open())
							file.close();
						throw runtime_error(string("GameScreen::processEvents(...): ")+ex.what());
					}
					return new HighScoresScreen(getSDLScreen());
				}
			case InputDialog::CANCEL:
				refreshRect(dlg_nick_->getRect());
				removeWidget(dlg_nick_);
				delete dlg_nick_;
				dlg_nick_=0;
				return 0;
			case InputDialog::NOTHING:
				return 0;
		}
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					try
					{
						if(game_over_)
							return new MenuScreen(getSDLScreen());
						else
						{
							confirmExit(0);
							return 0;
						}
					}
					catch(const exception& ex)
					{
						throw runtime_error(string("GameScreen::processEvents(...): ")+ex.what());
					}
				case SDLK_PAUSE:
					try
					{
						if(!game_over_)
						{
							pause();
							return 0;
						}
					}
					catch(const exception& ex)
					{
						throw runtime_error(string("GameScreen::processEvents(...): ")+ex.what());
					}
				default:
					if(matrix_ && !matrix_->isDead())
						matrix_->key(event.key.keysym.sym);
					break;
			}
		}
		else if(event.type==SDL_QUIT)
			confirmExit(1);
	return 0;
}

