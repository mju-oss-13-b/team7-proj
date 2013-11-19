/*
 * QBOS: tge/Rapidity.cpp
 *
 * Clase para representar rapidez. Permite calcular calcular unidades
 * en base a ticks y viceversa.
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

#include "Rapidity.h"



tge::Rapidity::Rapidity()
{
	units_=0;
	ticks_=1;
}

tge::Rapidity::Rapidity(int units,int ticks)
{
	units_=units;
	ticks_=ticks;
}

tge::Rapidity::~Rapidity()
{
}


void tge::Rapidity::setRapidity(int units,int ticks)
{
	units_=units;
	ticks_=ticks;
}

int tge::Rapidity::calcUnits(int& ticks)
{
	int units=(ticks/ticks_)*units_;
	ticks=ticks%ticks_;
	return units;
}

int tge::Rapidity::calcTicks(int& units)
{
	int ticks=(units/units_)*ticks_;
	units=units%units_;
	return ticks;
}

