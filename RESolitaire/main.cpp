// RESolitaireXP.cpp : Define o ponto de entrada para o aplicativo.
// g++ main.cpp -o RESolitaireXP.exe -mwindows -luser32 -lgdi32 -lcomctl32

// This defines allows to use commctrl
// For use commctrl header we need to use almost minumium XP version

#define _WIN32_IE 0x0600
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include "resource.h"
#include "error.h"
#include "graphics.h"
#include "cards.h"

/****** GLOBALS *******/
HINSTANCE hInstance;
HWND hwnd;
HWND statusHwnd;
HDC hdc;
HMENU hMenu;
HICON hIcon;
HANDLE _iconHandle;
HACCEL hAccTable;

// Main Window Context
// Font vars
int fontLargestChar = 0;
int fontHeight = 0;

// Device vars
int numColors = 0;
int visibileWidth = 0;
int visibileHeight = 0;

// RegisterWindow
UINT registerWindowMsg;
/************************/

// timer
int timerCount = 0;

// Cards
Cards deck[DECK_MAX];
Cards tableau[TABLEAU_MAX][TABLEAU_CARDS_MAX];
Cards foundation[SUITS_SIZE][FOUNDATION_CARDS_PER_PILE];
Cards pile[PILE_MAX];
int pileCount = 0;

/****** WINDOW FUNCTIONS SIGNATURES *******/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void NewGame(void);


/*****************************************/

