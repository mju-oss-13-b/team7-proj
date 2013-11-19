/*
 * QBOS: Configuration.h
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

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <string>
#include <SDL/SDL.h>

using std::string;


class Configuration
{
public:
	enum MoveKey{RIGHT,LEFT,ROTATE_CW,ROTATE_ACW,DOWN,FALL};
private:
	SDLKey move_keys_[6];
	int delay_,interval_;
public:
	Configuration();
	~Configuration();

	void setDefault();

	SDLKey getMoveKey(MoveKey mkey);
	void setMoveKey(MoveKey mkey,SDLKey key);

	void setRepeatInterval(int interval);
	int getRepeatInterval();

	void setRepeatDelay(int delay);
	int getRepeatDelay();

	void load(string file);
	void save(string file);
};

#endif
