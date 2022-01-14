#include "OldPaint.h"

OldPaint::OldPaint(IOffset* offsets, Process* process, Screen* screen,
             HWND overLayHWND, HWND targetHWND, DWORD entListBase,
             DWORD localPlayerBase) {
  offsets_ = offsets;
  process_ = process;
  screen_ = screen;
  entListBase_ = entListBase;
  localPlayerBase_ = localPlayerBase;

  TargetHWND_ = targetHWND;
  init(overLayHWND);
}

int OldPaint::init(HWND hWnd) {
  if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &object_))) {
    exit(1);
  }

  ZeroMemory(&params_, sizeof(params_));

  params_.BackBufferWidth = screen_->width_;
  params_.BackBufferHeight = screen_->height_;
  params_.Windowed = true;
  params_.hDeviceWindow = hWnd;
  params_.SwapEffect = D3DSWAPEFFECT_DISCARD;
  params_.MultiSampleQuality = D3DMULTISAMPLE_NONE;
  params_.BackBufferFormat = D3DFMT_A8R8G8B8;
  params_.EnableAutoDepthStencil = TRUE;
  params_.AutoDepthStencilFormat = D3DFMT_D16;

  object_->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                          D3DCREATE_HARDWARE_VERTEXPROCESSING, &params_, 0,
                          &device_);

  D3DXCreateFont(device_, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
                 OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH,
                 L"Comic Sans", &font_);  // todo: what a meme..

  return 0;
}

void OldPaint::drawText(char* string, int x, int y, int a, int r, int g, int b) {
  RECT rect;
  rect.top = y;
  rect.left = x;
  font_->DrawTextA(0, string, strlen(string), &rect, DT_NOCLIP,
                   D3DCOLOR_ARGB(a, r, g, b));
}

void OldPaint::drawLine(int x1, int y1, int x2, int y2, int thickness,
                     D3DCOLOR color) {
  ID3DXLine* LineL;
  D3DXCreateLine(device_, &LineL);

  D3DXVECTOR2 Line[2];

  Line[0] = D3DXVECTOR2(x1, y1);
  Line[1] = D3DXVECTOR2(x2, y2);
  LineL->SetWidth(thickness);
  LineL->Draw(Line, 2, color);
  LineL->Release();
}

int OldPaint::render() {
  if (device_ == NULL) return 1;

  device_->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
  device_->BeginScene();

  if (TargetHWND_ == GetForegroundWindow()) {
    drawFov();
    drawPlayers();
  }

  device_->EndScene();
  device_->PresentEx(0, 0, 0, 0, 0);

  return 0;
}

void OldPaint::drawFov() {
  int halfWidth = screen_->width_ / 2;
  int halfHeight = screen_->height_ / 2;
  int halfFov = screen_->aimbotFov_ / 2;

  int x1 = halfWidth - halfFov;
  int y1 = halfHeight - halfFov;

  int x2 = halfWidth + halfFov;
  int y2 = halfHeight + halfFov;

  D3DCOLOR color = D3DCOLOR_ARGB(255, 0xf5, 0, 0);

  drawLine(x1, y1, x1, y2, 2, color);
  drawLine(x1, y1, x2, y1, 2, color);
  drawLine(x1, y2, x2, y2, 2, color);
  drawLine(x2, y1, x2, y2, 2, color);
  // #f50000
}

void OldPaint::drawPlayers() {
  // LocalCoords_t local = tracking_->getLocalCoordinate();
  ClientInfo_t client = {0};
  Entity_t player = {0};
  Vec2_t pos = {0};
  float viewMatrix[16] = {0};
  // Angle_t normAngle = {0};

  Entity_t localent = {0};
  // process_->readAddress(localPlayerBase_, (BYTE*)&localent, sizeof(localent));
  process_->readAddress(offsets_->localPlayerBase(), (BYTE*)&localent, sizeof(localent));

  int drawnEntities = MAX_PLAYERS;

  for (DWORD i = 0; i < MAX_PLAYERS; ++i) {
    client = getClientAtIndex(i);
    printf("Index: %d\n", i);

    Angle_t relativeAngle{0};
    Vec3_t result = {0};
    Entity_t playerEnt = {0};

    if (client.entity) {
      process_->readAddress(client.entity, (BYTE*)&playerEnt,
                            sizeof(playerEnt));

      if ((playerEnt.teamNo == 2 || playerEnt.teamNo == 3) &&
          playerEnt.teamNo != localent.teamNo && playerEnt.deadFlag != 0x202) {
        result.x = playerEnt.x5;
        result.y = playerEnt.y5;
        result.z = playerEnt.z5 + 70.0f;

        // relativeAngle = Geometry_.getRelativeAngle(result, local);
        // relativeAngle.yaw *= -1.0f /** 0.841121495327*/;
        // normAngle = Geometry_.normalizeAngle(relativeAngle, screen_);

        // todo: get this from offsets...
        process_->readAddress(offsets_->engineBase() + VIEW_MATRIX_OFFSET,
                              (BYTE*)&viewMatrix, sizeof(viewMatrix));
        pos = Geometry_.worldToScreen(result, (float*)viewMatrix, screen_);

        if (pos.x >= 0 && pos.x <= screen_->width_ && pos.y >= 0 &&
            pos.y <= screen_->height_) {
          // D3DCOLOR_ARGB color(255, 171, 0, 182);
          // drawText((char*)".", pos.x, pos.y, 255, 171, 0, 182);
          drawLine(pos.x, pos.y, screen_->width_ / 2, screen_->height_, 2,
                   D3DCOLOR_ARGB(255, 171, 0, 182));
        }
      } else
        drawnEntities -= 1;
    } else
      drawnEntities -= 1;
  }

  //
  // char text[18] = "#entities drawn:";
  // char aChar = '0' + drawnEntities;
  // text[16] = aChar;
  // drawText((char*)text, width_ / 2.0f, height_ / 2.0f, 255, 171, 0, 182);
}

ClientInfo_t OldPaint::getClientAtIndex(DWORD index) {
  ClientInfo_t client;
  process_->readAddress(entListBase_ + sizeof(ClientInfo_t) * index,
                        (BYTE*)&client, sizeof(client));
  return client;
}
