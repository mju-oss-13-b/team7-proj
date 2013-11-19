/*
 * QBOS: Block.cpp
 *
 * Un bloque de cubos que cae. Los pedazos de matriz que quedan
 * en el aire despues de que se hace una linea se hacen caer en
 * forma de Block.
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
#include "Block.h"
#include "constants.h"
#include "tge/Util.h"

using std::runtime_error;
using tge::Util;



SDL_Surface* (*Block::blocks_)[5]=0;

Block::Block(int row,int z,SafeMatrix& matrix)
				:FallingObject(0,0,0,0,z)
{
	int ro;
	for(ro=0;ro<row;ro++)
	{
		int j;
		for(j=0;j<MATRIX_COLS && matrix.get(ro,j)==-1;j++);
		if(j<MATRIX_COLS)
			break;
	}
	if(ro>=row)
		throw runtime_error("Block::Block(): Block vacio");

	int co;
	for(co=0;co<MATRIX_COLS;co++)
	{
		int j;
		for(j=ro;j<row && matrix.get(j,co)==-1;j++);
		if(j<row)
			break;
	}
	int cf;
	for(cf=MATRIX_COLS-1;cf>=co;cf--)
	{
		int j;
		for(j=0;j<row && matrix.get(j,cf)==-1;j++);
		if(j<row)
			break;
	}
	if(co>cf)
		throw runtime_error("Block::Block(): Block vacio");

	matrix_=SafeMatrix(row-ro,cf-co+1);
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
		{
			matrix_.set(r,c,matrix.get(ro+r,co+c));
			matrix.set(ro+r,co+c,-1);
		}

	setX(co*BLOCK_DIM+MATRIX_X);
	setY(ro*BLOCK_DIM+MATRIX_Y);

	setWidth(matrix_.getCols()*BLOCK_DIM);
	setHeight(matrix_.getRows()*BLOCK_DIM);

	//matrix_.print();

	rects_.reserve(matrix_.getCols());
	refreshRects();
}

Block::~Block()
{
}



//Implementacion de tge::Widget::paint(...)

void Block::paint(SDL_Surface* screen,const SDL_Rect* parent_rect)
{
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
		{
			if(matrix_.get(r,c)>=0)
			{
				SDL_Rect rect;
				rect.x=getX()+c*BLOCK_DIM;
				rect.y=getY()+r*BLOCK_DIM;
				SDL_BlitSurface((*blocks_)[matrix_.get(r,c)],0,screen,&rect);
			}
		}
	updateLastPaintedRect();
}



//Implementacion de FallingObject::refreshRects()

void Block::refreshRects()
{
	Uint32 index=0;
	for(int i=0;i<matrix_.getRows();i++)
	{
		int j=0;
		while(j<matrix_.getCols())
		{
			int co=getNext(i,j);
			int cf=getNext(i,co,true);
			if(co<cf)
			{
				SDL_Rect rect;
				rect.x=co*BLOCK_DIM+getX();
				rect.y=i*BLOCK_DIM+getY();
				rect.w=(cf-co)*BLOCK_DIM;
				rect.h=BLOCK_DIM;
				if(rects_.size()<=index)
					rects_.push_back(rect);
				else
					rects_[index]=rect;
				index++;
			}
			j=cf;
		}
	}
}

void Block::putIn(int row,int col,SafeMatrix& matrix)
{
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
			if(matrix_.get(r,c)>=0 && row+r>=0)
				matrix.set(row+r,col+c,matrix_.get(r,c));
}

int Block::getMinimumDeltaY(SafeMatrix& matrix)
{
	int row,col,min;
	row=(getY()-MATRIX_Y)/BLOCK_DIM;
	col=(getX()-MATRIX_X)/BLOCK_DIM;
	min=MATRIX_ROWS;
	for(int i=0;i<matrix_.getCols();i++)
	{
		int j;
		for(j=matrix_.getRows()-1;j>=0 && matrix_.get(j,i)==-1;j--);
		if(j>=0)
			min=Util::min(min,matrix.getLowerRow(row+j,col+i)-(row+j+1));
	}
	return min;
}



//Metodos privados

int Block::getNext(int row,int col,bool free)
{
	int i;
	for(i=col;i<matrix_.getCols();i++)
		if(free && matrix_.get(row,i)==-1
				|| !free && matrix_.get(row,i)!=-1)
			break;
	return i;
}

