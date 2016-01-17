//==============================================================================
//
// qrdecode[qrdecode.h]
// Author : Yasuaki Yamashita : 2015/10/13
//
//==============================================================================

#pragma once

#ifndef _QRDECODE_H_
#define _QRDECODE_H_

#include <string>

struct CvCapture;
using QrDecoderHandle = void*;

class QRreader {
public:
  QRreader();
  ~QRreader();

  void update();

  const std::string& getString() { return _string; }
  void clear(){ _string.clear(); }
  void charSet(const char* str){ _string = str; }

private:
  // カメラからのビデオキャプチャを初期化
  CvCapture *_VideoCapture;

  // デコーダ
  QrDecoderHandle _decoder;

  std::string _string;

};


#endif
//EOF