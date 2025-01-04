#include <windows.h>
#include <iostream>

#pragma comment(lib, "cards.dll")

typedef BOOL(WINAPI* cdtInitPtr)(int* width, int* height);

int main(void)
{
    HINSTANCE hCardsDLL = LoadLibrary(TEXT("RESolitaireXP\\scripts\\cards\\Cards\\cards.dll"));
    if (!hCardsDLL)
    {
        printf("Falha ao carregar cards.dll. Erro: 0x%x\n", GetLastError());
        return 1;
    }
    else
    {
        printf("Success loaded.");
        // Get functions pointers
        cdtInitPtr cdtInit = (cdtInitPtr)GetProcAddress(hCardsDLL, "cdtInit");

        if (!cdtInit)
        {
            printf("Falha ao obter nomes de  cards.dll. Erro: 0x%x\n", GetLastError());
            FreeLibrary(hCardsDLL);
            return 1;
        }

        // Init cdtInit
        int width = 0, height = 0;
        if (!cdtInit(&width, &height))
        {
            MessageBox(NULL, L"Erro inicializing cdtInit", nullptr, MB_OK);
            FreeLibrary(hCardsDLL);
            return 1;
        }

        printf("Cards dimensions: %d, %d\n", width, height);
    }
}