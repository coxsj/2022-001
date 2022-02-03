
#include <iostream>
#include <ctime>

#include "Utility.h"


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
void Utility::cursorTo(const short newV, const short newH) {
    COORD destCoord;
    destCoord.X = newH;
    destCoord.Y = newV;
    cursorTo(destCoord);
}
void Utility::cursorTo(COORD newCoord) {
    //initialize objects for cursor manipulation
    HANDLE hStdout;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    //position cursor at new position
    SetConsoleCursorPosition(hStdout, newCoord);
}
COORD Utility::GetConsoleCursorPosition(){
    //initialize objects for cursor manipulation
    HANDLE hStdout;
    COORD destCoord;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hStdout, &cbsi))
        return cbsi.dwCursorPosition;
    else{
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}
void Utility::printAt(const short atV, const short atH, std::string s) {
    cursorTo(atV, atH);
    std::cout << s;
}
void Utility::printNChars(const unsigned int& num, const char& c) {
    for (auto i = 0; i < num; i++) {
        std::cout << c;
    }
}
void Utility::randomSeed() {
    //Randomize computer random cell picker
    srand(static_cast<unsigned int>(time(0)));
}
