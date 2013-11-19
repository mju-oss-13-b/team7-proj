/*
 * QBOS: qbos.cpp
 *
 * funcion main del juego.
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

#ifdef HAVE_UNISTD_H 
	#include <unistd.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include <SDL/SDL.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "Configuration.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "HighScoresScreen.h"
#include "constants.h"
#include "tge/Util.h"
#include "tge/SDLScreen.h"
#include "tge/Cursors.h"

using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::runtime_error;
using std::fstream;
using tge::Util;
using tge::SDLScreen;
using tge::Cursors;


//Si no existe file, lo crea y retorna true. Si ocurre un error
//al tratar de crear file, dispara una excepcion.
//Si file ya existe no hace nada y retorna false.
bool touch(string file);


int main(int argc,char** argv)
{
	int tl;
	bool quit;
	Screen* screen;

	cout<<PACKAGE_STRING<<" beta"<<endl<<endl
		<<"Jorge Riquelme Santana  <totex@totex.cl>"<<endl
		<<"http://www.totex.cl"<<endl<<endl;
#ifdef HAVE_CHDIR
	if(chdir(DATADIR))
	{
		cout<<"Ups! no pude encontrar data en "<<DATADIR<<endl;
		if(chdir("./data"))
		{
			cout<<"ah! no pude encontrar data"<<endl;
			return 1;
		}
		else
			cout<<"data encontrada en ./data, haz un make install :p"<<endl;
	}
	else
		cout<<"data encontrada en "<<DATADIR<<"..."<<endl;
#else
	cout<<"Ups! HAVE_CHDIR no esta definida!"<<endl;
#endif

#if defined(HAVE_SYS_STAT_H) and defined(HAVE_SYS_TYPES_H)
	string homedir=(string)getenv("HOME")+"/.qbos/";

	int k=mkdir(homedir.c_str(),S_IRWXU|S_IROTH|S_IXOTH|S_IRGRP|S_IXGRP);
	if(!k)
		cout<<"creando directorio ~/.qbos ..."<<endl;
#else
	cout<<"Ups! HAVE_SYS_STAT_H y HAVE_SYS_TYPES_H no estan definidas!"<<endl;
	string homedir="";
#endif

	try
	{
		if(touch(homedir+"high_scores"))
		{
			cout<<"creando archivo "+homedir+"high_scores ..."<<endl;
			HighScoresScreen::makeDefaultRanking(homedir+"high_scores");
		}
		if(touch(homedir+"config"))
			cout<<"creando archivo "+homedir+"config ..."<<endl;
	}
	catch(const exception& ex)
	{
		cout<<ex.what()<<endl;
		return 2;
	}


	SDLScreen* sdl_screen=0;

	try
	{
		sdl_screen=new SDLScreen(436,560,BPP);
		sdl_screen->setTitle((string)PACKAGE_STRING+" beta");
		Util::setDefaultSurfaceProperties(SDL_SWSURFACE,BPP,RMASK,GMASK,BMASK,AMASK);

	}
	catch(const exception& ex)
	{
		cout<<ex.what()<<endl;
		return 3;
	}

	try
	{
		Configuration config;
		config.load(homedir+"config");
		SDL_EnableKeyRepeat(config.getRepeatDelay(),config.getRepeatInterval());

		screen=new MenuScreen(sdl_screen);
	}
	catch(const exception& ex)
	{
		cout<<ex.what()<<endl;
		delete sdl_screen;
		return 4;
	}

	tl=SDL_GetTicks();

	quit=false;
	while(!quit)
	{
		int t=SDL_GetTicks();
		if(t-tl>=10)
		{
			int rep=(t-tl)/10;

			screen->ticks(rep);
			tl+=10*rep;

			try
			{
				Screen* new_screen=screen->processEvents(quit);
				if(new_screen)
				{
					delete screen;
					screen=new_screen;
				}
				screen->update();
			}
			catch(const exception& ex)
			{
				cout<<ex.what()<<endl;
				delete screen;
				delete sdl_screen;
				return 5;
			}
		}
		else
			SDL_Delay(10-(t-tl));
	}
	delete screen;
	delete sdl_screen;

	cout<<endl<<"chauuu! :p"<<endl;

	return 0;
}

bool touch(string file)
{
	fstream f;
	f.open(file.c_str(),fstream::in);
	f.close();
	if(f.fail())
	{
		f.clear();
		f.open(file.c_str(),fstream::out);
		f.close();
		if(f.fail())
			throw runtime_error(((string)"touch(string file): no se pudo crear "+file).c_str());
		return true;
	}
	else
		return false;
}
