/*
 * QBOS: tge/Util.cpp
 *
 * Clase con metodos estaticos de proposito general.
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
#include <SDL/SDL_image.h>
#include "Util.h"

using std::runtime_error;



int tge::Util::flags_;
int tge::Util::bpp_;
int tge::Util::rmask_;
int tge::Util::gmask_;
int tge::Util::bmask_;
int tge::Util::amask_;

void tge::Util::alpha32(SDL_Surface *surface,int alpha)
{
	SDL_PixelFormat* format=surface->format;
	for(int x=0;x<surface->w;x++)
		for(int y=0;y<surface->h;y++)
		{
			//saco pixel
			Uint8* p=(Uint8*)surface->pixels + y * surface->pitch + x * 4;
			Uint32 pixel=*(Uint32 *)p;

			//extraigo r,g,b,a
			Uint8 r,g,b,a;
			unsigned v;
			v=(pixel & format->Rmask) >> format->Rshift;
			r=(v << format->Rloss) + (v >> (8 - (format->Rloss << 1)));
			v=(pixel & format->Gmask) >> format->Gshift;
			g=(v << format->Gloss) + (v >> (8 - (format->Gloss << 1)));
			v=(pixel & format->Bmask) >> format->Bshift;
			b=(v << format->Bloss) + (v >> (8 - (format->Bloss << 1)));
			if(format->Amask)
			{
				v=(pixel & format->Amask) >> format->Ashift;
				a=(v << format->Aloss) + (v >> (8 - (format->Aloss << 1)));
			}
			else
				a=SDL_ALPHA_OPAQUE;

			//aplico alpha
			a=(Uint8)((a*alpha+128)/255);

			//empaqueto de nuevo
			pixel=(r >> format->Rloss) << format->Rshift
				| (g >> format->Gloss) << format->Gshift
				| (b >> format->Bloss) << format->Bshift
				| ((a >> format->Aloss) << format->Ashift & format->Amask);

			//actualizo pixel
			*(Uint32*)p=pixel;
		}
}

bool tge::Util::pointInRect(int x,int y,const SDL_Rect& rect)
{
	return x>rect.x && x<rect.x+rect.w && y>rect.y && y<rect.y+rect.h;
}

SDL_Rect tge::Util::mergeRects(const SDL_Rect& r1,const SDL_Rect& r2)
{
	SDL_Rect rect;
	rect.x=min(r1.x,r2.x);
	rect.y=min(r1.y,r2.y);
	rect.w=max(r1.x+r1.w,r2.x+r2.w);
	rect.h=max(r1.y+r1.h,r2.y+r2.h);
	rect.w-=rect.x;
	rect.h-=rect.y;
	return rect;
}

bool tge::Util::intersectRects(const SDL_Rect& r1,const SDL_Rect& r2,SDL_Rect& r3)
{
	if(r2.x<r1.x+r1.w && r2.x+r2.w>r1.x
			&& r2.y<r1.y+r1.h && r2.y+r2.h>r1.y)
	{
		r3.x=max(r1.x,r2.x);
		r3.y=max(r1.y,r2.y);
		int x1=min(r1.x+r1.w,r2.x+r2.w);
		int y1=min(r1.y+r1.h,r2.y+r2.h);
		r3.w=x1-r3.x;
		r3.h=y1-r3.y;

		return true;
	}
	else
		return false;
}

SDL_Surface* tge::Util::loadImage(const string& file,bool alpha)
{
	SDL_Surface* tmp=IMG_Load(file.c_str());
	if(!tmp)
		throw runtime_error(string("tge::loadImage(...): ")+IMG_GetError());

	SDL_Surface* img=0;
	if(alpha)
		img=SDL_DisplayFormatAlpha(tmp);
	else
		img=SDL_DisplayFormat(tmp);
	if(!img)
		return tmp;
	else
	{
		SDL_FreeSurface(tmp);
		return img;
	}

}

void tge::Util::setDefaultSurfaceProperties(int flags,int bpp,int rmask,int gmask,int bmask,int amask)
{
	flags_=flags;
	bpp_=bpp;
	rmask_=rmask;
	gmask_=gmask;
	bmask_=bmask;
	amask_=amask;
}

SDL_Surface* tge::Util::createSurface(int w,int h,SDL_Color* color)
{
	SDL_Surface* tmp=SDL_CreateRGBSurface(flags_,w,h,bpp_,rmask_,gmask_,bmask_,amask_);
	if(!tmp)
		throw runtime_error(string("tge::createSurface")+" SDL_GetError()");
	SDL_Surface* tmp2=SDL_DisplayFormat(tmp);
	SDL_Surface* result;
	if(!tmp2)
		result=tmp;
	else
	{
		SDL_FreeSurface(tmp);
		result=tmp2;
	}
	if(color)
		SDL_FillRect(result,0,SDL_MapRGB(result->format,color->r,color->g,color->b));
	return result;
}

int tge::Util::min(int n1,int n2)
{
	return n1<n2?n1:n2;
}

int tge::Util::max(int n1,int n2)
{
	return n1>n2?n1:n2;
}

//tomada de http://www.smart2help.com/e-books/ticpp-2nd-ed-vol-two/Chap04.htm
string tge::Util::trim(const string& s)
{
	if(s.length()==0)
		return s;
	int b=s.find_first_not_of(" \t");
	int e=s.find_last_not_of(" \t");
	if(b==-1) // No non-spaces
		return "";
	return string(s,b,e-b+1);
}

