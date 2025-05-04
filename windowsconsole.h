#ifndef __C_WINDOWS_CONSOLE_H__
#define __C_WINDOWS_CONSOLE_H__

enum Color 
{
	Black = 0,
	DarkBlue,
	DarkGreen,
	GrayBlue,
	Red,
	DarkRed,
	Yellow,
	GrayWhite,
	BrownGray,
	Blue,
	Green,
	Cyan,
	OrangeRed,
	BrightRed,
	BrightYellow,
	White
};

void SetConsoleColor(Color foreGround, Color backGround = Black);
void Gotoxy(int x, int y);
void HideCursor();

#endif  // __C_WINDOWS_CONSOLE_H__
