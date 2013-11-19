/*
 * QBOS: tge/Cursors.h
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

#ifndef TGE_CURSORS_H
#define TGE_CURSORS_H

#include <SDL/SDL.h>


namespace tge
{

class Cursors
{
	static SDL_Cursor* default_; //cursor por defecto
	static SDL_Cursor* hand_;
public:
	//Inicializa namespace, en este minuto solo guarda el cursor
	//por defecto.
	static void init();

	//No hace ninguna wea, pero si hay un init() mejor que exista,
	//al menos de adorno, un destroy()
	static void destroy();

	//Accesors
	static SDL_Cursor* getDefaultCursor();
	static SDL_Cursor* getHandCursor();

	//Crear un cursor en base a la informacion de un arreglo
	//bidimensional, la que esta en formato xpm.
	//En caso de error dispara Excepcion
	static SDL_Cursor* createCursor(const char** xpm);
};

}

#endif
