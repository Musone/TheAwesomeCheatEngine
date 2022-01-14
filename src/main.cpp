#include <stdio.h>
#include <ctype.h>
#include <windows.h>

// #define _UNICODE 
// #define UNICODE

#include "Modules/Tf2Hacks.h"

using std::cin;
using std::cout;

void start(HINSTANCE hInstance, int nCmdShow)
{
	Tf2Hacks* aimbot;
	aimbot = new Tf2Hacks(hInstance, nCmdShow);
	aimbot->start();

	delete aimbot;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	//Resizing and formatting the window, it's important.
	SetConsoleTitle(L"An Awesome Cheat Engine by Your mom, bech");
	HWND window = GetConsoleWindow();
	MoveWindow(window, 100, 100, 480, 480, true);


	// POLL GAME WINDOW ////////////////////////////////////////////
	if (!FindWindow(NULL, L"Team Fortress 2"))
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		// cout << "Please Open TF2" << endl;
	}

	while (!FindWindow(NULL, L"Team Fortress 2")) { Sleep(500); }


	// cout << endl;

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	// cout << "TF2 has been found." << endl << endl;
	////////////////////////////////////////////////////////////////

	start(hInstance, nCmdShow);

	// cout << "Main says: Quitting, Cya!\n";
	Sleep(300);
	return 0;
}
