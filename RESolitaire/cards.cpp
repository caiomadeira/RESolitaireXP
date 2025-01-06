#include "cards.h"

cdtInitPtr cdtInit = NULL;
cdtDrawPtr cdtDraw = NULL;
cdtAnimatePtr cdtAnimate = NULL;
cdtTermPtr cdtTerm = NULL;

HINSTANCE hCardsDll = NULL;

int cardWidth, cardHeight = 0;

void RECdtDraw(HDC hdc, int x, int y, int id, DWORD flags)
{
    BOOL result = cdtDraw(hdc, x, y, id, 0);
    if (!result) { 
        wchar_t debugMsg[256];
        swprintf(debugMsg, L"Erro no cdtDraw: width=%d, height=%d", cardWidth, cardHeight);
        MessageBox(NULL, debugMsg, L"Erro", MB_OK);
    }
}

BOOL LoadCardsDll(void)
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

    cdtInit(&cardWidth, &cardHeight);
    return TRUE;
}