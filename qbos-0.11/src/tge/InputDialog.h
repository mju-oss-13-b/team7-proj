/*
 * QBOS: tge/InputDialog.h
 *
 * Dialogo para ingresar texto
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

#ifndef TGE_INPUT_DIALOG_H
#define TGE_INPUT_DIALOG_H

#include <string>
#include <vector>
#include "Window.h"
#include "Button.h"
#include "Textfield.h"

using std::string;



namespace tge
{

class InputDialog:public Window
{
public:
	enum ActionCode{OK,CANCEL,NOTHING};
private:
	Textfield* textfield_;
	Button* btn_ok_,*btn_cancel_;
	SDL_Rect ok_rect_,cancel_rect_;
	vector<int>* alphabet_;
public:
	InputDialog(string text,int x,int y,int w,int h,int z,vector<int>* alphabet=0);
	virtual ~InputDialog();

	const string& getText();
	void ticks(int t);
	ActionCode processEvents();
};

}

#endif
