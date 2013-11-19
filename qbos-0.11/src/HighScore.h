/*
 * QBOS: HighScore.h
 *
 * Puntaje.
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

#ifndef _HIGHSCORE_H
#define _HIGHSCORE_H

#include <string>
#include <istream>
#include <ostream>

using std::string;
using std::istream;
using std::ostream;


class HighScore
{
	string nick_;
	int points_;
public:
	HighScore();
	HighScore(const string& nick,int points);
	~HighScore();

	void readFrom(istream& is);
	void writeTo(ostream& os);

	const string& getNick() const;
	int getPoints() const;

	bool operator<(HighScore& hs);
};

#endif
