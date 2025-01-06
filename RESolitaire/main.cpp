// RESolitaireXP.cpp : Define o ponto de entrada para o aplicativo.
// g++ main.cpp -o RESolitaireXP.exe -mwindows -luser32 -lgdi32 -lcomctl32

// This defines allows to use commctrl
// For use commctrl header we need to use almost minumium XP version

#define _WIN32_IE 0x0600
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include "resource.h"
#include "graphics.h"
#include "error.h"
#include "cards.h"
#include <commctrl.h>
#include <iostream>

/****** FUNCTIONS SIGNATURES *******/
BOOL LoadCardsDll(void);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawInitialDeck(void);
/************************/

/****** GLOBALS *******/
HINSTANCE hInstance;
HWND hwnd;
HWND statusHwnd;
HDC hdc;
HMENU hMenu;
HICON hIcon;
HANDLE _iconHandle;
HACCEL hAccTable;

// Main Window Context
// Font vars
int fontLargestChar = 0;
int fontHeight = 0;

// Device vars
int numColors = 0;
int visibileWidth = 0;
int visibileHeight = 0;

// RegisterWindow
UINT registerWindowMsg;
/************************/


int TimerCallback(void)
{
    //MessageBox(hwnd, L"Timer acionado com callback!", L"Timer Callback", MB_OK);
  return 1;
}

LRESULT CALLBACK WindowProcStatusBar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result;
    RECT rect;

    PAINTSTRUCT ps;
    if (uMsg == WM_PAINT) // uMsg == 0xf
    {
        HDC hdc = BeginPaint(hwnd, &ps);

        // Size of bar
        GetClientRect(statusHwnd, &rect);
        rect.top = rect.bottom - 30;

        // Draw white background of bar
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        // Draw Score text
        // Bg Transparent for text
        SetBkMode(hdc, TRANSPARENT);
        // Text color (black)
        SetTextColor(hdc, RGB(0, 0, 0));

        // Definir a fonte (opcional, caso queira alterar o estilo do texto).
        HFONT font = CreateFont(
            -16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
        SelectObject(hdc, font);

        // Exibir o texto à direita da barra.
        std::wstring text = L"Score: 0 Time: 276";
        DrawText(hdc, text.c_str(), -1, &rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

        DeleteObject(font); // Libera a fonte.

        // 4. (Opcional) Adicionar uma borda separadora acima da barra.
        HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192)); // Cinza claro.
        SelectObject(hdc, borderPen);
        MoveToEx(hdc, rect.left, rect.top, NULL); // Linha no topo da barra.
        LineTo(hdc, rect.right, rect.top);
        DeleteObject(borderPen); // Libera a caneta.

        EndPaint(hwnd, &ps);
        result = 0;
    } 
    else 
        result = DefWindowProc(statusHwnd, uMsg, wParam, lParam);

    return result;
}

int ConfigWndClassStatusBar(int nCmdShow)
{
    WNDCLASS wc = {0};

    wc.style = 0;
    wc.lpfnWndProc = WindowProcStatusBar;
    wc.cbWndExtra = 0;
    wc.cbClsExtra = 0;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); // Fundo padrão
    wc.lpszClassName = L"MyCustomStatusBar";

    if (!RegisterClass(&wc)) 
    {
        DWORD errorCode = GetLastError();
        wchar_t errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errorMsg, 256, NULL);
        MessageBox(NULL, errorMsg, L"ConfigWndClassStatusBar", MB_OK);
    }
    return 1;
}

