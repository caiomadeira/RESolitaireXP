#include<windows.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include <CommCtrl.h> // INITCOMMONCONTROLSEX

#pragma comment(lib, "cards.dll")

// GLOBALS
HWND hwnd;
HACCEL hAccTable;
HINSTANCE hInstance;

// Undefined Globals - Mapping
LPWSTR* lpBuffer;    //DAT_01007220
int width = 0; // &DAT_01007308 = 00000000h
int height = 0;// &DAT_0100730c = 00000000h

LONG DAT_01007368 = 0;
LONG DAT_01007310 = 0;
int DAT_01007168 = 0;
int DAT_01007364 = 0;

// SSet for cards dll cdtInit
typedef BOOL(WINAPI *cdtInitPtr)(int* width, int* height);
int MainGameWindow(HINSTANCE hinstance, int nCmdShow, short* param_3, int param_4);


HINSTANCE LoadCardDll(const char* path)
{
    HINSTANCE hCardsDLL = LoadLibrary(TEXT("cards.dll"));
    if (!hCardsDLL)
    {
        printf("Falha ao carregar cards.dll. Erro: 0x%x\n", GetLastError());
        return NULL;
    }
    return hCardsDLL;
}

// uint tempwWinMain(HINSTANCE param_1,int param_2,undefined4 param_3,int param_4)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
  LPWSTR commandLine;
  UINT result;
  int isAccelerated;
  BOOL messageStatus;
  tagMSG msg;
  
  commandLine = GetCommandLineW();
  int param_4 = 1;
  //wchar_t (usado por LPWSTR) e short têm o mesmo tamanho (2 bytes). 
  result = MainGameWindow(hInstance, nCmdShow, (short*)commandLine, param_4);
  if (result != 0) {
    msg.wParam = 1;
    while(true) 
    {
      messageStatus = GetMessageW(&msg,(HWND)nullptr,0,0);
      if (messageStatus == 0) break;
      isAccelerated = TranslateAcceleratorW(hwnd, hAccTable, &msg);
      if (isAccelerated == 0) 
      {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
    }
    result = msg.wParam != 0;
  }
  return result;
}

void PassesToLoadString(LPWSTR lpBuffer, USHORT uID, int cchBufferMax)
{
    LoadStringW(hInstance, (UINT)uID, lpBuffer, cchBufferMax);
    return;
}

