#include "error.h"

BOOL customErrorMsgBox(CONST WCHAR* msg, CONST WCHAR* title)
{
    MessageBox(NULL, msg, title, MB_OK);
    return FALSE;
}