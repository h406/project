//==============================================================================
//
// iColorOption[iColorOption.h]
// Author : Yasuaki Yamashita : 2015/12/24
//
//==============================================================================

#pragma once

#ifndef _ICOLOROPTION_H_
#define _ICOLOROPTION_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class iColorOption {
public:
  iColorOption() : _color(1,1,1,1) {};

  void setColor(const D3DXCOLOR& color) { _color = color; }
  D3DXCOLOR& getColor() { return _color; }
protected:
  D3DXCOLOR _color;
};

#endif
//EOF