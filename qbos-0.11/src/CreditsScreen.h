/*
 * QBOS: CreditsScreen.h
 *
 * Pantalla con los creditos.
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

#ifndef _CREDITS_SCREEN_H
#define _CREDITS_SCREEN_H

#include "Screen.h"
#include "tge/Label.h"

using tge::Label;


class CreditsScreen:public Screen
{
	Label* lb_title_,*lb_totexcl_;
	Label* lb_text_[7];
	Label* lb_gpl_[2];
	Label* lb_dev_[3],*lb_mail1_;
	Label* lb_testers_[4];
public:
	CreditsScreen(SDLScreen* sdl_screen);
	~CreditsScreen();

	void ticks(int t);
	Screen* processEvents(bool& quit);
};

#endif
