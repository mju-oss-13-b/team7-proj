/*
 * QBOS: tge/Uta.h
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

#ifndef TGE_UTA_H
#define TGE_UTA_H

#include <SDL/SDL.h>
#include <vector>
#include <utility>
#include <list>

using std::vector;
using std::pair;
using std::list;


namespace tge
{

class Uta
{
	int w_,h_;
	int cols_,rows_;
	vector<Uint32> utiles_;

	bool intersect(int r1_x0,int r1_y0,int r1_xf,int r1_yf,
			int r2_x0,int r2_y0,int r2_xf,int r2_yf,
			int& rf_x0,int& rf_y0,int& rf_xf,int& rf_yf);
public:
	//Ancho y alto de la pantalla en pixeles
	Uta(int w,int h);
	~Uta();

	//Limpio el microtiles array
	void clear();

	//Agrego rects
	void addRect(const SDL_Rect& rect);
	void addRects(const list<SDL_Rect>& rects);

	//Retorna un pair, donde el campo first indica cuantas rects
	//tiene el arreglo en second. La memoria del arreglo del campo
	//second fue reservada con malloc, es responsabilidad de quien
	//llama a este metodo liberar la memoria posteriormente con free.
	pair<int,SDL_Rect*> getRects();
};

}

#endif
