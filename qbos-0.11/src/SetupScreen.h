/*
 * QBOS: SetupScreen.h
 *
 * Pantalla de configuracion.
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

#ifndef _SETUP_SCREEN_H
#define _SETUP_SCREEN_H

#include <map>
#include <string>
#include "Screen.h"
#include "GetKey.h"
#include "Configuration.h"
#include "tge/Label.h"
#include "tge/Window.h"
#include "tge/ImageButton.h"
#include "tge/Button.h"
#include "tge/InputDialog.h"
#include "tge/SimpleDialog.h"

using std::map;
using std::string;
using tge::Label;
using tge::Window;
using tge::ImageButton;
using tge::Button;
using tge::InputDialog;
using tge::SimpleDialog;


class SetupScreen:public Screen
{
	static const int total_items_=9;
	enum Items{RIGHT,ROTATE_CW,LEFT,ROTATE_ACW,DOWN,FALL,INTERVAL,DELAY,DEFAULT};

	Label* lb_title_,*lb_totexcl_;

	Window* win_mk_,*win_oo_;
	Label* lb_mk_,*lb_oo_;

	static const int move_items_=6;
	Label* lb_move_items_[move_items_];
	ImageButton* ib_move_items_[move_items_];
	int editing_;

	Label* lb_interval_,*lb_delay_,*lb_interval_value_,*lb_delay_value_;

	Button* load_defaults_;

	Widget* items_[total_items_];
	SDL_Rect items_rect_[total_items_];
	int item_,clicked_;

	map<int,string> sdl_keys_;

	InputDialog* dlg_interval_,*dlg_delay_;
	GetKey* dlg_getkey_;
	SimpleDialog* dlg_error_;

	Configuration config_;

	void makeKeyMap();
	void refresh();
public:
	SetupScreen(SDLScreen* sdl_screen);
	~SetupScreen();

	void getKey(int clicked);
	void getRepeatInterval();
	void getRepeatDelay();
	void showError(string error);

	void ticks(int t);
	Screen* processEvents(bool& quit);
};

#endif
