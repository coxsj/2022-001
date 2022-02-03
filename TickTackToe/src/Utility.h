#pragma once

#include <windows.h>

class Utility
{
public:
    struct ConsolePos {
    private:
        short H;
        short V;
        short hLength;
        short vLength;
    public:
        short getH() { return H; }
        short getHlen() { return hLength; }
        short getLineAfter() { return V + vLength + 1; }
        short getV() { return V; }
        short getVlen() { return vLength; }
        void setH(short newH) { H = newH; }
        void setHlen(short newHlen) { hLength = newHlen; }
        void setV(short newV) { V = newV; }
        void setVlen(short newVlen) { vLength = newVlen; }
    };


private:
    COORD storedCursorPos;

public:
    //http://www.cplusplus.com/articles/4z18T05o/
    static void cursorToZeroZero() { cursorTo(0, 0); }// printf("\033[0;0H");
    static void cursorTo(const short newV, const short newH);
    static void cursorTo(COORD newCoord);
    static void clear_screen(char fill = ' ');
    static COORD GetConsoleCursorPosition();
    static unsigned int const GetConsoleCursorPositionH() { return GetConsoleCursorPosition().X; }
    static unsigned int const GetConsoleCursorPositionV() { return GetConsoleCursorPosition().Y; }
    void printAt(const short atV, const short atH, std::string s);
    void printNChars(const unsigned int& num, const char& c);
    void restoreStoredCursorPos() { cursorTo(storedCursorPos); }
    void storeCursorPos() { storedCursorPos = GetConsoleCursorPosition(); }
};
