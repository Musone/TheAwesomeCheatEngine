#include "GameOffsetsTf2.h"
#include "ProcessManager.h"
// #include "CNetVar.h"
// #include <fstream>
#include <stdio.h>
#include <ctype.h>
// #include <ctime>

// #pragma warning(disable : 4996)

using std::cin;
using std::cout;

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GameOffsetsTf2* offsetsTf2;

	//Resizing and formatting the window, it's important.
	SetConsoleTitle(L"TF2 Offset Dumper by Your mom, bech");
	HWND window = GetConsoleWindow();
	MoveWindow(window, 100, 100, 480, 480, true);


	// POLL GAME WINDOW ////////////////////////////////////////////
	if (!FindWindow(NULL, L"Team Fortress 2"))
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Please Open TF2" << endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSE_WHITE);
	}

	while (!FindWindow(NULL, L"Team Fortress 2")) { Sleep(500); }

	cout << endl;

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "TF2 has been found." << endl << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSE_WHITE);
	////////////////////////////////////////////////////////////////

	offsetsTf2 = new GameOffsetsTf2();
	offsetsTf2->printOffsets();

	char yn;

	SetConsoleTextAttribute(hConsole, FOREGROUND_CYAN);

	cout << "Do you want to save to a file [Y] Yes | [N] No : ";
	cin >> yn;

	// todo: concat the file name to the out path in the save method.
	if (yn == 'Y' || yn == 'y') offsetsTf2->save(".\\out\\Tf2_offsets.txt");

	cout << "Bye!" << endl;
	Sleep(300);
	return 0;
}
