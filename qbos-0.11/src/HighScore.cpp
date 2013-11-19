/*
 * QBOS: HighScore.cpp
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

#include "HighScore.h"



HighScore::HighScore()
{
	nick_="";
	points_=0;
}

HighScore::HighScore(const string& nick,int points)
{
	nick_=nick;
	points_=points;
}

HighScore::~HighScore()
{
}

void HighScore::readFrom(istream& is)
{
	is.read(reinterpret_cast<char*>(&points_),sizeof(int));
	int len=-1;
	is.read(reinterpret_cast<char*>(&len),sizeof(int));
	if(len==-1)
		return;
	char* buffer=new char[len+1];
	is.read(reinterpret_cast<char*>(buffer),len);
	buffer[len]=0;
	nick_=string(buffer);
	delete [] buffer;
}

void HighScore::writeTo(ostream& os)
{
	os.write(reinterpret_cast<const char*>(&points_),sizeof(int));
	int len=nick_.size();
	os.write(reinterpret_cast<const char*>(&len),sizeof(int));
	os.write(reinterpret_cast<const char*>(nick_.c_str()),len);
}

const string& HighScore::getNick() const
{
	return nick_;
}

int HighScore::getPoints() const
{
	return points_;
}

bool HighScore::operator<(HighScore& hs)
{
	return points_<hs.points_;
}

