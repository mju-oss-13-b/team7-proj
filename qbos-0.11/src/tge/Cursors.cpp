/*
 * QBOS: tge/cursors.cpp
 *
 * clase con metodos estaticos para el manejo de cursores.
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

#include <string>
#include <stdexcept>
#include "Cursors.h"

using std::string;
using std::exception;
using std::runtime_error;



const char* hand_xpm[]=
{
"24 24 3 1",
"  c None",
". c #000000",
"+ c #FFFFFF",
"     ..                 ",
"    .++.                ",
"    .++.                ",
"    .++.                ",
"    .++.                ",
"    .++...              ",
"    .++.++...           ",
"    .++.++.++..         ",
"    .++.++.++.+.        ",
"... .++.++.++.++.       ",
".++..++++++++.++.       ",
".+++.+++++++++++.       ",
" .++.+++++++++++.       ",
"  .+.+++++++++++.       ",
"  .+++++++++++++.       ",
"   .++++++++++++.       ",
"   .+++++++++++.        ",
"    .++++++++++.        ",
"    .++++++++++.        ",
"     .++++++++.         ",
"     .++++++++.         ",
"     ..........         ",
"                        ",
"                        ",
"5,0"
};

SDL_Cursor* tge::Cursors::default_=0;
SDL_Cursor* tge::Cursors::hand_=0;

void tge::Cursors::init()
{
	default_=SDL_GetCursor(); //guarda cursor por defecto
	try
	{
		hand_=createCursor(hand_xpm);
	}
	catch(const exception& ex)
	{
		hand_=0;
		throw runtime_error(string("tge::Cursors::init(): ")+ex.what());
	}
}

void tge::Cursors::destroy()
{
	if(hand_)
		SDL_FreeCursor(hand_);
}


SDL_Cursor* tge::Cursors::getDefaultCursor()
{
	return default_;
}

SDL_Cursor* tge::Cursors::getHandCursor()
{
	return hand_;
}


SDL_Cursor* tge::Cursors::createCursor(const char** xpm)
{
	int i,row,col,hot_x,hot_y,dim;
	Uint8* data,*mask;
	char black,white;

	sscanf(xpm[0],"%d",&dim);
	data=new Uint8[4*dim];
	mask=new Uint8[4*dim];

	sscanf(xpm[2],"%c",&black);
	sscanf(xpm[3],"%c",&white);

	for(i=-1,row=0;row<dim;++row)
	{
		for(col=0;col<dim;++col)
		{
			if(col%8)
			{
				data[i]<<=1;
				mask[i]<<=1;
			}
			else
			{
				++i;
				data[i]=mask[i]=0;
			}
			if(xpm[4+row][col]==black)
			{
				data[i]|=0x01;
				mask[i]|=0x01;
			}
			else if(xpm[4+row][col]==white)
			{
				mask[i]|=0x01;
			}
		}
	}
	sscanf(xpm[4+row],"%d,%d",&hot_x,&hot_y);

	SDL_Cursor* new_cursor=SDL_CreateCursor(data,mask,dim,dim,hot_x,hot_y);
	delete [] mask;
	delete [] data;
	if(!new_cursor)
		throw runtime_error(string("tge::CursorManager::addCursor(...): ")+SDL_GetError());
	return new_cursor;
}

