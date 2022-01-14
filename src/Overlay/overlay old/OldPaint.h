#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "../../Modules/Process.h"
#include "../../Resources/StructsAndResources.h"

// todo: this really shouldnt be here
#include "../../Modules/Tf2Tracking.h"

class OldPaint {
 public:
  OldPaint(){};

  OldPaint(IOffset* offsets, Process* process, Screen* screen, HWND overLayHWND,
        HWND targetHWND, DWORD entListBase, DWORD localPlayerBase);

  ~OldPaint(){};

  int render();

 private:
  IDirect3D9Ex* object_ = NULL;
  IDirect3DDevice9Ex* device_ = NULL;
  D3DPRESENT_PARAMETERS params_;
  ID3DXFont* font_ = 0;
  HWND TargetHWND_ = 0;
  // int width_ = 0;
  // int height_ = 0;

  // todo: addons ~~~~~~~~~~~~
  Process* process_ = 0;
  Screen* screen_ = 0;
  DWORD entListBase_ = 0;
  DWORD localPlayerBase_ = 0;
  IOffset* offsets_ = 0;

  Geometry Geometry_;

  // float xfov_ = 107.0f;
  // double xfov_ = 107.0;
  // double yfov_ = 65.0;
  // double yfov_ = xfov_ * 9.0 / 16.0;
  double threshold_ = 1.0;

  // todo: addons ~~~~~~~~~~~~

  int init(HWND hWnd);

  void drawText(char* string, int x, int y, int a, int r, int g, int b);
  void drawPlayers();
  // Angle_t normalizeAngle(Angle_t angle);

  // todo: remove this copypasta with abstractions
  ClientInfo_t getClientAtIndex(DWORD index);
  void drawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
  void drawFov();
  // bool worldToScreen(DWORD playerBase);

  // Vec2_t worldToScreen(Vec3_t pos);
  // void show_int(int x);
};
