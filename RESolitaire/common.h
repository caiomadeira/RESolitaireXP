#pragma once

#ifndef common_h
#define common_h

#define UNICODE
#define _UNICODE

// Arquivos de Cabeçalho do Windows
#include <windows.h>
#include <windowsx.h> // For GET_X_LPARAM(lParam)
// Arquivos de Cabeçalho C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wchar.h>
#include <stdio.h>
#include <time.h>
#include <commctrl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <wchar.h>
#include <stdlib.h>
#include "globals.h"


#pragma comment(lib, "comctl32.lib")

#endif