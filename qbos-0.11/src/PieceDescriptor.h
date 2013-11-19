/*
 * QBOS: PieceDescriptor.h
 *
 * Descriptor de una pieza. Esta clase describe el comportamiento
 * de una pieza: su constitucion y como gira en la pantalla.
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

#ifndef _PIECE_DESCRIPTOR_H
#define _PIECE_DESCRIPTOR_H

#include <string>
#include <istream>
#include "SafeMatrix.h"

using std::string;
using std::istream;


class PieceDescriptor
{
	SafeMatrix matrix_;
	float left_delta_[4][2];
	float right_delta_[4][2];
public:
	PieceDescriptor();
	~PieceDescriptor();

	SafeMatrix getSafeMatrix(int state);

	float getLeftDx(int state);
	float getLeftDy(int state);
	float getRightDx(int state);
	float getRightDy(int state);

	friend istream& operator>>(istream& is,PieceDescriptor& pd);
};

#endif
