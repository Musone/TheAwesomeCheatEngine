#include "Tf2OverlayOld.h"

#define MAX_LOADSTRING 100

// todo: Turn this into a class instead of having global spaghetti

// Global Variables:
HINSTANCE hInst;                                          // current instance
WCHAR overlayWindowName[MAX_LOADSTRING] = L"Tf2Overlay";  // The title bar text
LPCSTR targetWindowName = "Team Fortress 2";
HWND overlayHWND, targetHWND;
WNDCLASSEXW wcex;
int width, height;
OldPaint paint;

// Forward declarations of functions included in this code module:
ATOM registerClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int startOverlay(IOffset* offsets, Process* process, Screen* screen,
                 HINSTANCE hInstance, int nCmdShow, DWORD entListBase,
                 DWORD localPlayerBase);

// todo: idea: create a thread who is in charge of constantly updating local
// player since I use is so much...
DWORD WINAPI overlayThreadInit(LPVOID vthis) {
  OverLayThreadParams_t* params =
      (OverLayThreadParams_t*)vthis;  // todo: holy parameter drilling...
  startOverlay(params->offsets, params->process, params->screen,
               params->hInstance, params->nCmdShow, params->entListBase,
               params->localPlayerBase);
  // todo: need to create abstractions to get the functionality here... Also
  // need to abstract the threading process...
  return 0;
}

int startOverlay(IOffset* offsets, Process* process, Screen* screen,
                 HINSTANCE hInstance, int nCmdShow, DWORD entListBase,
                 DWORD localPlayerBase) {
  registerClass(hInstance);

  targetHWND = FindWindowA(0, targetWindowName);
  if (targetHWND) {
    RECT rect;
    GetWindowRect(targetHWND, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
  } else
    return FALSE;

  // Perform application initialization:
  if (!InitInstance(hInstance, SW_SHOW)) {
    return FALSE;
  }
  screen->width_ = width;
  screen->height_ = height;
  paint = OldPaint(offsets, process, screen, overlayHWND, targetHWND, entListBase,
                localPlayerBase);
  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    RECT rect;
    GetWindowRect(targetHWND, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    MoveWindow(overlayHWND, rect.left, rect.top, width, height, true);
  }

  return (int)msg.wParam;
}

//
//  FUNCTION: registerClass()
//
//  PURPOSE: Registers the window class.
//
ATOM registerClass(HINSTANCE hInstance) {
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // Store instance handle in our global variable

  overlayHWND =
      CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
                      overlayWindowName, overlayWindowName, WS_POPUP, 1, 1,
                      width, height, nullptr, nullptr, hInstance, nullptr);

  if (!overlayHWND) {
    return FALSE;
  }
  SetLayeredWindowAttributes(overlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);

  ShowWindow(overlayHWND, nCmdShow);

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_PAINT:
      paint.render();
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
