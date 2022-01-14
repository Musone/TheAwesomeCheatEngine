#include "Tf2Overlay.h"


Tf2Overlay* overlay = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


Tf2Overlay::Tf2Overlay(IOffset* offsets, Process* process, Screen* screen, HINSTANCE hInstance, int nCmdShow)
{
	overlay = this;
	offsets_ = offsets;
	process_ = process;
	screen_ = screen;
	hInstance_ = hInstance;
	nCmdShow_ = nCmdShow;

	mutex_ = CreateMutexA(NULL, FALSE, NULL);
	thread_ = CreateThread(NULL, 0, threadInit, (LPVOID)this, 0, NULL);
}


int Tf2Overlay::startOverlay()
{
	registerClass(hInstance_);

	targetHWND = FindWindowA(0, targetWindowName);
	if (targetHWND)
	{
		if (resize())
		{
			MoveWindow(overlayHWND, rect_.left, rect_.top, screen_->width_,
			           screen_->height_, true);
		}
	}
	else
		return FALSE;

	// Perform application initialization:
	if (!InitInstance(hInstance_, SW_SHOW))
	{
		return FALSE;
	}
	// hWnd = overlayHWND;
	paint = Paint(offsets_, process_, screen_, overlayHWND, targetHWND);

	while (!terminateThread_)
	{
		WaitForSingleObject(mutex_, INFINITE);
		paint.render();
		ReleaseMutex(mutex_);
		Sleep(10);
	}

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0) && !terminateThread_)
	{
		WaitForSingleObject(mutex_, INFINITE);

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		ReleaseMutex(mutex_);

		if (resize())
		{
			MoveWindow(overlayHWND, rect_.left, rect_.top, screen_->width_, screen_->height_, true);
		}
		// Sleep(150);
	}

	return (int)msg.wParam;
}


bool Tf2Overlay::resize()
{
	RECT newRect;
	GetWindowRect(targetHWND, &newRect);
	if (newRect.right == rect_.right && newRect.left == rect_.left &&
		newRect.top == rect_.top && newRect.bottom == rect_.bottom)
	{
		return false;
	}

	int newWidth = newRect.right - newRect.left;
	int newHeight = newRect.bottom - newRect.top;
	rect_ = newRect;
	screen_->width_ = newWidth;
	screen_->height_ = newHeight;
	return true;
}

// todo: thread copy pasta ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tf2Overlay::~Tf2Overlay()
{
	closeThread();
	CloseHandle(thread_);
	CloseHandle(mutex_);
}

DWORD WINAPI Tf2Overlay::threadInit(LPVOID vthis)
{
	Tf2Overlay* thus = (Tf2Overlay*)vthis;
	thus->startOverlay();
	return 0;
}

void Tf2Overlay::closeThread()
{
	pause();
	terminateThread_ = true;
	resume();
	WaitForSingleObject(thread_, INFINITE);
}

void Tf2Overlay::openThread()
{
	thread_ = CreateThread(NULL, 0, threadInit, (LPVOID)this, 0, NULL);
}

void Tf2Overlay::pause()
{
	if (paused_) return;
	WaitForSingleObject(mutex_, INFINITE);
	paused_ = true;
}

void Tf2Overlay::resume()
{
	if (!paused_) return;
	paused_ = false;
	ReleaseMutex(mutex_);
}

// todo: thread copy pasta ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//
//  FUNCTION: registerClass()
//
//  PURPOSE: Registers the window class.
//
ATOM Tf2Overlay::registerClass(HINSTANCE hInstance)
{
	wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszMenuName = overlayWindowName;
	wcex.lpszClassName = overlayWindowName;
	wcex.hIconSm = 0;

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL Tf2Overlay::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	overlayHWND =
		CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		                overlayWindowName, overlayWindowName, WS_POPUP, rect_.left, rect_.top,
		                screen_->width_, screen_->height_, 0, 0, hInstance, 0);

	if (!overlayHWND)
	{
		return FALSE;
	}
	SetLayeredWindowAttributes(overlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);

	ShowWindow(overlayHWND, nCmdShow);

	return TRUE;
}

void Tf2Overlay::render()
{
	paint.render();
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process_ the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK Tf2Overlay::WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		// overlay->render();
		overlay->render();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
