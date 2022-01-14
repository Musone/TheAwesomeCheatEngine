#pragma once

#include <Windows.h>

#include "../../Modules/Process.h"
#include "../../Modules/Tf2Tracking.h"
// #include "../Paint.h"
#include "OldPaint.h"
#include "../Resource.h"

typedef struct overLayThreadParams {
  IOffset* offsets;
  Process* process;
  // todo: we really shouldnt need this here... there should be a class that
  // handles coordinates and angles...

  Screen* screen;
  HINSTANCE hInstance;
  int nCmdShow;
  DWORD entListBase;
  DWORD localPlayerBase;
} OverLayThreadParams_t;

DWORD WINAPI overlayThreadInit(LPVOID vthis);
// int startOverlay(HINSTANCE hInstance, int nCmdShow);

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
// 	ATOM registerClass(HINSTANCE hInstance);
// 	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
//
// public:
// };
