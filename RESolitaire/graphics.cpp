#include "graphics.h"
#include "globals.h"

HBRUSH backgroundBrush = NULL;

HBRUSH defaultBackGround(void)
{
    if (!backgroundBrush)
        backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));
    return backgroundBrush;
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

void DrawBitmap(HDC hdc, LPCWSTR bitmapPath, int x, int y)
{
    // Carregar o bitmap (Unicode)
    HBITMAP hBitmap = (HBITMAP)LoadImageW(
        NULL,              // Sem instância, carregando do sistema de arquivos
        bitmapPath,        // Caminho para o arquivo bitmap (Unicode)
        IMAGE_BITMAP,      // Tipo de imagem
        0, 0,              // Dimensões originais
        LR_LOADFROMFILE    // Carregar de um arquivo
    );

    if (!hBitmap)
    {
        MessageBoxW(NULL, L"Erro ao carregar o bitmap.", L"Erro", MB_OK);
        return;
    }

    // Criar um contexto de dispositivo compatível
    HDC hMemDC = CreateCompatibleDC(hdc);
    if (!hMemDC)
    {
        MessageBoxW(NULL, L"Erro ao criar DC compatível.", L"Erro", MB_OK);
        DeleteObject(hBitmap);
        return;
    }

    // Selecionar o bitmap no DC de memória
    HGDIOBJ hOldBitmap = SelectObject(hMemDC, hBitmap);

    // Obter as dimensões do bitmap
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // Copiar o bitmap para o contexto da janela
    BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);

    // Restaurar o bitmap original e liberar recursos
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
}

/****** STATUS BAR FUNCTIONS *******/
LRESULT CALLBACK WindowProcStatusBar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result;
    RECT rect;

    PAINTSTRUCT ps;
    if (uMsg == WM_PAINT && hwnd != NULL) // uMsg == 0xf
    {
        HDC hdc = BeginPaint(hwnd, &ps);

        // Size of bar
        GetClientRect(statusHwnd, &rect);
        //rect.top = rect.bottom - 30;
        rect.right = rect.right + -4;

        // Draw white background of bar
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        // Draw Score text
        // Bg Transparent for text
        SetBkMode(hdc, TRANSPARENT);
        // Text color (black)
        SetTextColor(hdc, RGB(0, 0, 0));

        LPCWSTR pszFaceName = L"MS Shell Dlg"; // Font name

        int cWeight = 700;
        int cOrientation = 0;
        int cEscapement = 0;
        int cWidth = 0;
        int nDenominator = 0x48;

        int cHeight = GetDeviceCaps(hdc, 0x5a);
        cHeight = MulDiv(9, cHeight, nDenominator);

        // Definir a fonte (opcional, caso queira alterar o estilo do texto).
        HFONT font = CreateFontW(-cHeight, cWidth, cEscapement, cOrientation, 
            cWeight, 
            0, 0, 0, // bItalic, bUnderline, bStrikeOut
            1, // iCharSet
            0, // iOutPrecision 
            0, // iClipPrecision
            0, // iQuality
            0, // iPitchAndFamily
            pszFaceName
        );
         
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
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);

    return result;
}

WNDCLASS ConfigWndClassStatusBar(void)
{
    WNDCLASS wc = { 0 };

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
    return wc;
}

int CreateStatusBarWindow(HWND parentHwnd, WNDCLASS wc)
{
    RECT rect;
    int statusBarHeight = fontHeight + 2; // Fixed height
    GetClientRect(parentHwnd, &rect);
    statusHwnd = CreateWindowEx(
        0, 
        wc.lpszClassName, // Classe padrão do Windows para barras de status
        L"", 
        WS_CHILD | WS_VISIBLE, 
        rect.left + -1, 
        (rect.bottom - statusBarHeight) + 1, 
        (rect.right - rect.left) + 2, 
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