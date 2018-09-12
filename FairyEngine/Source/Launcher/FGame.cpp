/*
 * ------------------------------------------------------------------------
 *  Name:   FGame.cpp
 *  Desc:   Logical game object.
 *  Author: Yish
 *  Date:   2015/5/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FGame.h"

FGame* g_pFairyGame = NULL;

FGame::FGame()
{
	g_pFairyGame = this;
}

FGame::~FGame()
{
	g_pFairyGame = NULL;
}