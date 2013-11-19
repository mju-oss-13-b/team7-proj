/*
 * QBOS: Matrix.cpp
 *
 * Si hay una clase importante, esa es Matrix. Representa la
 * matriz de bloques, crea y destruye las piezas, bloques y
 * lineas.
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

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Matrix.h"
#include "constants.h"
#include "tge/Util.h"

using std::string;
using std::ostringstream;
using std::istream;
using std::ifstream;
using std::exception;
using std::runtime_error;
using tge::Util;



Matrix::Matrix(Configuration& config,int z):Widget(MATRIX_X,MATRIX_Y,MATRIX_W,MATRIX_H,z)
{

	ticks_=0;
	rnd_=0;
	for(int i=0;i<5;i++)
		blocks_[i]=0;
	falling_piece_=0;
	next_piece_=0;
	level_=points_=num_lines_=0;

	try
	{
		config_=config;

		matrix_=SafeMatrix(MATRIX_ROWS,MATRIX_COLS);
		for(int i=0;i<MATRIX_ROWS;i++)
			for(int j=0;j<MATRIX_COLS;j++)
				matrix_.set(i,j,-1);
		rnd_=new TRandomMersenne(time(0));
		for(int i=0;i<5;i++)
		{
			std::ostringstream ostr;
			ostr<<i;
			blocks_[i]=Util::loadImage(string("images/block")+ostr.str()+".png");
		}

		ifstream in;
		in.open("pieces/t");
		in>>descriptor_[0];
		in.close();
		in.open("pieces/o");
		in>>descriptor_[1];
		in.close();
		in.open("pieces/l");
		in>>descriptor_[2];
		in.close();
		in.open("pieces/i");
		in>>descriptor_[3];
		in.close();
		in.open("pieces/j");
		in>>descriptor_[4];
		in.close();
		in.open("pieces/s");
		in>>descriptor_[5];
		in.close();
		in.open("pieces/z");
		in>>descriptor_[6];
		in.close();

		falling_piece_=makeRandomPiece();
		next_piece_=makeRandomPiece();

	}
	catch(const exception& ex)
	{
		if(rnd_)
			delete rnd_;
		for(int i=0;i<5;i++)
			if(blocks_[i])
				SDL_FreeSurface(blocks_[i]);
		if(falling_piece_)
			delete falling_piece_;
		if(next_piece_)
			delete next_piece_;

		throw runtime_error(string("Matrix::Matrix(...): ")+ex.what());
	}
	widgets_.push_back(falling_piece_);

	Block::blocks_=&blocks_;

	piece_rapidity_.second.setRapidity(1,4);
	piece_rapidity_.first=0;
	blocks_rapidity_.second.setRapidity(4,1);
	blocks_rapidity_.first=0;

	fade_rapidity_.setRapidity(15,1);
	Line::setFadeRapidity(&fade_rapidity_);

	dead_=false;
}

Matrix::~Matrix()
{
	for(int i=0;i<5;i++)
		SDL_FreeSurface(blocks_[i]);

	if(next_piece_)
		delete next_piece_;

	list<Widget*>::iterator i;
	for(i=widgets_.begin();i!=widgets_.end();i++)
		delete *i;

	delete rnd_;
}


bool Matrix::isDead()
{
	return dead_;
}

int Matrix::getLevel()
{
	return level_;
}

int Matrix::getPoints()
{
	return points_;
}

SDL_Surface* Matrix::getNextPiece()
{
	return next_piece_->getSurface();
}

void Matrix::key(SDLKey k)
{
	if(lines_.empty() && falling_blocks_.empty() && falling_piece_)
	{
		if(k==config_.getMoveKey(Configuration::DOWN))
			downFallingPiece(PIECE_YPIXS_DOWN);
		else if(k==config_.getMoveKey(Configuration::LEFT))
		{
			falling_piece_->left();
			if(collidesWithMatrix(falling_piece_->getRects()))
				falling_piece_->right();
			else
				falling_piece_->okMove();
		}
		else if(k==config_.getMoveKey(Configuration::RIGHT))
		{
			falling_piece_->right();
			if(collidesWithMatrix(falling_piece_->getRects()))
				falling_piece_->left();
			else
				falling_piece_->okMove();
		}
		else if(k==config_.getMoveKey(Configuration::ROTATE_CW))
		{
			falling_piece_->rotateLeft();
			if(collidesWithMatrix(falling_piece_->getRects()))
				falling_piece_->rotateRight();
			else
				falling_piece_->okMove();
		}
		else if(k==config_.getMoveKey(Configuration::ROTATE_ACW))
		{
			falling_piece_->rotateRight();
			if(collidesWithMatrix(falling_piece_->getRects()))
				falling_piece_->rotateLeft();
			else
				falling_piece_->okMove();
		}
		else if(k==config_.getMoveKey(Configuration::FALL))
			collapseFallingPiece();
	}
}

void Matrix::ticks(int t)
{
	if(!lines_.empty()) //Si hay alguna linea la hago desaparecer
	{
		list<Line*>::iterator i;
		for(i=lines_.begin();i!=lines_.end();)
		{
			if((*i)->dead()) //linea transparente
			{
				widgets_.remove(*i);
				delete *i;
				i=lines_.erase(i);
			}
			else
			{
				//sumo ticks anteriores
				t+=ticks_;
				ticks_=0;
				(*i)->ticks(t);
				i++;
			}
		}
	}
	else if(!falling_blocks_.empty()) //Hay un conjunto de bloques cayendo
	{
		//sumo ticks sobrantes de la pasada anterior
		blocks_rapidity_.first+=t+ticks_;
		ticks_=0;
		int pixs=blocks_rapidity_.second.calcUnits(blocks_rapidity_.first);
		try
		{
			//Avanzo el bloque de a MAX_PIECE_YPIXS_DOWN pixeles, si la cantidad
			//es mayor el sistema de colisiones podria fallar
			int k=pixs/MAX_PIECE_YPIXS_DOWN;
			int rest=pixs%MAX_PIECE_YPIXS_DOWN;
			for(int i=0;i<k;i++)
			{
				downFallingBlocks(MAX_PIECE_YPIXS_DOWN);
				pixs-=MAX_PIECE_YPIXS_DOWN;
			}
			downFallingBlocks(rest);
		}
		catch(int null_block)
		{
			//No se alcanzaron a ocupar todos los ticks en el bloque que
			//caia, pues este colisiono. Los ticks sobrantes los guardo
			//en ticks_ para la proxima vuelta
			ticks_=blocks_rapidity_.second.calcTicks(pixs);
		}
	}
	else
	{
		//Si no hay pieza cayendo uso next_piece_ y creo la siguiente
		if(!falling_piece_)
		{
			falling_piece_=next_piece_;
			next_piece_=0;
			if(!falling_piece_->canInsert(matrix_))
			{
				dead_=true;
				refresh_areas_.push_back(falling_piece_->insertOnTop(matrix_));
				delete falling_piece_;
				falling_piece_=0;
				return;
			}
			else
			{
				widgets_.push_back(falling_piece_);
				try
				{
					next_piece_=makeRandomPiece();
				}
				catch(const exception& ex) //doh!
				{
					throw runtime_error(string("Matrix::ticks(...): ")+ex.what());
				}
			}
		}
		//sumo ticks sobrantes de la pasada anterior
		piece_rapidity_.first+=t+ticks_;
		ticks_=0;
		int pixs=piece_rapidity_.second.calcUnits(piece_rapidity_.first);
		try
		{
			//Avanzo la pieza de a MAX_PIECE_YPIXS_DOWN pixeles, si la cantidad
			//es mayor el sistema de colisiones podria fallar
			int n=pixs/MAX_PIECE_YPIXS_DOWN;
			int rest=pixs%MAX_PIECE_YPIXS_DOWN;
			for(int i=0;i<n;i++)
			{
				downFallingPiece(MAX_PIECE_YPIXS_DOWN);
				pixs-=MAX_PIECE_YPIXS_DOWN;
			}
			downFallingPiece(rest);
		}
		catch(int null_piece)
		{
			//No se alcanzaron a ocupar todos los ticks en la pieza que
			//caia, la pieza colisiono antes. Los ticks sobrantes los guardo
			//en ticks_ para la proxima vuelta
			ticks_=piece_rapidity_.second.calcTicks(pixs);
		}
		catch(const exception& ex) //!ups
		{
			throw runtime_error(string("Matrix::ticks(...): ")+ex.what());
		}
	}
}



//Implementacion de tge::Widget::getUpdateRects(...)

void Matrix::getUpdateRects(Uta* uta,const SDL_Rect* parent_rect)
{
	//recupero rectas de todo lo que cae
	list<Widget*>::iterator w;
	for(w=widgets_.begin();w!=widgets_.end();w++)
		(*w)->getUpdateRects(uta);

	//recupero rectas de los espacios de la matriz que
	//no han sido actualizados y que han cambiado
	list<SDL_Rect>::iterator i;
	for(i=refresh_areas_.begin();i!=refresh_areas_.end();i++)
		uta->addRect(*i);
	refresh_areas_.clear();
}


//Implementacion de tge::Widget::paint(...)

void Matrix::paint(SDL_Surface* screen,const SDL_Rect* parent_rect)
{
	for(int i=0;i<MATRIX_ROWS;i++)
		for(int j=0;j<MATRIX_COLS;j++)
			if(matrix_.get(i,j)>=0) //pinto la matriz
			{
				SDL_Rect r;
				r.x=j*BLOCK_DIM+MATRIX_X;
				r.y=i*BLOCK_DIM+MATRIX_Y;
				SDL_BlitSurface(blocks_[matrix_.get(i,j)],0,screen,&r);
			}

	list<Widget*>::iterator w;
	for(w=widgets_.begin();w!=widgets_.end();w++)
		(*w)->paint(screen); //pinto los objetos que caen y las lineas
}



//Metodos protegidos

bool Matrix::collidesWithMatrix(const vector<SDL_Rect>& rects)
{
	vector<SDL_Rect>::const_iterator k;
	for(k=rects.begin();k!=rects.end();k++)
	{
		if((*k).x<MATRIX_X || (*k).x+(*k).w>MATRIX_X+MATRIX_W
				|| (*k).y<MATRIX_Y || (*k).y+(*k).h>MATRIX_Y+MATRIX_H)
			return true;

		int row,col,rows,cols;

		rows=(*k).h/BLOCK_DIM;
		cols=(*k).w/BLOCK_DIM;

		row=((*k).y-MATRIX_Y)/BLOCK_DIM;
		if(((*k).y-MATRIX_Y)%BLOCK_DIM)
			rows++;

		col=((*k).x-MATRIX_X)/BLOCK_DIM;
		if(((*k).x-MATRIX_X)%BLOCK_DIM)
			cols++;

		rows+=row;
		cols+=col;
		for(int i=row;i<rows;i++)
			for(int j=col;j<cols;j++)
				if(matrix_.get(i,j)!=-1)
					return true;
	}
	return false;
}

void Matrix::downFallingPiece(int pixs)
{
	if(!falling_piece_)
		throw 0;
	falling_piece_->down(pixs);
	if(collidesWithMatrix(falling_piece_->getRects()))
		try
		{
			collapseFallingPiece();
		}
		catch(const exception& ex)
		{
			throw runtime_error(string("Matrix::downFallingPiece(...): ")+ex.what());
		}
}

void Matrix::downFallingBlocks(int pixs)
{
	list<Block*>::iterator i;
	for(i=falling_blocks_.begin();i!=falling_blocks_.end();)
	{
		(*i)->down(pixs);
		if(collidesWithMatrix((*i)->getRects()))
		{
			refresh_areas_.push_back((*i)->collapse(matrix_));
			widgets_.remove(*i);
			delete *i;
			i=falling_blocks_.erase(i);
		}
		else
			i++;
	}
}

void Matrix::collapseFallingPiece()
{
	int row,rows;
	refresh_areas_.push_back(falling_piece_->collapse(matrix_));
	widgets_.remove(falling_piece_);
	row=(falling_piece_->getY()-MATRIX_Y)/BLOCK_DIM;
	rows=falling_piece_->getHeight()/BLOCK_DIM;
	delete falling_piece_;
	falling_piece_=0;

	//matrix_.print();

	try
	{
		updatePointsAndLevel(findLines(row,rows));
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Matrix::collapseFallingPiece(...): ")+ex.what());
	}
}

int Matrix::findLines(int row,int rows)
{
	int lines=0;
	for(int i=row;i<row+rows;i++)
	{
		int j;
		for(j=0;j<MATRIX_COLS;j++)
			if(matrix_.get(i,j)==-1)
				break;
		if(j==MATRIX_COLS)
		{
			Line* line=0;
			try
			{
				line=new Line(i,2,matrix_,blocks_);
				lines_.push_back(line);
				widgets_.push_back(line);
				lines++;
			}
			catch(const exception& ex)
			{
				throw runtime_error(string("Matrix::findLines(...): ")+ex.what());
			}

			try
			{
				Block* block=new Block(i,2,matrix_);
				falling_blocks_.push_back(block);
				widgets_.push_back(block);
			}
			catch(const exception& ex)
			{
			}
		}
	}
	return lines;
}

Piece* Matrix::makeRandomPiece()
{
	int b=rnd_->IRandom(0,4); //block index
	int p=rnd_->IRandom(0,6); //tipo de pieza
	int s=rnd_->IRandom(0,3); //state
	Piece* piece=0;
	try
	{
		piece=new Piece(
			MATRIX_X+(MATRIX_COLS/2)*BLOCK_DIM,MATRIX_Y
			,1,(Piece::State)s,b,blocks_[b],&descriptor_[p]);
		return piece;
	}
	catch(const exception& ex)
	{
		throw runtime_error(string("Matrix::makeRandomPiece(...): ")+ex.what());
	}
}

void Matrix::updatePointsAndLevel(int lines)
{
	if(!lines)
		return;
	num_lines_+=lines;
	level_=num_lines_/20;
	points_+=lines*(10+level_)+4*(lines-1)*(lines-1);
	switch(level_)
	{
		case 0:
			piece_rapidity_.second.setRapidity(1,6);
			break;
		case 1:
			piece_rapidity_.second.setRapidity(1,5);
			break;
		case 2:
			piece_rapidity_.second.setRapidity(1,4);
			break;
		case 3:
			piece_rapidity_.second.setRapidity(1,3);
			break;
		case 4:
			piece_rapidity_.second.setRapidity(1,2);
			break;
		case 5:
			piece_rapidity_.second.setRapidity(1,1);
			break;
		case 6:
			piece_rapidity_.second.setRapidity(2,1);
			break;
		case 7:
			piece_rapidity_.second.setRapidity(3,1);
			break;
		case 8:
			piece_rapidity_.second.setRapidity(4,1);
			break;
		case 9:
			piece_rapidity_.second.setRapidity(5,1);
			break;
		case 10:
			piece_rapidity_.second.setRapidity(6,1);
			break;
		default:
			piece_rapidity_.second.setRapidity(6,1);
	}
}

