/*
 * QBOS: tge/Uta.cpp
 *
 * Mi simplificada implementacion de la tecnica microtiles array,
 * usada para para refrescar areas minimas de la pantalla.
 * No es para nada lo mas rapido del mundo, pero se ajusta a mis
 * necesidades :p
 *
 * En http://www.levien.com/libart/uta.html puedes encontrar una
 * descripcion sobre este metodo.
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

#include "Uta.h"

#define UTA_BBOX_CONS(x0,y0,x1,y1) (((x0)<<24) | ((y0)<<16) | ((x1)<<8) | (y1))

#define UTA_BBOX_X0(ub) ((ub)>>24)
#define UTA_BBOX_Y0(ub) (((ub)>>16) & 0xff)
#define UTA_BBOX_X1(ub) (((ub)>>8) & 0xff)
#define UTA_BBOX_Y1(ub) ((ub)&0xff)

#define UTILE_SHIFT 5
#define UTILE_SIZE (1<<UTILE_SHIFT)

#define UTILE_FULL 0x00002020
#define UTILE_EMPTY 0x0

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))



tge::Uta::Uta(int w,int h)
{
	w_=w;
	h_=h;
	rows_=h/UTILE_SIZE;
	if(h%UTILE_SIZE)
		rows_++;
	cols_=w/UTILE_SIZE;
	if(w%UTILE_SIZE)
		cols_++;
	utiles_.reserve(rows_*cols_);
	utiles_.resize(rows_*cols_);

	vector<Uint32>::iterator i;
	for(i=utiles_.begin();i!=utiles_.end();i++)
		*i=UTILE_EMPTY;
}

tge::Uta::~Uta()
{
}


void tge::Uta::clear()
{
	vector<Uint32>::iterator i;
	for(i=utiles_.begin();i!=utiles_.end();i++)
		*i=UTILE_EMPTY;
}

void tge::Uta::addRect(const SDL_Rect& crect)
{
	if(crect.w<=0 || crect.h<=0)
		return;
	
	SDL_Rect rect=crect;
	if(rect.x<0)
		rect.x=0;
	if(rect.y<0)
		rect.y=0;
	if(rect.x+rect.w>w_)
		rect.w=w_-rect.x;
	if(rect.y+rect.h>h_)
		rect.h=h_-rect.y;

	int r,c,cf,rf;
	if(rect.x%UTILE_SIZE)
	{
		int dw=UTILE_SIZE-rect.x%UTILE_SIZE;
		if(rect.w-dw<=0)
			cf=0;
		else
			cf=(rect.w-dw)/UTILE_SIZE+(((rect.w-dw)%UTILE_SIZE)?1:0);
	}
	else
		cf=rect.w/UTILE_SIZE+((rect.w%UTILE_SIZE)?1:0);

	if(rect.y%UTILE_SIZE)
	{
		int dh=UTILE_SIZE-rect.y%UTILE_SIZE;
		if(rect.h-dh<=0)
			rf=0;
		else
			rf=(rect.h-dh)/UTILE_SIZE+(((rect.h-dh)%UTILE_SIZE)?1:0);
	}
	else
		rf=rect.h/UTILE_SIZE+((rect.h%UTILE_SIZE)?1:0);

	rf+=rect.y/UTILE_SIZE;
	cf+=rect.x/UTILE_SIZE;

	
	for(r=rect.y/UTILE_SIZE;r<=rf;r++)
		for(c=rect.x/UTILE_SIZE;c<=cf;c++)
		{
			int r1_x0,r1_y0,r1_x1,r1_y1;
			int r2_x0,r2_y0,r2_x1,r2_y1;
			int r_x0,r_y0,r_x1,r_y1;

			r1_x0=c*UTILE_SIZE;
			r1_y0=r*UTILE_SIZE;
			r1_x1=r1_x0+UTILE_SIZE;
			r1_y1=r1_y0+UTILE_SIZE;
			r2_x0=rect.x;
			r2_y0=rect.y;
			r2_x1=rect.x+rect.w;
			r2_y1=rect.y+rect.h;

			if(intersect(r1_x0,r1_y0,r1_x1,r1_y1,
					r2_x0,r2_y0,r2_x1,r2_y1,
					r_x0,r_y0,r_x1,r_y1))
			{
				int x0,y0,x1,y1;
				Uint32& utile=utiles_[r*cols_+c];

				if(utile==UTILE_FULL)
					continue;
				else if(utile!=UTILE_EMPTY)
				{
					//desempaqueto utile
					x0=r1_x0+UTA_BBOX_X0(utile);
					y0=r1_y0+UTA_BBOX_Y0(utile);
					x1=r1_x0+UTA_BBOX_X1(utile);
					y1=r1_y0+UTA_BBOX_Y1(utile);

					//hago minimo super rectangulo
					x0=MIN(x0,r_x0);
					y0=MIN(y0,r_y0);
					x1=MAX(x1,r_x1);
					y1=MAX(y1,r_y1);
				}
				else
				{
					x0=r_x0;
					y0=r_y0;
					x1=r_x1;
					y1=r_y1;
				}

				//empaqueto y actualizo utile
				utile=UTA_BBOX_CONS(x0-r1_x0,y0-r1_y0,x1-r1_x0,y1-r1_y0);
			}
		}
}

void tge::Uta::addRects(const list<SDL_Rect>& rects)
{
	list<SDL_Rect>::const_iterator i;
	for(i=rects.begin();i!=rects.end();i++)
		addRect(*i);
}

pair<int,SDL_Rect*> tge::Uta::getRects()
{
	vector<Uint32>::iterator i;
	int n=0;
	for(i=utiles_.begin();i!=utiles_.end();i++)
		if(*i!=UTILE_EMPTY)
			n++;
	if(!n)
		return pair<int,SDL_Rect*>(0,0);

	SDL_Rect* rects=(SDL_Rect*)malloc(sizeof(SDL_Rect)*n);
	n=0;
	for(int r=0;r<rows_;r++)
		for(int c=0;c<cols_;c++)
		{
			Uint32 utile=utiles_[r*cols_+c];

			if(utile==UTILE_EMPTY)
				continue;
			else if(utile==UTILE_FULL)
			{
				rects[n].x=c*UTILE_SIZE;
				rects[n].y=r*UTILE_SIZE;
				rects[n].w=rects[n].h=UTILE_SIZE;
				n++;
			}
			else
			{
				rects[n].x=UTA_BBOX_X0(utile);
				rects[n].y=UTA_BBOX_Y0(utile);
				rects[n].w=UTA_BBOX_X1(utile)-rects[n].x;
				rects[n].h=UTA_BBOX_Y1(utile)-rects[n].y;
				rects[n].x+=c*UTILE_SIZE;
				rects[n].y+=r*UTILE_SIZE;
				n++;
			}
		}
	return pair<int,SDL_Rect*>(n,rects);
}


bool tge::Uta::intersect(int r1_x0,int r1_y0,int r1_x1,int r1_y1,
		int r2_x0,int r2_y0,int r2_x1,int r2_y1,
		int& rf_x0,int& rf_y0,int& rf_x1,int& rf_y1)
{
	if(r2_x0<r1_x1 && r2_x1>r1_x0
			&& r2_y0<r1_y1 && r2_y1>r1_y0)
	{
		rf_x0=MAX(r1_x0,r2_x0);
		rf_y0=MAX(r1_y0,r2_y0);
		rf_x1=MIN(r1_x1,r2_x1);
		rf_y1=MIN(r1_y1,r2_y1);

		return true;
	}
	else
		return false;
}

