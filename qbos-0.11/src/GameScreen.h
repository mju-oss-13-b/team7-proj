/*
 * QBOS: GameScreen.h
 *
 * Pantalla del juego.
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

#ifndef _GAME_SCREEN_H
#define _GAME_SCREEN_H

#include "Screen.h"
#include "Matrix.h"
#include "NextPiece.h"
#include "tge/Label.h"
#include "tge/Animation.h"
#include "tge/Window.h"
#include "tge/ConfirmDialog.h"
#include "tge/SimpleDialog.h"
#include "tge/InputDialog.h"

using tge::Label;
using tge::Animation;
using tge::Window;
using tge::ConfirmDialog;
using tge::SimpleDialog;
using tge::InputDialog;


class GameScreen:public Screen
{
public:
	enum GameState{PLAYING,GAMEOVER,PAUSED};
private:
	Window* matrix_shadow_,*panel_shadow_;
	Matrix* matrix_;
	Label* lb_next_piece_,*lb_level_,*lb_points_,*lb_totexcl_;
	NextPiece* next_piece_;
	GameState state_;
	Animation* game_over_;
	ConfirmDialog* confirm_exit_;
	SimpleDialog* pause_;
	InputDialog* dlg_nick_;
public:
	GameScreen(SDLScreen* sdl_screen);
	~GameScreen();

	void confirmExit(int quit);
	void pause();
	void getNick();

	void ticks(int t);
	Screen* processEvents(bool& quit);
};

#endif
