#include "debug.h"

BOOL debugConsole(void)
{
    if (DEBUG_ON)
    {
        AllocConsole();

        freopen("CONOUT$", "w", stdout);
        printf("Debug - RE Solitaire XP");
        return TRUE;
    }
    return FALSE;
}

void printCardsFrom(const char* name, Cards cards[], int size)
{
    printf("\n:::::: %s: %s ::::::\n", __func__, name);
    for(int i = 0; i < size; i++)
    {
        printf("[%d]: Card ID: %d, Suit: %d\n", i, cards[i].value, cards[i].suit);
    }
    printf("::::::::::::::::::::::::::::::::\n");
}

void printCardsBIDimentional(const char* name, Cards* cards[], int size)
{
    printf("\n:::::: %s: %s ::::::\n", __func__, name);
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < i; j++)
            printf("[%d][%d]: Card ID: %d, Suit: %d\n", i, j, cards[i][j].value, cards[i][j].suit);
    }
    printf("::::::::::::::::::::::::::::::::\n");
}