# Copyright (C) 2006 Jorge Riquelme Santana <totex@totex.cl>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


bin_PROGRAMS = qbos
qbos_SOURCES = \
		Block.cpp             MenuArrow.cpp \
		Configuration.cpp     MenuItem.cpp \
		CreditsScreen.cpp     MenuScreen.cpp \
		FallingObject.cpp     NextPiece.cpp \
		GameScreen.cpp        Piece.cpp \
		GetKey.cpp            PieceDescriptor.cpp \
		HighScore.cpp         qbos.cpp \
		HighScoresScreen.cpp  SafeMatrix.cpp \
		Line.cpp              Screen.cpp \
		Matrix.cpp            SetupScreen.cpp

noinst_HEADERS = \
		Block.h             Matrix.h \
		Configuration.h     MenuArrow.h \
		constants.h         MenuItem.h \
		CreditsScreen.h     MenuScreen.h \
		FallingObject.h     NextPiece.h \
		GameScreen.h        PieceDescriptor.h \
		GetKey.h            Piece.h \
		HighScore.h         SafeMatrix.h \
		HighScoresScreen.h  Screen.h \
		Line.h              SetupScreen.h

INCLUDES = -DDATADIR=\""$(pkgdatadir)"\"

SUBDIRS = tge randomc

qbos_LDADD = tge/libtge.a randomc/librandomc.a
