#include <stdio.h>
#include <ctype.h>

#include "Tf2Aimbot.h"

using std::cin;
using std::cout;

void start()
{
	Tf2Aimbot* aimbot;
	aimbot = new Tf2Aimbot();
	aimbot->start();

	delete aimbot;
}

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	//Resizing and formatting the window, it's important.
	SetConsoleTitle(L"An Awesome Cheat Engine by Your mom, bech");
	HWND window = GetConsoleWindow();
	MoveWindow(window, 100, 100, 480, 480, true);


	// POLL GAME WINDOW ////////////////////////////////////////////
	if (!FindWindow(NULL, L"Team Fortress 2"))
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Please Open TF2" << endl;
	}
	
	while (!FindWindow(NULL, L"Team Fortress 2")) { Sleep(500); }
	
	
	cout << endl;
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "TF2 has been found." << endl << endl;
	////////////////////////////////////////////////////////////////

	start();

	cout << "Main says: Quitting, Cya!\n";
	Sleep(300);
	return 0;
}
