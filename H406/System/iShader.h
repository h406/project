//==============================================================================
//
// ishader[ishader.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _iSHADER_H_
#define _iSHADER_H_

//==============================================================================
// struct
//------------------------------------------------------------------------------
struct VtxShader {
  LPDIRECT3DVERTEXSHADER9 _shader;
  LPD3DXCONSTANTTABLE _constTable;
  string filename;
};

struct PixShader {
  LPDIRECT3DPIXELSHADER9 _shader;
  LPD3DXCONSTANTTABLE _constTable;
  string filename;
};

#endif
//EOF
