//==============================================================================
//
// main[main.cpp]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================


#include "title.h"

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
  UnusedParam(hInstance);
  UnusedParam(hPrevInstance);
  UnusedParam(lpCmdLine);
  UnusedParam(nCmdShow);

  App& app = App::instance(1280,720);

  // �^�C�g��
  app.setTitle("H406");

  // �ŏ��̃V�[��
  app.setBaceScene(Title::create<Title>());

  return app.run();
}



//EOF