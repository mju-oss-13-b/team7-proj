/*
 * QBOS: Piece.cpp
 *
 * Una pieza :p
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

#include <string>
#include <stdexcept>
#include "Piece.h"
#include "constants.h"
#include "tge/Util.h"

using std::string;
using std::exception;
using std::runtime_error;
using tge::Util;



Piece::Piece(int x,int y,int z,State state,int block_index,SDL_Surface* block
			,PieceDescriptor* descriptor):FallingObject(x,y,0,0,z)
{
	state_=state;
	block_index_=block_index;
	block_=block;

rotateMatrix[NORMAL][LEFT]=LEFT;
rotateMatrix[NORMAL][RIGHT]=RIGHT;
rotateMatrix[LEFT][LEFT]=INVERSE;
rotateMatrix[LEFT][RIGHT]=NORMAL;
rotateMatrix[INVERSE][LEFT]=RIGHT;
rotateMatrix[INVERSE][RIGHT]=LEFT;
rotateMatrix[RIGHT][LEFT]=NORMAL;
rotateMatrix[RIGHT][RIGHT]=INVERSE;

	descriptor_=descriptor;

	try
	{
		matrix_=descriptor_->getSafeMatrix(state_);
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Piece::Piece(): ")+ex.what());
	}

	setWidth(matrix_.getCols()*BLOCK_DIM);
	setHeight(matrix_.getRows()*BLOCK_DIM);

	if(matrix_.getRows()>matrix_.getCols())
		rects_.resize(matrix_.getCols());
	else
		rects_.resize(matrix_.getRows());
	refreshRects();
}

Piece::~Piece()
{
}


void Piece::left()
{
	vector<SDL_Rect>::iterator i;
	for(i=rects_.begin();i!=rects_.end();i++)
		(*i).x-=BLOCK_DIM;
	setX(getX()-BLOCK_DIM);
}

void Piece::right()
{
	vector<SDL_Rect>::iterator i;
	for(i=rects_.begin();i!=rects_.end();i++)
		(*i).x+=BLOCK_DIM;
	setX(getX()+BLOCK_DIM);
}

void Piece::rotateLeft()
{
	setX(getX()+(int)(BLOCK_DIM*descriptor_->getLeftDx(state_)));
	setY(getY()+(int)(BLOCK_DIM*descriptor_->getLeftDy(state_)));
	state_=(State)rotateMatrix[state_][LEFT];
	matrix_.rotateLeft();
	setWidth(matrix_.getCols()*BLOCK_DIM);
	setHeight(matrix_.getRows()*BLOCK_DIM);
	refreshRects();
}

void Piece::rotateRight()
{
	setX(getX()+(int)(BLOCK_DIM*descriptor_->getRightDx(state_)));
	setY(getY()+(int)(BLOCK_DIM*descriptor_->getRightDy(state_)));
	state_=(State)rotateMatrix[state_][RIGHT];
	matrix_.rotateRight();
	setWidth(matrix_.getCols()*BLOCK_DIM);
	setHeight(matrix_.getRows()*BLOCK_DIM);
	refreshRects();
}

SDL_Surface* Piece::getSurface()
{
	SDL_Surface* surface;
	try
	{
		SDL_Color colorkey;
		colorkey.r=colorkey.b=255;
		colorkey.g=0;
		surface=Util::createSurface(getWidth(),getHeight(),&colorkey);
		SDL_SetColorKey(surface,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(surface->format,255,0,255));
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Piece::getSurface(...): ")+ex.what());
	}

	SDL_Rect rect;
	int r,c;
	for(r=rect.y=0;r<matrix_.getRows();r++,rect.y+=BLOCK_DIM)
		for(c=rect.x=0;c<matrix_.getCols();c++,rect.x+=BLOCK_DIM)
			if(matrix_.get(r,c))
				SDL_BlitSurface(block_,0,surface,&rect);
	return surface;
}

void Piece::okMove()
{
	setUpdate(true);
}

bool Piece::canInsert(SafeMatrix& matrix)
{
	int row=(getY()-MATRIX_Y)/BLOCK_DIM;
	int col=(getX()-MATRIX_X)/BLOCK_DIM;
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
			if(matrix_.get(r,c) && matrix.get(row+r,col+c)!=-1)
				return false;
	return true;
}

SDL_Rect Piece::insertOnTop(SafeMatrix& matrix)
{
	int row=-matrix_.getRows()+1;
	
	while(true) //señor, no me dejes caer en el loop infinito y librame del leak, amen
	{
		if(canInsert(matrix,row))
			row++;
		else
		{
			row--;
			break;
		}
	}

	int col=(getX()-MATRIX_X)/BLOCK_DIM;
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
			if(r>=0 && matrix_.get(r,c))
				matrix.set(row+r,col+c,block_index_);

	return Util::mergeRects(getLastPaintedRect(),getRect());
}



//Implementacion tge::Widget::pait(...)

void Piece::paint(SDL_Surface* screen,const SDL_Rect* parent_rect)
{
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
		{
			if(matrix_.get(r,c))
			{
				SDL_Rect rect;
				rect.x=getX()+c*BLOCK_DIM;
				rect.y=getY()+r*BLOCK_DIM;
				SDL_BlitSurface(block_,0,screen,&rect);
			}
		}
	updateLastPaintedRect();
}



//Metodos privados

bool Piece::canInsert(SafeMatrix& matrix,int row)
{
	int col=(getX()-MATRIX_X)/BLOCK_DIM;
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
			if(row+r>=0 && matrix_.get(r,c) && matrix.get(row+r,col+c)!=-1)
				return false;
	return true;
}

void Piece::refreshRects()
{
	vector<SDL_Rect>::iterator t=rects_.begin();
	if(matrix_.getRows()>matrix_.getCols())
	{
		for(int i=0;i<matrix_.getCols();i++)
		{
			int j,k;
			for(j=0;j<matrix_.getRows() && !matrix_.get(j,i);j++);
			for(k=j;k<matrix_.getRows() && matrix_.get(k,i);k++);
			(*t).x=getX()+i*BLOCK_DIM;
			(*t).y=getY()+j*BLOCK_DIM;
			(*t).w=BLOCK_DIM;
			(*t).h=(k-j)*BLOCK_DIM;
			t++;
		}
	}
	else
	{
		for(int i=0;i<matrix_.getRows();i++)
		{
			int j,k;
			for(j=0;j<matrix_.getCols() && !matrix_.get(i,j);j++);
			for(k=j;k<matrix_.getCols() && matrix_.get(i,k);k++);
			(*t).x=getX()+j*BLOCK_DIM;
			(*t).y=getY()+i*BLOCK_DIM;
			(*t).w=(k-j)*BLOCK_DIM;
			(*t).h=BLOCK_DIM;
			t++;
		}
	}
}

void Piece::putIn(int row,int col,SafeMatrix& matrix)
{
	for(int r=0;r<matrix_.getRows();r++)
		for(int c=0;c<matrix_.getCols();c++)
			if(matrix_.get(r,c) && row+r>=0)
				matrix.set(row+r,col+c,block_index_);
}

int Piece::getMinimumDeltaY(SafeMatrix& matrix)
{
	int row,col,min;
	row=(getY()-MATRIX_Y)/BLOCK_DIM;
	col=(getX()-MATRIX_X)/BLOCK_DIM;
	min=MATRIX_ROWS;
	for(int i=0;i<matrix_.getCols();i++)
	{
		int j;
		for(j=matrix_.getRows()-1;!matrix_.get(j,i);j--);
		min=Util::min(min,matrix.getLowerRow(row+j,col+i)-(row+j+1));
	}
	return min;
}

