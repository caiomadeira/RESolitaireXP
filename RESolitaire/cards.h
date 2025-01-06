#pragma once

#ifndef cards_h
#define cards_h

#include "common.h"

typedef BOOL(WINAPI *cdtInitPtr)(int *width, int *height);
typedef BOOL(WINAPI *cdtDrawPtr)(HDC hdc, int x, int y, int cardID, DWORD flags);
typedef BOOL(WINAPI *cdtAnimatePtr)(int animationType, int param1, int param2, int speed);
typedef BOOL(WINAPI *cdtTermPtr)();

extern cdtInitPtr cdtInit;
extern cdtDrawPtr cdtDraw;
extern cdtAnimatePtr cdtAnimate;
extern cdtTermPtr cdtTerm;

extern HINSTANCE hCardsDll;

extern int cardWidth, cardHeight;

BOOL LoadCardsDll(void);
void RECdtDraw(HDC hdc, int x, int y, int id, DWORD flags);


#endif