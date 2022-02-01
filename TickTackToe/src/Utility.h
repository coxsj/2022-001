#pragma once

#include <windows.h>

class Utility
{
public:
    static void cursorToZeroZero(){ printf("\033[0;0HHello\n"); }
    static void clear_screen(char fill = ' ') {
        COORD tl = { 0,0 };
        CONSOLE_SCREEN_BUFFER_INFO s;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacter(console, fill, cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
    }
};

