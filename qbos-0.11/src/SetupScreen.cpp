/*
 * QBOS: SetupScreen.cpp
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

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "SetupScreen.h"
#include "MenuScreen.h"
#include "constants.h"
#include "GetKey.h"
#include "tge/Util.h"
#include "tge/Cursors.h"
#include "tge/Brush.h"

using std::exception;
using std::runtime_error;
using std::max;
using std::ostringstream;
using tge::Util;
using tge::Cursors;
using tge::Brush;



SetupScreen::SetupScreen(SDLScreen* sdl_screen):Screen(sdl_screen)
{
	SDL_Surface* bkg=0;
	lb_title_=lb_totexcl_=0;

	win_mk_=win_oo_=0;
	lb_mk_=lb_oo_=0;

	SDL_Surface* buttons[move_items_][3];
	for(int i=0;i<move_items_;i++)
	{
		lb_move_items_[i]=0;
		ib_move_items_[i]=0;
		for(int j=0;j<move_items_;j++)
			buttons[i][j]=0;
	}
	lb_interval_=lb_delay_=lb_interval_value_=lb_delay_value_=0;

	load_defaults_=0;

	try
	{
		makeKeyMap();
		config_.load(getHomeFile("config").c_str());

		bkg=Util::loadImage("images/bkg_setup.png");

		SDL_Color c1={230,230,230,0};
		lb_totexcl_=new Label(0,0,1,"fonts/vera.ttf",12,c1,Label::BLENDED);
		lb_totexcl_->setText("www.totex.cl");

		SDL_Color c2={255,176,0,0,};
		lb_title_=new Label(0,0,1,"fonts/polosemi.ttf",40,c2,Label::BLENDED);
		lb_title_->setText("Opciones");

		SDL_Color c3={55,55,55,0};
		Brush brush(c3);
		win_mk_=new Window(SETUP_WINDOW_X,SETUP_WINDOW_Y0,getWidth()-2*SETUP_WINDOW_X,235,1);
		win_mk_->setBackground(brush);
		win_mk_->setBackgroundAlpha(100);
		win_oo_=new Window(SETUP_WINDOW_X,SETUP_WINDOW_Y1,getWidth()-2*SETUP_WINDOW_X,95,1);
		win_oo_->setBackground(brush);
		win_oo_->setBackgroundAlpha(100);

		SDL_Color c4={194,194,194,0,};
		lb_mk_=new Label(0,0,1,"fonts/vera.ttf",14,c4,Label::BLENDED);
		lb_mk_->setText("TECLAS DE MOVIMIENTO");
		lb_oo_=new Label(0,0,1,"fonts/vera.ttf",14,c4,Label::BLENDED);
		lb_oo_->setText("OTRAS OPCIONES");
	
		buttons[0][0]=Util::loadImage("images/setup_right0.png",true);
		buttons[0][1]=Util::loadImage("images/setup_right0.png",true);
		buttons[0][2]=Util::loadImage("images/setup_right1.png",true);
		ib_move_items_[RIGHT]=new ImageButton(0,0,1,buttons[0][0],buttons[0][1],buttons[0][2]);
		buttons[1][0]=Util::loadImage("images/setup_rotate_cw0.png",true);
		buttons[1][1]=Util::loadImage("images/setup_rotate_cw0.png",true);
		buttons[1][2]=Util::loadImage("images/setup_rotate_cw1.png",true);
		ib_move_items_[ROTATE_CW]=new ImageButton(0,0,1,buttons[1][0],buttons[1][1],buttons[1][2]);
		buttons[2][0]=Util::loadImage("images/setup_left0.png",true);
		buttons[2][1]=Util::loadImage("images/setup_left0.png",true);
		buttons[2][2]=Util::loadImage("images/setup_left1.png",true);
		ib_move_items_[LEFT]=new ImageButton(0,0,1,buttons[2][0],buttons[2][1],buttons[2][2]);
		buttons[3][0]=Util::loadImage("images/setup_rotate_acw0.png",true);
		buttons[3][1]=Util::loadImage("images/setup_rotate_acw0.png",true);
		buttons[3][2]=Util::loadImage("images/setup_rotate_acw1.png",true);
		ib_move_items_[ROTATE_ACW]=new ImageButton(0,0,1,buttons[3][0],buttons[3][1],buttons[3][2]);
		buttons[4][0]=Util::loadImage("images/setup_down0.png",true);
		buttons[4][1]=Util::loadImage("images/setup_down0.png",true);
		buttons[4][2]=Util::loadImage("images/setup_down1.png",true);
		ib_move_items_[DOWN]=new ImageButton(0,0,1,buttons[4][0],buttons[4][1],buttons[4][2]);
		buttons[5][0]=Util::loadImage("images/setup_fall0.png",true);
		buttons[5][1]=Util::loadImage("images/setup_fall0.png",true);
		buttons[5][2]=Util::loadImage("images/setup_fall1.png",true);
		ib_move_items_[FALL]=new ImageButton(0,0,1,buttons[5][0],buttons[5][1],buttons[5][2]);

		SDL_Color c5={255,255,255,0,};
		lb_move_items_[RIGHT]=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_move_items_[ROTATE_CW]=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_move_items_[LEFT]=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_move_items_[ROTATE_ACW]=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_move_items_[DOWN]=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_move_items_[FALL]=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);

		lb_interval_=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_interval_->setText("INTERVALO REPETICION:");
		lb_interval_value_=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_delay_=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);
		lb_delay_->setText("RETRASO REPETICION:");
		lb_delay_value_=new Label(0,0,1,"fonts/vera.ttf",14,c5,Label::BLENDED);

		load_defaults_=new Button(0,0,1);
		load_defaults_->setText("Valores por Defecto");

		refresh();
	}
	catch(const exception& ex)
	{
		if(bkg)
			SDL_FreeSurface(bkg);
		if(lb_totexcl_)
			delete lb_totexcl_;
		if(lb_title_)
			delete lb_title_;
		if(win_mk_)
			delete win_mk_;
		if(win_oo_)
			delete win_oo_;
		if(lb_mk_)
			delete lb_mk_;
		if(lb_oo_)
			delete lb_oo_;

		for(int i=0;i<move_items_;i++)
		{
			if(lb_move_items_[i])
				delete lb_move_items_[i];
			if(ib_move_items_[i])
				delete ib_move_items_[i];
			for(int j=0;j<move_items_;j++)
				if(buttons[i][j])
					delete buttons[i][j];
		}

		if(lb_interval_)
			delete lb_interval_;
		if(lb_interval_value_)
			delete lb_interval_value_;
		if(lb_delay_)
			delete lb_delay_;
		if(lb_delay_value_)
			delete lb_delay_value_;
		if(load_defaults_)
			delete load_defaults_;

		throw runtime_error(string("SetupScreen::SetupScreen(...): ")+ex.what());
	}

	setBackground(bkg);

	lb_totexcl_->setX(5);
	lb_totexcl_->setY(getHeight()-lb_totexcl_->getHeight()-3);
	addWidget(lb_totexcl_);
	lb_title_->setX((getWidth()-lb_title_->getWidth())/2);
	lb_title_->setY(10);
	addWidget(lb_title_);

	win_mk_->addWidget(lb_mk_);
	lb_mk_->setX((getWidth()-lb_mk_->getWidth())/2);
	lb_mk_->setY(SETUP_WINDOW_IN_MARGIN_Y);

	int y0=lb_mk_->getY()+lb_mk_->getHeight()+SETUP_WINDOW_IN_MARGIN_Y;
	int hmax=max(max(lb_move_items_[RIGHT]->getHeight(),ib_move_items_[RIGHT]->getHeight())
			,max(lb_move_items_[ROTATE_CW]->getHeight(),ib_move_items_[ROTATE_CW]->getHeight()));
	ib_move_items_[RIGHT]->setY(y0+(hmax-ib_move_items_[RIGHT]->getHeight())/2);
	lb_move_items_[RIGHT]->setY(y0+(hmax-lb_move_items_[RIGHT]->getHeight())/2);
	ib_move_items_[ROTATE_CW]->setY(y0+(hmax-ib_move_items_[ROTATE_CW]->getHeight())/2);
	lb_move_items_[ROTATE_CW]->setY(y0+(hmax-lb_move_items_[ROTATE_CW]->getHeight())/2);

	y0+=hmax+SETUP_WINDOW_IN_MARGIN_Y;
	hmax=max(max(lb_move_items_[LEFT]->getHeight(),ib_move_items_[LEFT]->getHeight())
			,max(lb_move_items_[ROTATE_ACW]->getHeight(),ib_move_items_[ROTATE_ACW]->getHeight()));
	ib_move_items_[LEFT]->setY(y0+(hmax-ib_move_items_[LEFT]->getHeight())/2);
	lb_move_items_[LEFT]->setY(y0+(hmax-lb_move_items_[LEFT]->getHeight())/2);
	ib_move_items_[ROTATE_ACW]->setY(y0+(hmax-ib_move_items_[ROTATE_ACW]->getHeight())/2);
	lb_move_items_[ROTATE_ACW]->setY(y0+(hmax-lb_move_items_[ROTATE_ACW]->getHeight())/2);

	y0+=hmax+SETUP_WINDOW_IN_MARGIN_Y;
	hmax=max(max(lb_move_items_[DOWN]->getHeight(),ib_move_items_[DOWN]->getHeight())
			,max(lb_move_items_[FALL]->getHeight(),ib_move_items_[FALL]->getHeight()));
	ib_move_items_[DOWN]->setY(y0+(hmax-ib_move_items_[DOWN]->getHeight())/2);
	lb_move_items_[DOWN]->setY(y0+(hmax-lb_move_items_[DOWN]->getHeight())/2);
	ib_move_items_[FALL]->setY(y0+(hmax-ib_move_items_[FALL]->getHeight())/2);
	lb_move_items_[FALL]->setY(y0+(hmax-lb_move_items_[FALL]->getHeight())/2);

	int x0=SETUP_WINDOW_IN_X0;
	int wmax=max(ib_move_items_[RIGHT]->getWidth()
			,max(ib_move_items_[LEFT]->getWidth(),ib_move_items_[DOWN]->getWidth()));
	ib_move_items_[RIGHT]->setX(x0+(wmax-ib_move_items_[RIGHT]->getWidth())/2);
	ib_move_items_[LEFT]->setX(x0+(wmax-ib_move_items_[LEFT]->getWidth())/2);
	ib_move_items_[DOWN]->setX(x0+(wmax-ib_move_items_[DOWN]->getWidth())/2);
	lb_move_items_[RIGHT]->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);
	lb_move_items_[LEFT]->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);
	lb_move_items_[DOWN]->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);

	x0=SETUP_WINDOW_IN_X1;
	wmax=max(ib_move_items_[ROTATE_CW]->getWidth()
			,max(ib_move_items_[ROTATE_ACW]->getWidth(),ib_move_items_[FALL]->getWidth()));
	ib_move_items_[ROTATE_CW]->setX(x0+(wmax-ib_move_items_[ROTATE_CW]->getWidth())/2);
	ib_move_items_[ROTATE_ACW]->setX(x0+(wmax-ib_move_items_[ROTATE_CW]->getWidth())/2);
	ib_move_items_[FALL]->setX(x0+(wmax-ib_move_items_[FALL]->getWidth())/2);
	lb_move_items_[ROTATE_CW]->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);
	lb_move_items_[ROTATE_ACW]->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);
	lb_move_items_[FALL]->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);

	win_mk_->addWidget(ib_move_items_[RIGHT]);
	win_mk_->addWidget(lb_move_items_[RIGHT]);
	win_mk_->addWidget(ib_move_items_[ROTATE_CW]);
	win_mk_->addWidget(lb_move_items_[ROTATE_CW]);
	win_mk_->addWidget(ib_move_items_[LEFT]);
	win_mk_->addWidget(lb_move_items_[LEFT]);
	win_mk_->addWidget(ib_move_items_[ROTATE_ACW]);
	win_mk_->addWidget(lb_move_items_[ROTATE_ACW]);
	win_mk_->addWidget(ib_move_items_[DOWN]);
	win_mk_->addWidget(lb_move_items_[DOWN]);
	win_mk_->addWidget(ib_move_items_[FALL]);
	win_mk_->addWidget(lb_move_items_[FALL]);


	lb_oo_->setX((getWidth()-lb_oo_->getWidth())/2);
	lb_oo_->setY(SETUP_WINDOW_IN_MARGIN_Y);
	win_oo_->addWidget(lb_oo_);

	x0=SETUP_WINDOW_IN_X0+SETUP_WINDOW_IN_MARGIN_X;
	y0=lb_oo_->getY()+lb_mk_->getHeight()+SETUP_WINDOW_IN_MARGIN_Y;
	wmax=max(lb_interval_->getWidth(),lb_delay_->getWidth());
	lb_interval_->setX(x0);
	lb_interval_->setY(y0);
	lb_interval_value_->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);
	lb_interval_value_->setY(y0);
	y0+=max(lb_interval_->getHeight(),lb_interval_value_->getHeight())+SETUP_WINDOW_IN_MARGIN_Y;
	lb_delay_->setX(x0);
	lb_delay_->setY(y0);
	lb_delay_value_->setX(x0+wmax+SETUP_WINDOW_IN_MARGIN_X);
	lb_delay_value_->setY(y0);

	win_oo_->addWidget(lb_interval_);
	win_oo_->addWidget(lb_interval_value_);
	win_oo_->addWidget(lb_delay_);
	win_oo_->addWidget(lb_delay_value_);

	addWidget(win_mk_);
	addWidget(win_oo_);

	load_defaults_->setX(getWidth()-load_defaults_->getWidth()-SETUP_WINDOW_X);
	load_defaults_->setY(getHeight()-load_defaults_->getHeight()-SETUP_WINDOW_X);
	addWidget(load_defaults_);

	for(int i=0;i<move_items_;i++)
		items_[i]=ib_move_items_[i];
	items_[INTERVAL]=lb_interval_;
	items_[DELAY]=lb_delay_;
	items_[DEFAULT]=load_defaults_;

	for(int i=0;i<move_items_;i++)
	{
		items_rect_[i]=ib_move_items_[i]->getRect();
		items_rect_[i].x+=win_mk_->getX();
		items_rect_[i].y+=win_mk_->getY();
	}
	items_rect_[INTERVAL]=lb_interval_->getRect();
	items_rect_[INTERVAL].x+=win_oo_->getX();
	items_rect_[INTERVAL].y+=win_oo_->getY();
	items_rect_[DELAY]=lb_delay_->getRect();
	items_rect_[DELAY].x+=win_oo_->getX();
	items_rect_[DELAY].y+=win_oo_->getY();
	items_rect_[DEFAULT]=load_defaults_->getRect();

	item_=clicked_=-1;

	dlg_interval_=dlg_delay_=0;
	dlg_getkey_=0;
	dlg_error_=0;
}

SetupScreen::~SetupScreen()
{
	SDL_EnableKeyRepeat(config_.getRepeatDelay(),config_.getRepeatInterval());
	config_.save(getHomeFile("config").c_str());
}

void SetupScreen::makeKeyMap()
{
	sdl_keys_[SDLK_BACKSPACE]="BACKSPACE";
	sdl_keys_[SDLK_TAB]="TAB";
	sdl_keys_[SDLK_CLEAR]="CLEAR";
	sdl_keys_[SDLK_RETURN]="RETURN";
	sdl_keys_[SDLK_PAUSE]="PAUSE";
	sdl_keys_[SDLK_SPACE]="SPACE";
	sdl_keys_[SDLK_EXCLAIM]="EXCLAMATION MARK";
	sdl_keys_[SDLK_QUOTEDBL]="DOUBLE QUOTE";
	sdl_keys_[SDLK_HASH]="HASH";
	sdl_keys_[SDLK_DOLLAR]="DOLLAR";
	sdl_keys_[SDLK_AMPERSAND]="AMPERSAND";
	sdl_keys_[SDLK_QUOTE]="SINGLE QUOTE";
	sdl_keys_[SDLK_LEFTPAREN]="LEFT PARENTHESIS";
	sdl_keys_[SDLK_RIGHTPAREN]="RIGHT PARENTHESIS";
	sdl_keys_[SDLK_ASTERISK]="ASTERISK";
	sdl_keys_[SDLK_PLUS]="PLUS SIGN";
	sdl_keys_[SDLK_COMMA]="COMMA";
	sdl_keys_[SDLK_MINUS]="MINUS SIGN";
	sdl_keys_[SDLK_PERIOD]="PERIOD / FULL STOP";
	sdl_keys_[SDLK_SLASH]="FORWARD SLASH";
	sdl_keys_[SDLK_0]="0";
	sdl_keys_[SDLK_1]="1";
	sdl_keys_[SDLK_2]="2";
	sdl_keys_[SDLK_3]="3";
	sdl_keys_[SDLK_4]="4";
	sdl_keys_[SDLK_5]="5";
	sdl_keys_[SDLK_6]="6";
	sdl_keys_[SDLK_7]="7";
	sdl_keys_[SDLK_8]="8";
	sdl_keys_[SDLK_9]="9";
	sdl_keys_[SDLK_COLON]="COLON";
	sdl_keys_[SDLK_SEMICOLON]="SEMICOLON";
	sdl_keys_[SDLK_LESS]="LESS-THAN SIGN";
	sdl_keys_[SDLK_EQUALS]="EQUALS SIGN";
	sdl_keys_[SDLK_GREATER]="GREATER-THAN SIGN";
	sdl_keys_[SDLK_QUESTION]="QUESTION MARK";
	sdl_keys_[SDLK_AT]="AT";
	sdl_keys_[SDLK_LEFTBRACKET]="LEFT BRACKET";
	sdl_keys_[SDLK_BACKSLASH]="BACKSLASH";
	sdl_keys_[SDLK_RIGHTBRACKET]="RIGHT BRACKET";
	sdl_keys_[SDLK_CARET]="CARET";
	sdl_keys_[SDLK_UNDERSCORE]="UNDERSCORE";
	sdl_keys_[SDLK_BACKQUOTE]="GRAVE";
	sdl_keys_[SDLK_a]="A";
	sdl_keys_[SDLK_b]="B";
	sdl_keys_[SDLK_c]="C";
	sdl_keys_[SDLK_d]="D";
	sdl_keys_[SDLK_e]="E";
	sdl_keys_[SDLK_f]="F";
	sdl_keys_[SDLK_g]="G";
	sdl_keys_[SDLK_h]="H";
	sdl_keys_[SDLK_i]="I";
	sdl_keys_[SDLK_j]="J";
	sdl_keys_[SDLK_k]="K";
	sdl_keys_[SDLK_l]="L";
	sdl_keys_[SDLK_m]="M";
	sdl_keys_[SDLK_n]="N";
	sdl_keys_[SDLK_o]="O";
	sdl_keys_[SDLK_p]="P";
	sdl_keys_[SDLK_q]="Q";
	sdl_keys_[SDLK_r]="R";
	sdl_keys_[SDLK_s]="S";
	sdl_keys_[SDLK_t]="T";
	sdl_keys_[SDLK_u]="U";
	sdl_keys_[SDLK_v]="V";
	sdl_keys_[SDLK_w]="W";
	sdl_keys_[SDLK_x]="X";
	sdl_keys_[SDLK_y]="Y";
	sdl_keys_[SDLK_z]="Z";
	sdl_keys_[SDLK_DELETE]="DELETE";
	sdl_keys_[SDLK_KP0]="KEYPAD 0";
	sdl_keys_[SDLK_KP1]="KEYPAD 1";
	sdl_keys_[SDLK_KP2]="KEYPAD 2";
	sdl_keys_[SDLK_KP3]="KEYPAD 3";
	sdl_keys_[SDLK_KP4]="KEYPAD 4";
	sdl_keys_[SDLK_KP5]="KEYPAD 5";
	sdl_keys_[SDLK_KP6]="KEYPAD 6";
	sdl_keys_[SDLK_KP7]="KEYPAD 7";
	sdl_keys_[SDLK_KP8]="KEYPAD 8";
	sdl_keys_[SDLK_KP9]="KEYPAD 9";
	sdl_keys_[SDLK_KP_PERIOD]="KEYPAD PERIOD";
	sdl_keys_[SDLK_KP_DIVIDE]="KEYPAD DIVIDE";
	sdl_keys_[SDLK_KP_MULTIPLY]="KEYPAD MULTIPLY";
	sdl_keys_[SDLK_KP_MINUS]="KEYPAD MINUS";
	sdl_keys_[SDLK_KP_PLUS]="KEYPAD PLUS";
	sdl_keys_[SDLK_KP_ENTER]="KEYPAD ENTER";
	sdl_keys_[SDLK_KP_EQUALS]="KEYPAD EQUALS";
	sdl_keys_[SDLK_UP]="UP ARROW";
	sdl_keys_[SDLK_DOWN]="DOWN ARROW";
	sdl_keys_[SDLK_RIGHT]="RIGHT ARROW";
	sdl_keys_[SDLK_LEFT]="LEFT ARROW";
	sdl_keys_[SDLK_INSERT]="INSERT";
	sdl_keys_[SDLK_HOME]="HOME";
	sdl_keys_[SDLK_END]="END";
	sdl_keys_[SDLK_PAGEUP]="PAGE UP";
	sdl_keys_[SDLK_PAGEDOWN]="PAGE DOWN";
	sdl_keys_[SDLK_F1]="F1";
	sdl_keys_[SDLK_F2]="F2";
	sdl_keys_[SDLK_F3]="F3";
	sdl_keys_[SDLK_F4]="F4";
	sdl_keys_[SDLK_F5]="F5";
	sdl_keys_[SDLK_F6]="F6";
	sdl_keys_[SDLK_F7]="F7";
	sdl_keys_[SDLK_F8]="F8";
	sdl_keys_[SDLK_F9]="F9";
	sdl_keys_[SDLK_F10]="F10";
	sdl_keys_[SDLK_F11]="F11";
	sdl_keys_[SDLK_F12]="F12";
	sdl_keys_[SDLK_F13]="F13";
	sdl_keys_[SDLK_F14]="F14";
	sdl_keys_[SDLK_F15]="F15";
	sdl_keys_[SDLK_NUMLOCK]="NUMLOCK";
	sdl_keys_[SDLK_CAPSLOCK]="CAPSLOCK";
	sdl_keys_[SDLK_SCROLLOCK]="SCROLLOCK";
	sdl_keys_[SDLK_RSHIFT]="RIGHT SHIFT";
	sdl_keys_[SDLK_LSHIFT]="LEFT SHIFT";
	sdl_keys_[SDLK_RCTRL]="RIGHT CTRL";
	sdl_keys_[SDLK_LCTRL]="LEFT CTRL";
	sdl_keys_[SDLK_RALT]="RIGHT ALT / ALT GR";
	sdl_keys_[SDLK_LALT]="LEFT ALT";
	sdl_keys_[SDLK_RMETA]="RIGHT META";
	sdl_keys_[SDLK_LMETA]="LEFT META";
	sdl_keys_[SDLK_LSUPER]="LEFT WINDOWS KEY";
	sdl_keys_[SDLK_RSUPER]="RIGHT WINDOWS KEY";
	sdl_keys_[SDLK_MODE]="MODE SHIFT";
	sdl_keys_[SDLK_COMPOSE]="COMPOSE";
	sdl_keys_[SDLK_HELP]="HELP";
	sdl_keys_[SDLK_PRINT]="PRINT-SCREEN";
	sdl_keys_[SDLK_SYSREQ]="SYSRQ";
	sdl_keys_[SDLK_BREAK]="BREAK";
	sdl_keys_[SDLK_MENU]="MENU";
	sdl_keys_[SDLK_POWER]="POWER";
	sdl_keys_[SDLK_EURO]="EURO";
	sdl_keys_[SDLK_UNDO]="UNDO";
}

void SetupScreen::refresh()
{
	lb_move_items_[RIGHT]->setText(sdl_keys_[config_.getMoveKey(Configuration::RIGHT)]);
	lb_move_items_[ROTATE_CW]->setText(sdl_keys_[config_.getMoveKey(Configuration::ROTATE_CW)]);
	lb_move_items_[LEFT]->setText(sdl_keys_[config_.getMoveKey(Configuration::LEFT)]);
	lb_move_items_[ROTATE_ACW]->setText(sdl_keys_[config_.getMoveKey(Configuration::ROTATE_ACW)]);
	lb_move_items_[DOWN]->setText(sdl_keys_[config_.getMoveKey(Configuration::DOWN)]);
	lb_move_items_[FALL]->setText(sdl_keys_[config_.getMoveKey(Configuration::FALL)]);

	ostringstream str;
	str<<config_.getRepeatInterval();
	lb_interval_value_->setText(str.str()+" ms");
	str.str("");
	str<<config_.getRepeatDelay();
	lb_delay_value_->setText(str.str()+" ms");
}

void SetupScreen::getKey(int clicked)
{
	clicked_=clicked;
	int w=200;
	int h=80;
	dlg_getkey_=new GetKey((getWidth()-w)/2,(getHeight()-h)/2,w,h,1);
	addWidget(dlg_getkey_);
}

void SetupScreen::getRepeatInterval()
{
	vector<int>* alphabet=new vector<int>(20);
	alphabet->push_back(SDLK_0);
	alphabet->push_back(SDLK_1);
	alphabet->push_back(SDLK_2);
	alphabet->push_back(SDLK_3);
	alphabet->push_back(SDLK_4);
	alphabet->push_back(SDLK_5);
	alphabet->push_back(SDLK_6);
	alphabet->push_back(SDLK_7);
	alphabet->push_back(SDLK_8);
	alphabet->push_back(SDLK_9);
	alphabet->push_back(SDLK_KP0);
	alphabet->push_back(SDLK_KP1);
	alphabet->push_back(SDLK_KP2);
	alphabet->push_back(SDLK_KP3);
	alphabet->push_back(SDLK_KP4);
	alphabet->push_back(SDLK_KP5);
	alphabet->push_back(SDLK_KP6);
	alphabet->push_back(SDLK_KP7);
	alphabet->push_back(SDLK_KP8);
	alphabet->push_back(SDLK_KP9);
	int w=200;
	int h=80;
	dlg_interval_=new InputDialog("Intervalo Repeticion:",(getWidth()-w)/2,(getHeight()-h)/2,w,h,1,alphabet);
	addWidget(dlg_interval_);
}

void SetupScreen::getRepeatDelay()
{
	vector<int>* alphabet=new vector<int>(20);
	alphabet->push_back(SDLK_0);
	alphabet->push_back(SDLK_1);
	alphabet->push_back(SDLK_2);
	alphabet->push_back(SDLK_3);
	alphabet->push_back(SDLK_4);
	alphabet->push_back(SDLK_5);
	alphabet->push_back(SDLK_6);
	alphabet->push_back(SDLK_7);
	alphabet->push_back(SDLK_8);
	alphabet->push_back(SDLK_9);
	alphabet->push_back(SDLK_KP0);
	alphabet->push_back(SDLK_KP1);
	alphabet->push_back(SDLK_KP2);
	alphabet->push_back(SDLK_KP3);
	alphabet->push_back(SDLK_KP4);
	alphabet->push_back(SDLK_KP5);
	alphabet->push_back(SDLK_KP6);
	alphabet->push_back(SDLK_KP7);
	alphabet->push_back(SDLK_KP8);
	alphabet->push_back(SDLK_KP9);
	int w=200;
	int h=80;
	dlg_delay_=new InputDialog("Retraso Repeticion:",(getWidth()-w)/2,(getHeight()-h)/2,w,h,1,alphabet);
	addWidget(dlg_delay_);
}

void SetupScreen::showError(string error)
{
	int w=215;
	int h=65;
	vector<string> sz(2);
	dlg_error_=new SimpleDialog(error,"OK",(getWidth()-w)/2,(getHeight()-h)/2,w,h,1);
	addWidget(dlg_error_);
}

void SetupScreen::ticks(int t)
{
	if(dlg_interval_)
		dlg_interval_->ticks(t);
	else if(dlg_delay_)
		dlg_delay_->ticks(t);
	else if(dlg_getkey_)
		dlg_getkey_->ticks(t);
	else if(dlg_error_)
		dlg_error_->ticks(t);
}

Screen* SetupScreen::processEvents(bool& quit)
{
	if(dlg_interval_)
	{
		InputDialog::ActionCode code=dlg_interval_->processEvents();
		if(code==InputDialog::OK)
		{
			if(Util::trim(dlg_interval_->getText()).size())
			{
				config_.setRepeatInterval(atoi(dlg_interval_->getText().c_str()));
				try
				{
					lb_interval_value_->setText(dlg_interval_->getText()+" ms");
				}
				catch(const exception& ex)
				{
					throw runtime_error(string("SetupScreen::processEvents(...): ")+ex.what());
				}
				refreshRect(dlg_interval_->getRect());
				removeWidget(dlg_interval_);
				delete dlg_interval_;
				dlg_interval_=0;
			}
		}
		else if(code==InputDialog::CANCEL)
		{
			refreshRect(dlg_interval_->getRect());
			removeWidget(dlg_interval_);
			delete dlg_interval_;
			dlg_interval_=0;
		}
		return 0;
	}
	if(dlg_delay_)
	{
		InputDialog::ActionCode code=dlg_delay_->processEvents();
		if(code==InputDialog::OK)
		{
			if(Util::trim(dlg_delay_->getText()).size())
			{
				config_.setRepeatDelay(atoi(dlg_delay_->getText().c_str()));
				try
				{
					lb_delay_value_->setText(dlg_delay_->getText()+" ms");
				}
				catch(const exception& ex)
				{
					throw runtime_error(string("SetupScreen::processEvents(...): ")+ex.what());
				}
				refreshRect(dlg_delay_->getRect());
				removeWidget(dlg_delay_);
				delete dlg_delay_;
				dlg_delay_=0;
			}
		}
		else if(code==InputDialog::CANCEL)
		{
			refreshRect(dlg_delay_->getRect());
			removeWidget(dlg_delay_);
			delete dlg_delay_;
			dlg_delay_=0;
		}
		return 0;
	}
	if(dlg_getkey_)
	{
		int key=dlg_getkey_->processEvents();
		if(key!=-1)
		{
			refreshRect(dlg_getkey_->getRect());
			removeWidget(dlg_getkey_);
			delete dlg_getkey_;
			dlg_getkey_=0;
			if(key!=SDLK_ESCAPE)
			{
				if(sdl_keys_.find(key)!=sdl_keys_.end())
				{
					try
					{
						lb_move_items_[(Items)clicked_]->setText(sdl_keys_[key]);
					}
					catch(const exception& ex)
					{
						throw runtime_error(string("SetupScreen::processEvents(...): ")+ex.what());
					}
					switch(clicked_)
					{
						case RIGHT:
							config_.setMoveKey(Configuration::RIGHT,(SDLKey)key);
							break;
						case LEFT:
							config_.setMoveKey(Configuration::LEFT,(SDLKey)key);
							break;
						case ROTATE_CW:
							config_.setMoveKey(Configuration::ROTATE_CW,(SDLKey)key);
							break;
						case ROTATE_ACW:
							config_.setMoveKey(Configuration::ROTATE_ACW,(SDLKey)key);
							break;
						case DOWN:
							config_.setMoveKey(Configuration::DOWN,(SDLKey)key);
							break;
						case FALL:
							config_.setMoveKey(Configuration::FALL,(SDLKey)key);
							break;
					}
				}
				else
					showError("Duh!, por favor usa una tecla comun");
			}
		}
		return 0;
	}
	if(dlg_error_)
	{
		if(dlg_error_->processEvents())
		{
			refreshRect(dlg_error_->getRect());
			removeWidget(dlg_error_);
			delete dlg_error_;
			dlg_error_=0;
		}
		return 0;
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
		if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					try
					{
						return new MenuScreen(getSDLScreen());
					}
					catch(const exception& ex)
					{
						throw runtime_error(string("SetupScreen::processEvents(...): ")+ex.what());
					}
				default:
					return 0;
			}
		}
		else if(event.type==SDL_MOUSEMOTION)
		{
			int i,ex_item;
			for(i=0;i<total_items_;i++)
				if(Util::pointInRect(event.motion.x,event.motion.y,items_rect_[i]))
				{
					SDL_SetCursor(Cursors::getHandCursor());
					if(i!=item_)
					{
						ex_item=item_;
						item_=i;
						if(i<6)
							dynamic_cast<ImageButton*>(items_[i])->setState(ImageButton::MOUSEOVER);
						else if(i==8)
							dynamic_cast<Button*>(items_[i])->setState(Button::MOUSEOVER);
					}
					break;
				}
			if(i==total_items_)
			{
				SDL_SetCursor(Cursors::getDefaultCursor());
				if(item_!=-1)
				{
					if(item_<6)
						dynamic_cast<ImageButton*>(items_[item_])->setState(ImageButton::NORMAL);
					else if(item_==8)
						dynamic_cast<Button*>(items_[item_])->setState(Button::NORMAL);
					item_=-1;
				}
			}
		}
		else if(event.type==SDL_MOUSEBUTTONUP)
		{
			int i;
			for(i=0;i<total_items_;i++)
				if(Util::pointInRect(event.motion.x,event.motion.y,items_rect_[i]))
				{
					if(i<6)
					{
						dynamic_cast<ImageButton*>(items_[i])->setState(ImageButton::MOUSEOVER);
						getKey(i);
					}
					else if(i==6)
						getRepeatInterval();
					else if(i==7)
						getRepeatDelay();
					else if(i==8)
					{
						dynamic_cast<Button*>(items_[i])->setState(Button::MOUSEOVER);
						config_.setDefault();
						try
						{
							refresh();
						}
						catch(const exception& ex)
						{
							throw runtime_error(string("SetupScreen::processEvents(...): ")+ex.what());
						}
					}
					break;
				}
		}
		else if(event.type==SDL_MOUSEBUTTONDOWN)
		{
			int i;
			for(i=0;i<total_items_;i++)
				if(Util::pointInRect(event.motion.x,event.motion.y,items_rect_[i]))
				{
					if(i<6)
						dynamic_cast<ImageButton*>(items_[i])->setState(ImageButton::CLICKED);
					else if(i==8)
						dynamic_cast<Button*>(items_[i])->setState(Button::CLICKED);
					break;
				}
		}
		else if(event.type==SDL_QUIT)
			quit=true;
	return 0;
}

