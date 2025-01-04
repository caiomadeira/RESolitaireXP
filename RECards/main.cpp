#ifdef _WIN64
    #define STDCALL_OR_CDECL
#else
    #define STDCALL_OR_CDECL __cdecl
#endif

#include<windows.h>
#include<iostream>
#include<stdio.h>

// This data struct is given by windows.h (beyond wingdi.h)
// typedef struct tagBITMAP {
//     LONG bmType;
//     LONG bmWidth;
//     LONG bmHeight;
//     LONG bmWidthBytes;
//     WORD bmPlanes;
//     WORD bmBitsPixel;
//     LPVOID bmBits;
// } BITMAP;

// Global; DWORD = unsigned int
DWORD DWORD_6fc120e8; // Removed pointer sintax
DWORD DWORD_6fc120ec; // Removed pointer sintax
DWORD DWORD_6fc12004;
DWORD DWORD_6fc12000;
DWORD DAT_6fc120f4;

HINSTANCE hInstance;
HANDLE hHandle;

// FUNCTIONS SIGNATURES
int cdtInit(int *width, int *height);
void DeleteObj(HGDIOBJ param_1);


// Ordinal 5 - 0x13e6 - cdInit
int cdtInit(int *width, int *height)
{
    int result;
    BITMAP lpvoidPV[4];
    // undefined bmpWidth;
    // undefined bmpHeight;

    result = DAT_6fc120f4;
    DAT_6fc120f4 = DAT_6fc120f4 + 1;
    if (result == 0)
    {
        hHandle = LoadBitmapA(hInstance, (LPCSTR)0x35);
        DWORD_6fc120e8 = (DWORD)LoadBitmapA(hInstance, (LPCSTR)0x43);
        DWORD_6fc120ec = (DWORD)LoadBitmapA(hInstance, (LPCSTR)0x44);
        if (((hHandle == (HBITMAP)nullptr) || (DWORD_6fc120e8 == 0)) || ((HBITMAP)DWORD_6fc120ec == (HBITMAP)nullptr))
        {
            DeleteObj(hHandle); // FUN_6fc1137b
            DeleteObj((HGDIOBJ)DWORD_6fc120e8);
            DeleteObj((HGDIOBJ)DWORD_6fc120ec);
            return 0;
        }

        GetObjectA(hHandle, 0x18, lpvoidPV);
        *width = lpvoidPV->bmWidth; // *width = local_18;
        DWORD_6fc12004 = *width; // DWORD_6fc12004 = local_18;
        *height = lpvoidPV->bmHeight; //  *height = local_14;
        DWORD_6fc12000 = *height; // DWORD_6fc12000 = local_14;
    } 
    else
    {
        *width = DWORD_6fc12004;
        *height = DWORD_6fc12000;
    }
    return 1;
}

// FUN_6fc1137b
void DeleteObj(HGDIOBJ param_1)
{
    if (param_1 != (HGDIOBJ)nullptr)
    {
        DeleteObject(param_1);
        return;
    }
    return;
}


// Testing
int main(void)
{
    int width = 0, height = 0;

    int result = cdtInit(&width, &height);
    if (result == 1)
        printf("Sucess in cdtInit\n");
    else
        printf("Fail in cdtInit\n");
    return 0;
}