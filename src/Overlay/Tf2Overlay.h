#pragma once

#include <Windows.h>
// #include <Dwmapi.h> // todo: WTF
#include "../Modules/Process.h"
#include "../Modules/Tf2Tracking.h"
#include "Resource.h"
#include "Paint.h"

// typedef struct overLayThreadParams
// {
// 	IOffset* offsets;
// 	Process* process;
// 	// todo: we really shouldnt need this here... there should be a class that handles coordinates and angles...
//
// 	Screen* screen;
// 	HINSTANCE hInstance;
// 	int nCmdShow;
// 	DWORD entListBase;
// 	DWORD localPlayerBase;
// } OverLayThreadParams_t;


// DWORD WINAPI overlayThreadInit(LPVOID vthis);


class Tf2Overlay
{
private:
	HINSTANCE hInst; // current instance
	WCHAR overlayWindowName[30] = L"Tf2Overlay"; // The title bar text
	LPCSTR targetWindowName = "Team Fortress 2";
	HWND overlayHWND, targetHWND;
	WNDCLASSEXW wcex;
	// int width_ = 0;
	// int height_ = 0;
	Paint paint;

	int nCmdShow_;
	IOffset* offsets_;
	Process* process_;
	Screen* screen_;
	HINSTANCE hInstance_;

	HANDLE thread_;
	HANDLE mutex_;
	RECT rect_ = {0};

	bool paused_ = false;
	bool terminateThread_ = false;


	bool resize();

	// DWORD entListBase;
	// DWORD localPlayerBase;

	MSG msg;


public:
	Tf2Overlay(IOffset* offsets, Process* process, Screen* screen, HINSTANCE hInstance, int nCmdShow);
	~Tf2Overlay();

	bool isPaused() const { return paused_; }
	void pause();
	void resume();

protected:
	// int startOverlay(IOffset* offsets, Process* process, Screen* screen,
	//                  HINSTANCE hInstance, int nCmdShow, DWORD entListBase,
	//                  DWORD localPlayerBase);

	// todo: idea: create a thread who is in charge of constantly updating
	// local player since I use is so much...
	static DWORD WINAPI threadInit(LPVOID vthis);

	int startOverlay();

	ATOM registerClass(HINSTANCE hInstance);


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
	                                LPARAM lParam);

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	void render();


	void closeThread();
	void openThread();
};


// int startOverlay(HINSTANCE hInstance_, int nCmdShow_);


// class Tf2Overlay
// {
// protected:
// 	HINSTANCE hInst;
//
// 	int width_;
// 	int height_;
//
// 	LPCSTR targetWindowName = "hl2";
//
// 	WCHAR title[100] = L"My very cool overlay";
// 	ATOM registerClass(HINSTANCE hInstance_);
// 	BOOL InitInstance(HINSTANCE hInstance_, int nCmdShow_);
//
// public:
// };
