/*
 * QBOS: tge/Widget.h
 *
 * Clase base abstracta para derivar todos los objetos que se
 * pintan en la pantalla usando el microtiles array.
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

#ifndef TGE_WIDGET_H
#define TGE_WIDGET_H

#include <SDL/SDL.h>
#include <functional>
#include "Uta.h"

using std::binary_function;


namespace tge
{

class Widget
{
	SDL_Rect rect_,last_painted_rect_;
	int z_;
	bool update_;
protected:
	//accesor para _update
	void setUpdate(bool update);
	//actualiza last_painted_rect_
	void updateLastPaintedRect();
	const SDL_Rect& getLastPaintedRect();

	//Calculan posiciones absolutas
	bool calcAbsoluteRect(SDL_Rect rect,const SDL_Rect* parent_rect
						,SDL_Rect& absolute_position);
	bool calcAbsolutePositionAndDimension(SDL_Rect rect,const SDL_Rect* parent_rect
						,SDL_Rect& position,SDL_Rect& dimension);
public:
	Widget(int x,int y,int w,int h,int z);
	virtual ~Widget();

	//Accesors para todo
	const SDL_Rect& getRect();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	int getZ();
	void setX(int x);
	void setY(int y);
	virtual void setWidth(int w);
	virtual void setHeight(int h);
	void setZ(int z);
	bool getUpdate();

	//Este metodo debe marcar en uta todas las zonas de la
	//pieza que necesitan ser actualizadas. Aca, se debe llamar
	//a setUpdate(false). En todos los metodos en que el
	//estado del objeto cambie de estado, se debe llamar a
	//setUpdate(true).
	//parent_rect_ es la recta del widget padre; se usa para
	//calcular la posicion absoluta del widget y asi poder
	//marcar uta. Si parent_rect_ es nulo, entonces el widget
	//es hijo de la pantalla y su posicion ya es absoluta.
	virtual void getUpdateRects(Uta* uta,const SDL_Rect* parent_rect=0);

	//paint solo debe pintar y llamar a updateLastPaintedRect().
	//NO debe alterarse desde aqui el estado del objeto, ya que paint
	//puede ser llamada para repintar zonas donde coincida con otras
	//piezas.  parent_rect_ tiene el mismo significado que en el metodo
	//anterior. Debes tener cuidado de NO pintar fuera de
	//getAbsolutePosition()!
	virtual void paint(SDL_Surface* screen,const SDL_Rect* parent_rect=0)=0;
};

/*
 * Esta clase es una comodidad para ordenar listas de
 * punteros a Widget segun su atributo z_. Toda clase
 * "contenedora" de widgets deberia tener una lista de
 * sus hijos y hacer inserciones tomando en cuenta el
 * orden, usando para esto una instancia de LessZ
 */
class LessZ:public binary_function<Widget*,Widget*,bool>
{
public:
	bool operator()(Widget* w1,Widget* w2);
};

}

#endif