int CreateStatusBarWindow(HWND parentHwnd)
{
    RECT rect;

    int statusBarHeight = 30; // Fixed height
    GetClientRect(parentHwnd, &rect);
    statusHwnd = CreateWindowEx(
        0, 
        STATUSCLASSNAME, // Classe padrão do Windows para barras de status
        L"", 
        WS_CHILD | WS_VISIBLE, 
        rect.left, 
        rect.bottom - statusBarHeight, 
        rect.right - rect.left, 
        statusBarHeight, 
        parentHwnd, 
        NULL, 
        hInstance, 
        NULL);
    
    if (!statusHwnd)
    {
        DWORD errorCode = GetLastError();
        wchar_t errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errorMsg, 256, NULL);
        MessageBox(NULL, errorMsg, L"CreateStatusBarWindow", MB_OK);
        return 0;
    }

    ShowWindow(statusHwnd, 4);
    UpdateWindow(statusHwnd);
    return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ATOM regClass;
    WNDCLASS wc = { 0 };

    BOOL result;
    TEXTMETRIC textMetrics;
    HCURSOR cursor;
    time_t currentTime;
    WCHAR regWinBuffer[20];
    INITCOMMONCONTROLSEX commonControls;
    RECT windowRect;
    UINT_PTR timerId;
    MSG msg = { 0 };

    // Global hInstance
    hInstance = hInstance;

    result = LoadCardsDll();
    if (result)
    {    
        cursor = LoadCursor(hInstance, (LPCWSTR)0x7f00);
        hdc = GetDC(hwnd); // The hdc is global. Maybe this not make sense.
        if (hdc != NULL)
        {
            //Font configuration
            GetTextMetrics(hdc, &textMetrics);  
            fontHeight = textMetrics.tmHeight;
            fontLargestChar = textMetrics.tmMaxCharWidth;
            // Get Device (Monitor, Graphic Devices) Properties
            // NUMCOLORS = Number of available colors
            numColors = GetDeviceCaps(hdc, NUMCOLORS);
            visibileWidth = GetDeviceCaps(hdc, HORZRES); // Visible width area (pixels)(index = 8)
            visibileHeight = GetDeviceCaps(hdc, VERTRES); // Visibile height area (pixels)(index = 10)
            // Fill background with color GREEN
            backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));
            // Initialize time var
            currentTime = time(NULL);
            // Loads the String buffer to pass into RegisterWindowMessage to communication
            LoadStringW(hInstance, 0x67, regWinBuffer, 0x14);
            registerWindowMsg = RegisterWindowMessage(regWinBuffer);

            // Init common controls as progress bars, tool bars, list control, etc
            commonControls.dwSize = 8;
            commonControls.dwICC = 0x16fd;
            InitCommonControlsEx(&commonControls);

            // Load Icon and Image for
            hIcon = LoadIcon(hInstance, (LPCWSTR)0x1f4);
            HICON _iconHandle = (HICON)LoadImageW(hInstance, (LPCWSTR)0x1f4, 1, 0x10, 0x10, 0);

            wc.lpfnWndProc = WindowProc;
            wc.hInstance = hInstance;
            wc.lpszClassName = L"Solitaire Window XP";
            wc.style = 0x2008;
            wc.hbrBackground = backgroundBrush;
            wc.lpszMenuName = (LPCWSTR)0X1;
            wc.hIcon = hIcon;
            wc.hCursor = cursor;

            regClass = RegisterClass(&wc);
            if (!regClass) goto error_handler;

            // Window Rect Calculation
            windowRect.top = 0;
            windowRect.left = 0;
            int adjustWidth = cardWidth / 8 + 3;
            windowRect.right = cardWidth * 7 + adjustWidth * 8;
            windowRect.bottom = cardHeight << 2;
            AdjustWindowRect(&windowRect, 0xcf0000, 1);
            windowRect.bottom = windowRect.bottom - windowRect.top;
            windowRect.right = windowRect.right - windowRect.left;
            if (visibileHeight < windowRect.bottom) 
                windowRect.bottom = visibileHeight;

            // Init Window
            hwnd = CreateWindowEx(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 
                CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right, windowRect.bottom, 
                NULL, NULL, hInstance, NULL);

            if (hwnd != NULL)
            {
                hMenu = CreateMainMenu();
                SetMenu(hwnd, hMenu);
                // Init temporizator
                /*
                Setamos uma callback que lida com eventos do temporizador.
                */
                timerId = SetTimer(hwnd, 0x29a, 0xfa, (TIMERPROC)&TimerCallback);
                if (timerId != 0)
                {
                    ShowWindow(hwnd, nCmdShow);
                    UpdateWindow(hwnd);
                    hAccTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL));
                    if (!hAccTable) goto error_handler;

                    /********* This is like original code was, but it's too confusing ********/
                    if (!ConfigWndClassStatusBar(SW_SHOW))
                    {
                        MessageBox(NULL, L"Falha ao registrar a classe StatusBar!", L"Erro", MB_OK);
                        return 0;
                    }
                        CreateStatusBarWindow(hwnd);
                    //PostMessage(hwnd, 0x111, 1000, 0);
                    /***********************************************************************/

                }
            }
        } else { 
            error_handler:
            customErrorMsgBox(L"Error in initialization of window.", wc.lpszClassName); 
        }
    }
    
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hwnd, hAccTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    FreeLibrary(hCardsDll);
    DeleteObject(backgroundBrush);
    return 0;
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
            case ID_F1:
                MessageBox(hwnd, L"F1 pressed!", L"Accelerator", MB_OK);
                break;
            case ID_F2:
                MessageBox(hwnd, L"F2 pressed!", L"Accelerator", MB_OK);
                break;
        }
    case WM_SIZE:
    {
        if (statusHwnd)
        {
            RECT rect;
            GetClientRect(hwnd, &rect);

            int statusBarHeight = 30;
            MoveWindow(
                statusHwnd,
                rect.left,
                rect.bottom - statusBarHeight,
                rect.right - rect.left,
                statusBarHeight,
                TRUE
            );
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DrawInitialDeck(void)
{ 

    // Draw cover
    int xc = 10; 
    int yc = 10;
    for(int i = 0; i < 3; i++)
    {
        DrawBitmap(hdc, L"assets\\bitmap\\54.bmp", xc, yc);
        xc = xc + 2; 
        yc = xc + 0.5;
    }

    int x2;
    int x1 = 10, y1 = cardHeight + 20;
    int slotCount = 7;
    // Draw cards
    for(int i = 0; i < slotCount; i++)
    {
        RECdtDraw(hdc, x1, y1, i, 0);
        x1 = x1 + (cardWidth + 10);
        if (i == 1)
        {
            x2 = x1;
            // Draw empty slots
            for(int i = 0; i < 4; i++)
            {
                x2 = x2 + (cardWidth + 10);
                RECdtDraw(hdc, x2, 10, 52, 0);
            }
        }
    }
}