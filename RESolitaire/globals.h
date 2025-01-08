#ifndef globals_h
#define globals_h

#include <windows.h>

/****** GLOBALS *******/
extern HINSTANCE hInstance;
extern HWND hwnd;
extern HWND statusHwnd;
extern HDC hdc;
extern HMENU hMenu;
extern HICON hIcon;
extern HANDLE _iconHandle;
extern HACCEL hAccTable;

// Main Window Context
// Font vars
extern int fontLargestChar;
extern int fontHeight;

// Device vars
extern int numColors;
extern int visibileWidth;
extern int visibileHeight;

// RegisterWindow
extern UINT registerWindowMsg;

// timer count
extern int timerCount;

/****** CONSTANTS *******/

#define DECK_MAX 51
#define TABLEAU_MAX 7
#define TABLEAU_CARDS_MAX 19
#define PILE_MAX 24
#define FOUNDATION_CARDS_PER_PILE 13
#define SUITS_SIZE 4

/************************/

#endif