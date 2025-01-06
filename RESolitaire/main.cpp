// RESolitaireXP.cpp : Define o ponto de entrada para o aplicativo.
// g++ main.cpp -o RESolitaireXP.exe -mwindows -luser32 -lgdi32 -lcomctl32

#define UNICODE
#define _UNICODE
#include "common.h"

#pragma comment(lib, "comctl32.lib")

typedef BOOL(WINAPI *cdtInitPtr)(int *width, int *height);
typedef BOOL(WINAPI *cdtDrawPtr)(HDC hdc, int x, int y, int cardID, DWORD flags);
typedef BOOL(WINAPI *cdtAnimatePtr)(int animationType, int param1, int param2, int speed);
typedef BOOL(WINAPI *cdtTermPtr)();
HBRUSH defaultBackGround(void);
BOOL customErrorMsgBox(CONST WCHAR* msg, CONST WCHAR* title);
HMENU CreateMainMenu(void);
void customCdtDraw(HDC hdc, int x, int y, int id, DWORD flags);
void DrawInitialDeck(void);
// HBRUSH fillBackGround(void);
// GLOBALS
HWND hwnd;
HDC hdc;
HBRUSH backgroundBrush;
HMENU hMenu;
// ----------------
cdtInitPtr cdtInit;
cdtDrawPtr cdtDraw;
cdtAnimatePtr cdtAnimate;
cdtTermPtr cdtTerm;

HINSTANCE hCardsDll;
int width, height;

typedef enum 
{
    ACE   = 0,
    TWO   = 1,
    THREE = 2,
    FOUR  = 3,
    FIVE  = 4,
    SIX   = 5,
    SEVEN = 6,
    EIGHT = 7,
    NINE  = 8,
    TEN   = 9,
    JACK  = 10,
    QUEEN = 11,
    KING  = 12
} CardsID;


BOOL LoadCardsDll()
{
    hCardsDll = LoadLibrary(TEXT("cards_original.dll"));
    if (!hCardsDll) {
        DWORD errorCode = GetLastError();
        wchar_t errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errorMsg, 256, NULL);
        MessageBox(NULL, errorMsg, L"Erro ao carregar cards.dll!", MB_OK);
        return FALSE;
    }

    cdtInit = (cdtInitPtr)GetProcAddress(hCardsDll, "cdtInit");
    cdtDraw = (cdtDrawPtr)GetProcAddress(hCardsDll, "cdtDraw");

    if (!cdtInit) {
        MessageBox(NULL, L"Error in cdtInit.", L"Erro", MB_OK);
        return FALSE;
    }

    if (!cdtDraw)
    {
        MessageBox(NULL, L"Error in cdtDraw.", L"Erro", MB_OK);
        return FALSE;
    }

    cdtAnimatePtr cdtAnimate = (cdtAnimatePtr)GetProcAddress(hCardsDll, "cdtAnimate");
    cdtTermPtr cdtTerm = (cdtTermPtr)GetProcAddress(hCardsDll, "cdtTerm");

    if (!cdtAnimate || !cdtTerm)
    {
        MessageBox(NULL, L"Error in cdtAnimate or cdtTerm.", L"Erro", MB_OK);
        return FALSE;
    }


    cdtInit(&width, &height);
    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //HDC hdc;
    PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);  // Obter HDC válido
        if (hdc) 
        {
            // Tentativa de desenhar uma carta
            DrawInitialDeck();
            EndPaint(hwnd, &ps);  // Finalizar o HDC
        } 
        else 
        {
            MessageBox(NULL, L"Erro ao obter HDC!", L"Erro", MB_OK);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            MessageBox(hwnd, L"Novo arquivo criado!", L"Menu: Arquivo", MB_OK);
            break;
        
        default:
            break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ATOM regClass;
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Solitaire Window XP";
    wc.style = 0x2008;
    wc.hbrBackground = defaultBackGround();
    wc.lpszMenuName = (LPCWSTR)0X1;

    regClass = RegisterClass(&wc);
    if (!regClass)
        customErrorMsgBox(L"Register class error", wc.lpszClassName);

    hwnd = CreateWindowEx(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 
    CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (hwnd != NULL)
    {
        if (!LoadCardsDll()) return -1;
        hMenu = CreateMainMenu();
        if (hMenu == nullptr) 
            customErrorMsgBox(L"Error creating menu", wc.lpszClassName);
        else
            SetMenu(hwnd, hMenu);
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    FreeLibrary(hCardsDll);
    DeleteObject(backgroundBrush);
    return 0;
}

HBRUSH defaultBackGround(void)
{
    if (!backgroundBrush)
        backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));
    return backgroundBrush;
}

HBRUSH fillBackGround(void)
{
    RECT rect;
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 128, 0)); // Cor de fundo (verde)
    if (hBrush)
    {
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        return hBrush;
    }
    return FALSE;
}

BOOL customErrorMsgBox(CONST WCHAR* msg, CONST WCHAR* title)
{
    MessageBox(NULL, msg, title, MB_OK);
    return FALSE;
}

HMENU CreateMainMenu()
{
    HMENU hMenu = CreateMenu();
    if (hMenu)
    {
        HMENU hGame = CreatePopupMenu();
        HMENU hAbout = CreatePopupMenu();

        // Game Menu
        AppendMenu(hGame, MF_STRING, 1, L"Deal");
        AppendMenu(hGame, MF_SEPARATOR, 0, NULL);
        AppendMenu(hGame, MF_STRING, 1, L"Undo");
        AppendMenu(hGame, MF_STRING, 1, L"Deck...");
        AppendMenu(hGame, MF_STRING, 1, L"Options...");
        AppendMenu(hGame, MF_SEPARATOR, 0, NULL);
        AppendMenu(hGame, MF_STRING, 1, L"Exit");

        // About Menu
        AppendMenu(hAbout, MF_STRING, 1, L"Contents");
        AppendMenu(hAbout, MF_STRING, 1, L"Search for Help on...");
        AppendMenu(hAbout, MF_STRING, 1, L"How to Use Help");
        AppendMenu(hGame, MF_SEPARATOR, 0, NULL);
        AppendMenu(hAbout, MF_STRING, 1, L"About Solitaire");

        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hGame, L"Game");
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hAbout, L"Help");
    }
    return hMenu;
}

void DrawInitialDeck(void)
{
    // Generate Deck Cover
    // 54 - no cards img
    int id  = 67;
    int x = 10, y = 10;
    customCdtDraw(hdc, x, y, id, 0);
    
}

void customCdtDraw(HDC hdc, int x, int y, int id, DWORD flags)
{
    BOOL result = cdtDraw(hdc, x, y, id, 0);
    if (!result) { 
        wchar_t debugMsg[256];
        swprintf(debugMsg, L"Erro no cdtDraw: width=%d, height=%d", width, height);
        MessageBox(NULL, debugMsg, L"Erro", MB_OK);
    }
}