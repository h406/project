//==============================================================================
//
// Renderer[renderer.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

class App;
class TextureManager;

//==============================================================================
//  class
//------------------------------------------------------------------------------
class Renderer {
public:
  Renderer(const App* app);
  ~Renderer();

  bool draw(node* baceNode);

  LPDIRECT3DDEVICE9 getDevice() const { return _pD3DDevice; }

  const Matrix& getViewMtx() const { return _mtxView; }
  const Matrix& getProjMtx() const { return _mtxProj; }

private:
  //device
  LPDIRECT3DDEVICE9 _pD3DDevice;

  // •K—v‚ÈMatrix
  Matrix _mtxProj;
  Matrix _mtxView;

  TextureManager* _textureManager;
};

#endif
//EOF
