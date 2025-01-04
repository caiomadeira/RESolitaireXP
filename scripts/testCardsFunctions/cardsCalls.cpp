#include <windows.h>
#include <iostream>

#pragma comment(lib, "cards.dll")

typedef BOOL(WINAPI *cdtInitPtr)(int* width, int* height);
typedef BOOL(WINAPI *cdtDrawPtr)(HDC hdc, int x, int y, int cardID, DWORD flags);
typedef BOOL(WINAPI *cdtAnimatePtr)(int animationType, int param1, int param2, int speed);
typedef BOOL(WINAPI *cdtTermPtr)();
void drawAllCards(HWND hwnd, HDC hdc, cdtDrawPtr cdtDraw);

int main(void)
{
    HINSTANCE hCardsDLL = LoadLibrary(TEXT("cards.dll"));
    if (!hCardsDLL)
    {
        printf("Falha ao carregar cards.dll. Erro: 0x%x\n", GetLastError());
        return 1;
    }
    else
    {
        printf("Success loaded.\n");
        // Get functions pointers
        cdtInitPtr cdtInit = (cdtInitPtr)GetProcAddress(hCardsDLL, "cdtInit");
        cdtDrawPtr cdtDraw = (cdtDrawPtr)GetProcAddress(hCardsDLL, "cdtDraw");
        cdtAnimatePtr cdtAnimate = (cdtAnimatePtr)GetProcAddress(hCardsDLL, "cdtAnimate");
        cdtTermPtr cdtTerm = (cdtTermPtr)GetProcAddress(hCardsDLL, "cdtTerm");

        if (!cdtInit)
        {
            printf("Falha ao obter nomes de cards.dll. Erro: 0x%x\n", GetLastError());
            FreeLibrary(hCardsDLL);
            return 1;
        }

        // Init cdtInit
        int width = 0, height = 0;
        if (!cdtInit(&width, &height))
        {
            MessageBoxW(NULL, L"Erro inicializando cdtInit", L"Erro", MB_OK);
            printf("falha ao inciiar cdInit\n");
            FreeLibrary(hCardsDLL);
            return 1;
        }
        printf("Cards dimensions: %d, %d.\n", width, height);

        // Draw cards
        HWND hwnd = GetConsoleWindow();
        HDC hdc = GetDC(hwnd);
        if (!hdc) 
        {
            printf("Falha ao obter o context do dispositivo (HDC).");
            cdtTerm();
            FreeLibrary(hCardsDLL);
            return 1;
        }

        // Draw a card (ID = 0: Ás de espacadas)
        int cardID = 0;
        int x = 50, y = 50;
        if (!cdtDraw(hdc, x, y, cardID, 0)) 
            printf("Failed to draw cards.\n");
        else
            printf("Success to draw cards!\n");

        // animate
        int animationType = 1;
        int startPosition = 0;
        int endPosition = 10;
        int speed = 5;

        if (!cdtAnimate(animationType, startPosition, endPosition, speed))
            printf("Success animation.\n");
        else
            printf("Failed animation");

        drawAllCards(hwnd, hdc, cdtDraw);

        // Liberar o contexto gráfico
        ReleaseDC(hwnd, hdc);

        // Finalizar o sistema de cartas
        cdtTerm();

        // Liberar a DLL
        FreeLibrary(hCardsDLL);    
    }
    return 0;
}

void drawAllCards(HWND hwnd, HDC hdc, cdtDrawPtr cdtDraw)
{
    RECT rect;
    int x = 0;
    int y = 0;
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    GetWindowRect(hwnd, &rect);
    for(int i = 0; i < 50; i++)
    {
        cdtDraw(hdc, x, y, i, 0);
        x = x + 70;
        if (x > w)
            y = y + 100;
    }
}