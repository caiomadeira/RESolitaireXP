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
/************************/

#endif