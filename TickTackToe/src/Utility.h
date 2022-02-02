#pragma once

#include <windows.h>

class Utility
{

public:
    //http://www.cplusplus.com/articles/4z18T05o/
    static void cursorToZeroZero() { cursorTo(0, 0); }// printf("\033[0;0H");
    static void cursorTo(const int newV, const int newH);
    static void clear_screen(char fill = ' ');
};
