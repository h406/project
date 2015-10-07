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
class Texture;
class Camera;
class Shader;

class Effect;

//==============================================================================
//  class
//------------------------------------------------------------------------------
class Renderer {
public:
  Renderer(const App* app);
  ~Renderer();

  bool draw(node* baceNode);
  void update();

  LPDIRECT3DDEVICE9 getDevice() const { return _pD3DDevice; }
  const Matrix& getProjMtx() const { return _mtxProj; }

  Texture* getTexture() const { return _texture; }
  Camera* getCamera() const { return _camera; }
  Shader* getShader() const { return _shader; }
  Effect* getEffect() const { return _effect; }

private:
  void createDevice(const SIZE& windowSize, HWND hWnd);

private:
  //device
  LPDIRECT3DDEVICE9 _pD3DDevice;

  // ProjectonMatrix
  Matrix _mtxProj;

  // テクスチャマネージャ
  Texture* _texture;
  
  // cam
  Camera* _camera;

  // シェーダ
  Shader* _shader;

  // effect
  Effect* _effect;

};

#endif
//EOF
