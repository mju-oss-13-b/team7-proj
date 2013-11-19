/*
 * QBOS: Configuration.cpp
 *
 * Configuracion del juego
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

#include <fstream>
#include <stdexcept>
#include "Configuration.h"

using std::runtime_error;
using std::ifstream;
using std::ofstream;



Configuration::Configuration()
{
	setDefault();
}

Configuration::~Configuration()
{
}


void Configuration::setDefault()
{
	move_keys_[RIGHT]=SDLK_RIGHT;
	move_keys_[LEFT]=SDLK_LEFT;
	move_keys_[ROTATE_CW]=SDLK_UP;
	move_keys_[ROTATE_ACW]=SDLK_KP0;
	move_keys_[DOWN]=SDLK_DOWN;
	move_keys_[FALL]=SDLK_LCTRL;
	delay_=100;
	interval_=SDL_DEFAULT_REPEAT_INTERVAL;
}

SDLKey Configuration::getMoveKey(MoveKey mkey)
{
	return move_keys_[mkey];
}

void Configuration::setMoveKey(MoveKey mkey,SDLKey key)
{
	move_keys_[mkey]=key;
}

void Configuration::setRepeatInterval(int interval)
{
	interval_=interval;
}

int Configuration::getRepeatInterval()
{
	return interval_;
}

void Configuration::setRepeatDelay(int delay)
{
	delay_=delay;
}

int Configuration::getRepeatDelay()
{
	return delay_;
}

void Configuration::load(string file)
{
	ifstream is(file.c_str(),std::ios::binary);
	if(!is.is_open())
		throw runtime_error("Configuration::load(...): no se pudo abrir archivo \""+file+"\"");
	is.read(reinterpret_cast<char*>(&move_keys_[RIGHT]),sizeof(SDLKey));
	is.read(reinterpret_cast<char*>(&move_keys_[LEFT]),sizeof(SDLKey));
	is.read(reinterpret_cast<char*>(&move_keys_[ROTATE_CW]),sizeof(SDLKey));
	is.read(reinterpret_cast<char*>(&move_keys_[ROTATE_ACW]),sizeof(SDLKey));
	is.read(reinterpret_cast<char*>(&move_keys_[DOWN]),sizeof(SDLKey));
	is.read(reinterpret_cast<char*>(&move_keys_[FALL]),sizeof(SDLKey));
	is.read(reinterpret_cast<char*>(&delay_),sizeof(int));
	is.read(reinterpret_cast<char*>(&interval_),sizeof(int));
	is.close();
}

void Configuration::save(string file)
{
	ofstream os(file.c_str(),std::ios::binary);
	if(!os.is_open())
		throw runtime_error("Configuration::load(...): no se pudo abrir archivo \""+file+"\"");
	os.write(reinterpret_cast<const char*>(&move_keys_[RIGHT]),sizeof(SDLKey));
	os.write(reinterpret_cast<const char*>(&move_keys_[LEFT]),sizeof(SDLKey));
	os.write(reinterpret_cast<const char*>(&move_keys_[ROTATE_CW]),sizeof(SDLKey));
	os.write(reinterpret_cast<const char*>(&move_keys_[ROTATE_ACW]),sizeof(SDLKey));
	os.write(reinterpret_cast<const char*>(&move_keys_[DOWN]),sizeof(SDLKey));
	os.write(reinterpret_cast<const char*>(&move_keys_[FALL]),sizeof(SDLKey));
	os.write(reinterpret_cast<const char*>(&delay_),sizeof(int));
	os.write(reinterpret_cast<const char*>(&interval_),sizeof(int));
	if(!os.good())
		throw runtime_error("Configuration::load(...): no se pudo escribir archivo \""+file+"\"");
	os.close();
}

