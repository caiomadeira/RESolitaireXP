#ifndef debug_h
#define debug_h

#include "common.h"
#include "cards.h"

#define DEBUG_ON TRUE

#define DEBUG_PRINT(info) printf("%s: %s\n", __func__, info);

BOOL debugConsole(void);
void printCardsFrom(const char* name, Cards cards[], int size);
void printCardsBIDimentional(const char* name, Cards* cards[], int size);

#endif