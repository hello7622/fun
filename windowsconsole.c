#include "windowsconsole.h"
#include <windows.h>

void SetConsoleColor(Color foreGround, Color backGround) {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOutput == INVALID_HANDLE_VALUE) {
		return ;
	}
	SetConsoleTextAttribute(hOutput, foreGround | (backGround << 4));
}

void Gotoxy(int x, int y) {
	COORD pos;
	pos.X = y;
	pos.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(void) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = 0;
	SetConsoleCursorInfo(handle, &CursorInfo);
}