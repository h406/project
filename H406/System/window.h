//==============================================================================
//
// window[window.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _WINDOW_H_
#define _WINDOW_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class Window {
public:

  static LRESULT CALLBACK winProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

  virtual void update() {}

  HWND getHWnd() const { return _hWnd; }
  HINSTANCE getHInst() const { return _hInst; }
  SIZE getWindowSize() const { return _WindowSize; }

  int run();

  void setTitle(const char* text);

  void exit() { _bExitApplication = true; }

  int getFps() const { return _nCountFPS; }

protected:

  Window(int width,int height);
  virtual ~Window();

  HWND _hWnd;
  HINSTANCE _hInst;
  SIZE _WindowSize;

  bool _bExitApplication;
  int _nCountFPS;

};

#endif
//EOF