// TODO: Update G++ to >= 7.0, because my low version prevents use of some sleep functions
int TimerCallback(void)
{
    //MessageBox(hwnd, L"Timer acionado com callback!", L"Timer Callback", MB_OK);
    //Sleep(1000);
    timerCount++;
    //Redraw status bar
    InvalidateRect(statusHwnd, NULL, TRUE); 
    return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ATOM regClass;
    WNDCLASS wc = { 0 };

    BOOL result;
    TEXTMETRIC textMetrics;
    HCURSOR cursor;
    time_t currentTime;
    WCHAR regWinBuffer[20];
    INITCOMMONCONTROLSEX commonControls;
    RECT windowRect;
    UINT_PTR timerId;
    MSG msg = { 0 };

    // Global hInstance
    hInstance = hInstance;

    result = LoadCardsDll();
    if (result)
    {    
        cursor = LoadCursor(hInstance, (LPCWSTR)0x7f00);
        hdc = GetDC(hwnd); // The hdc is global. Maybe this not make sense.
        if (hdc != NULL)
        {
            //Font configuration
            GetTextMetrics(hdc, &textMetrics);  
            fontHeight = textMetrics.tmHeight;
            fontLargestChar = textMetrics.tmMaxCharWidth;
            // Get Device (Monitor, Graphic Devices) Properties
            // NUMCOLORS = Number of available colors
            numColors = GetDeviceCaps(hdc, NUMCOLORS);
            visibileWidth = GetDeviceCaps(hdc, HORZRES); // Visible width area (pixels)(index = 8)
            visibileHeight = GetDeviceCaps(hdc, VERTRES); // Visibile height area (pixels)(index = 10)
            // Fill background with color GREEN
            backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));
            // Initialize time var
            currentTime = time(NULL);
            // Loads the String buffer to pass into RegisterWindowMessage to communication
            LoadStringW(hInstance, 0x67, regWinBuffer, 0x14);
            registerWindowMsg = RegisterWindowMessage(regWinBuffer);

            // Init common controls as progress bars, tool bars, list control, etc
            commonControls.dwSize = 8;
            commonControls.dwICC = 0x16fd;
            InitCommonControlsEx(&commonControls);

            // Load Icon and Image for
            hIcon = LoadIcon(hInstance, (LPCWSTR)0x1f4);
            HICON _iconHandle = (HICON)LoadImageW(hInstance, (LPCWSTR)0x1f4, 1, 0x10, 0x10, 0);

            wc.lpfnWndProc = WindowProc;
            wc.hInstance = hInstance;
            wc.lpszClassName = L"Solitaire Window XP";
            wc.style = 0x2008;
            wc.hbrBackground = backgroundBrush;
            wc.lpszMenuName = (LPCWSTR)0X1;
            wc.hIcon = hIcon;
            wc.hCursor = cursor;

            regClass = RegisterClass(&wc);
            if (!regClass) goto error_handler;

            // Window Rect Calculation
            windowRect.top = 0;
            windowRect.left = 0;
            int adjustWidth = cardWidth / 8 + 3;
            windowRect.right = cardWidth * 7 + adjustWidth * 8;
            windowRect.bottom = cardHeight << 2;
            AdjustWindowRect(&windowRect, 0xcf0000, 1);
            windowRect.bottom = windowRect.bottom - windowRect.top;
            windowRect.right = windowRect.right - windowRect.left;
            if (visibileHeight < windowRect.bottom) 
                windowRect.bottom = visibileHeight;

            // Init Window
            hwnd = CreateWindowEx(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 
                CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right, windowRect.bottom, 
                NULL, NULL, hInstance, NULL);

            if (hwnd != NULL)
            {
                hMenu = CreateMainMenu();
                SetMenu(hwnd, hMenu);
                // Init temporizator
                /*
                Setamos uma callback que lida com eventos do temporizador.
                */
                timerId = SetTimer(hwnd, 0x29a, 0xfa, (TIMERPROC)&TimerCallback);
                if (timerId != 0)
                {
                    ShowWindow(hwnd, nCmdShow);
                    UpdateWindow(hwnd);
                    hAccTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL));
                    if (!hAccTable) goto error_handler;

                    /********* This is like original code was, but it's too confusing ********/
                    WNDCLASS wc2 = ConfigWndClassStatusBar();
                    CreateStatusBarWindow(hwnd, wc2);
                    //PostMessage(hwnd, 0x111, 1000, 0);
                    /***********************************************************************/
                }
            }
        } else { 
            error_handler:
            customErrorMsgBox(L"Error in initialization of window.", wc.lpszClassName); 
        }
    }
    
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hwnd, hAccTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    FreeLibrary(hCardsDll);
    DeleteObject(backgroundBrush);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //HDC hdc;
    PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_PAINT:

        hdc = BeginPaint(hwnd, &ps);  // Obter HDC válido
        if (hdc) 
        {
            // Tentativa de desenhar uma carta
            DrawInitialDeck();
            //InvalidateRect(hwnd, NULL, TRUE);
            EndPaint(hwnd, &ps);  // Finalizar o HDC
        } 
        else 
        {
            MessageBox(NULL, L"Erro ao obter HDC!", L"Erro", MB_OK);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case ID_F1:
                MessageBox(hwnd, L"F1 pressed!", L"Accelerator", MB_OK);
                break;
            case ID_F2:
                MessageBox(hwnd, L"F2 pressed!", L"Accelerator", MB_OK);
                break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DealCards(void)
{
    int deckIndex = 0;

    // Deal to tableau
    for (int i = 0; i < TABLEAU_MAX; ++i) {
        for (int j = 0; j <= i; ++j) {
            tableau[i][j] = deck[deckIndex++];
        }
    }

    // Remaining cards go to the pile
    pileCount = DECK_MAX - deckIndex;
    for (int i = 0; i < pileCount; ++i) {
        pile[i] = deck[deckIndex++];
    }
}

void InitDeck(void)
{
    int cardIndex = 0;
    for (int suit = 0; suit < SUITS_SIZE; ++suit) {
        for (int value = 1; value <= FOUNDATION_CARDS_PER_PILE; ++value) {
            deck[cardIndex].value = value;
            deck[cardIndex].suit = (Suits)suit;
            cardIndex++;
        }
    }

    // Shuffle the deck
    srand((unsigned int)time(NULL));
    for (int i = 0; i < DECK_MAX; ++i) {
        int j = rand() % DECK_MAX;
        Cards temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void DrawPile(int x, int y)
{
    for (int i = 0; i < 3; ++i) {
        DrawBitmap(hdc, L"assets\\bitmap\\54.bmp", x + i * 2, y + i * 2);
    }
}

void DrawTableau(int x, int y)
{
    y = cardHeight + 20;
    for (int i = 0; i < TABLEAU_MAX; i++) {
        int xOffset = x + i * (cardWidth + 10);
        for (int j = 0; j <= i; ++j) {
            if (tableau[i][j].value != 0) {
                int yOffset = y + j * 3;

                if (j == i && tableau[i][j].value != 0)
                    RECdtDraw(hdc, xOffset, yOffset, tableau[i][j].value + (tableau[i][j].suit * FOUNDATION_CARDS_PER_PILE), 0);
                else
                    DrawBitmap(hdc, L"assets\\bitmap\\54.bmp", xOffset, yOffset);
            }
        }
    }
}

void DrawFoundationSlots(int x, int y)
{
    int tableauCardsQtdBeforeSlot = 3;
    int tableauCardsPadding = 10;
    x = cardWidth * (tableauCardsQtdBeforeSlot + 1) - (tableauCardsPadding * tableauCardsQtdBeforeSlot);
    for (int i = 0; i < SUITS_SIZE; ++i) {
        RECdtDraw(hdc, x + i * (cardWidth + 10), y, 52, 0);
    }
}

void NewGame(void)
{
    InitDeck();
    DealCards();

    int x = 10, y = 10;
    DrawPile(x, y);
    DrawTableau(x, y);
    DrawFoundationSlots(x + TABLEAU_MAX * (cardWidth + 20), y);
}
