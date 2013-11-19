/*
 * QBOS: tge/ConfirmDialog.h
 *
 * Dialogo con si/no
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

#ifndef TGE_CONFIRM_DIALOG_H
#define TGE_CONFIRM_DIALOG_H

#include <vector>
#include <string>
#include "Window.h"
#include "Button.h"

using std::vector;
using std::string;


namespace tge
{

class ConfirmDialog:public Window
{
public:
	enum ActionCode{YES,NO,NOTHING};
private:
	int flags_;
	Button* btn_yes_,*btn_no_;
	SDL_Rect yes_rect_,no_rect_;
public:
	ConfirmDialog(int flags,string text
			,int x,int y,int w,int h,int z);
	ConfirmDialog(int flags,vector<string>& text
			,int x,int y,int w,int h,int z);
	virtual ~ConfirmDialog();

	int getFlags();
	void ticks(int t);
	ActionCode processEvents();
};

}

#endif
