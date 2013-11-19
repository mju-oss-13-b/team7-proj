/*
 * QBOS: MenuScreen.h
 *
 * Pantalla inicial con el menu principal.
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

#ifndef _MENU_SCREEN_H
#define _MENU_SCREEN_H

#include "Screen.h"
#include "MenuItem.h"
#include "MenuArrow.h"
#include "tge/Label.h"

using tge::Label;


class MenuScreen:public Screen
{
	static int item_;
	MenuItem* items_[4];
	MenuArrow* arrow_;
	Label* lb_totexcl_;

	Screen* makeNextScreen();
	void refreshItems(int ex_item);
public:
	MenuScreen(SDLScreen* sdl_screen);
	~MenuScreen();

	void ticks(int t);
	Screen* processEvents(bool& quit);
};

#endif
