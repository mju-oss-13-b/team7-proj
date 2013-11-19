/*
 * QBOS: tge/Util.h
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

#ifndef TGE_UTIL_H
#define TGE_UTIL_H

#include <SDL/SDL.h>
#include <string>

using std::string;


namespace tge
{

class Util
{
	static int flags_,bpp_;
	static int rmask_,gmask_,bmask_,amask_;
public:
	//Lentisimo metodo para aplicar alpha a una surface con canal alpha.
	//Si usted tienes una mejor forma... HAZME SABER PORFAVOR! :)
	static void alpha32(SDL_Surface *surface,int alpha);

	//true si x,y esta en rect
	static bool pointInRect(int x,int y,const SDL_Rect& rect);

	//Retorna la minima "super-recta" que contiene a r1 y r2
	static SDL_Rect mergeRects(const SDL_Rect& r1,const SDL_Rect& r2);

	//Retorna true si r1 y r2 se intersectan, deja en r3 la interseccion
	static bool intersectRects(const SDL_Rect& r1,const SDL_Rect& r2,SDL_Rect& r3);

	//Carga una imagen de un archivo, transforma a DisplayFormat... etc.
	//En caso de error dispara runtime_exception
	static SDL_Surface* loadImage(const string& file,bool alpha=false);


	//Setea un conjunto de variables, que son usadas por createSurface
	static void setDefaultSurfaceProperties(int flags,int bpp
			,int rmask,int gmask,int bmask,int amask);

	//Crea una surface de wxh, con las propiedades seteadas por la funcion
	//setDefaultSurfaceProperties. Si se pasa un color, se pinta con el.
	//En caso de error dispara exception
	static SDL_Surface* createSurface(int w,int h,SDL_Color* color=0);

	static int min(int n1,int n2);
	static int max(int n1,int n2);

	static string trim(const string& s);
};

}

#endif
