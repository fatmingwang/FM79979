#include "stdafx.h"
#include "FishGameApp.h"

cLanguageFile*g_pLanguageFile = nullptr;

bool	InitOnceObject()
{
	DeleteOnceObject();
	g_pLanguageFile = new cLanguageFile("");
	return true;
}

bool	DeleteOnceObject()
{
	SAFE_DELETE(g_pLanguageFile);
	return true;
}