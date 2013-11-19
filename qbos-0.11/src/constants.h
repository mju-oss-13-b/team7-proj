/*
 * QBOS: constants.h
 *
 * constantes x_O
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

#ifndef _CONSTANTS_H
#define _CONSTANTS_H


#define BPP 32
#if SDL_BYTEORDER==SDL_BIG_ENDIAN
	#define RMASK 0xff000000
	#define GMASK 0x00ff0000
	#define BMASK 0x0000ff00
	#define AMASK 0x000000ff
#else
	#define RMASK 0x000000ff
	#define GMASK 0x0000ff00
	#define BMASK 0x00ff0000
	#define AMASK 0xff000000
#endif

#define REPEAT_DELAY 100

#define BLOCK_DIM 25
#define MATRIX_X 5
#define MATRIX_Y 5
#define MATRIX_W 300
#define MATRIX_H 550
#define MATRIX_ROWS 22
#define MATRIX_COLS 12
#define PIECE_YPIXS_DOWN 10
#define MAX_PIECE_YPIXS_DOWN  24

#define PANEL_X 305
#define PANEL_Y 5
#define PANEL_W 126
#define PANEL_H 550

#define HS_ROW_X 10
#define HS_ROW_Y0 80
#define HS_ROW_MARGIN_X 5
#define HS_ROW_MARGIN_Y 3

#define HS_NUM 10

#define SETUP_WINDOW_X 10
#define SETUP_WINDOW_Y0 80
#define SETUP_WINDOW_Y1 325

#define SETUP_WINDOW_IN_X0 8
#define SETUP_WINDOW_IN_X1 222
#define SETUP_WINDOW_IN_MARGIN_X 10
#define SETUP_WINDOW_IN_MARGIN_Y 10


#endif
