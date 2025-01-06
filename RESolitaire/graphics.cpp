#include "graphics.h"

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