int MainGameWindow(HINSTANCE hinstance, int nCmdShow, short* param_3, int param_4)
{ 
  // LOAD DLL
    HINSTANCE hCardsDll = LoadCardDll("cards.dll");
    cdtInitPtr cdtInit = (cdtInitPtr)GetProcAddress(hCardsDll, "cdtInit");
    if (!cdtInit)
    {
        printf("Falha ao obter nomes de cards.dll. Erro: 0x%x\n", GetLastError());
        FreeLibrary(hCardsDll);
        return 1;
    }

    // INIT FUNCTION
    short currentChar;
    ATOM clAtom;
    int initResult;
    HDC hdc;
    UINT_PTR timerId;
    short *commandPtr;
    WNDCLASSEXW *wndClassPtr;
    time_t currentTime;
    tagTEXTMETRICW textMetrics;
    WCHAR windowTitleBuffer [20];
    WNDCLASSEXW wc;
    INITCOMMONCONTROLSEX commonControls;
    HCURSOR cursor;
    tagRECT windowRect;
    byte flagCursorVisibility;
    
    hInstance = hinstance;

    // **************************************************
    USHORT uID = 300;
    int cchBufferMax = 0x32;
    PassesToLoadString((LPWSTR)&lpBuffer,300,0x32);
    // **************************************************

    initResult = cdtInit(&width, &height);
    if (initResult != 0) {
        cursor = LoadCursorW((HINSTANCE)0x0, (LPCWSTR)0x7f00);
        hdc = GetDC((HWND)0x0);
        if (hdc != (HDC)0x0) 
        {
            GetTextMetricsW(hdc,&textMetrics);
            DAT_01007368 = textMetrics.tmHeight;
            DAT_01007310 = textMetrics.tmMaxCharWidth;
            initResult = GetDeviceCaps(hdc,0x18);
            if (initResult == 2) 
            {
                DAT_01007168 = 1;
            }
            int _DAT_01007360 = GetDeviceCaps(hdc,8);
            DAT_01007364 = GetDeviceCaps(hdc,10);
            DAT_0100718c = (UINT)(DAT_01007364 < 300);
            
            if (DAT_0100718c != 0) 
                DAT_0100730c = DAT_0100730c / 2;

            ReleaseDC((HWND)0x0,hdc);
            DAT_01007348 = (-(uint)(DAT_01007168 != 0) & 0xff7fff) + 0x8000;
            DAT_0100737c = CreateSolidBrush(DAT_01007348);
            currentTime = time((time_t *)0x0);
            srand((UINT)currentTime & 0xffff);
            PassesToLoadString((LPWSTR)&windowTitleBuffer, 100, 10);
            PassesToLoadString((LPWSTR)&DAT_010072a0,0x65,0x32);
            PassesToLoadString(windowTitleBuffer,0x67,0x14);
            DAT_01007160 = RegisterWindowMessageW(windowTitleBuffer);
            flagCursorVisibility = 0;
            currentChar = *commandLine;
            commandPtr = commandLine;
            while (currentChar != 0) {
                if ((currentChar == 0x2f) && (commandPtr[1] == 0x49)) {
                flagCursorVisibility = 1;
                break;
                }
                commandPtr = commandPtr + 1;
                currentChar = *commandPtr;
        }
        commonControls.dwSize = 8;
        commonControls.dwICC = 0x16fd;
        InitCommonControlsEx(&commonControls);
        hIcon = LoadIconW(hInstance,(LPCWSTR)0x1f4);
        _DAT_0100720c = (HICON)LoadImageW(hInstance,(LPCWSTR)0x1f4,1,0x10,0x10,0);
        if (nCmdShow == 0) {
            wndClassPtr = &wc;
            for (initResult = 0xc; initResult != 0; initResult = initResult + -1) {
            wndClassPtr->cbSize = 0;
            wndClassPtr = (WNDCLASSEXW *)&wndClassPtr->style;
            }
            wc.hInstance = hInstance;
            wc.hIcon = hIcon;
            wc.hCursor = cursor;
            wc.hbrBackground = DAT_0100737c;
            wc.cbSize = 0x30;
            wc.style = 0x2008;
            wc.lpfnWndProc = FUN_010016bd;
            wc.lpszMenuName = (LPCWSTR)0x1;
            wc.lpszClassName = u_Tarot_Sol._0100700c;
            wc.hIconSm = _DAT_0100720c;
            clAtom = RegisterClassExW(&wc);
            if (clAtom == 0) goto LAB_01001e0f;
        }
        windowRect.top = 0;
        windowRect.left = 0;
        DAT_01007338 = DAT_01007308 / 8 + 3;
        windowRect.right = DAT_01007308 * 7 + DAT_01007338 * 8;
        windowRect.bottom = DAT_0100730c << 2;
        AdjustWindowRect(&windowRect,0xcf0000,1);
        windowRect.bottom = windowRect.bottom - windowRect.top;
        windowRect.right = windowRect.right - windowRect.left;
        if (DAT_01007364 < windowRect.bottom) {
            windowRect.bottom = DAT_01007364;
        }
        hWnd = CreateWindowExW(0,u_Tarot_Sol._0100700c,(LPCWSTR)&WindowTitle,
                                (-(uint)flagCursorVisibility & 0x20000000) + 0x2cf0000,-0x80000000,0,
                                windowRect.right,windowRect.bottom,(HWND)0x0,(HMENU)0x0,hInstance,
                                (LPVOID)0x0);
        if (hWnd != (HWND)0x0) {
            FUN_01001504((uint *)&commandLine);
            timerId = SetTimer(hWnd,0x29a,0xfa,(TIMERPROC)&LAB_0100142b);
            if (timerId != 0) {
            thunk_FUN_01005581();
            FUN_010026f8((int)commandLine);
            ShowWindow(hWnd,param_4);
            UpdateWindow(hWnd);
            hAccTable = LoadAcceleratorsW(hinstance,L"HiddenAccel");
            FUN_01005f1e((uint)(nCmdShow == 0));
            if (_DAT_01007020 != 0) {
                FUN_01005c79();
            }
            if ((param_4 != 7) && (param_4 != 6)) {
                PostMessageW(hWnd,0x111,1000,0);
            }
            return 1;
            }
        }
        }
    }
    LAB_01001e0f:
    FUN_010023e2();
    return 0;
}