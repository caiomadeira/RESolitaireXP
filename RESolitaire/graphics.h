#pragma once
#ifndef graphics_h
#define graphics_h

#include "common.h"

extern HBRUSH backgroundBrush;

HBRUSH defaultBackGround(void);
BOOL customErrorMsgBox(CONST WCHAR* msg, CONST WCHAR* title);
HMENU CreateMainMenu(void);
void DrawBitmap(HDC hdc, LPCWSTR bitmapPath, int x, int y);

#endif