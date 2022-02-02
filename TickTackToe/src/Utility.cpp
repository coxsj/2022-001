#include "Utility.h"

void Utility::cursorTo(const int newV, const int newH) {
    //initialize objects for cursor manipulation
    HANDLE hStdout;
    COORD destCoord;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    //position cursor at new position
    destCoord.X = newH;
    destCoord.Y = newV;
    SetConsoleCursorPosition(hStdout, destCoord);
}
void Utility::clear_screen(char fill) {
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}