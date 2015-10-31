//==============================================================================
//
// Renderer[renderer.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

class node;
class App;
class Texture;
class Camera;
class Shader;
class Sprite2D;
class PostEffect;
class Effect;

//==============================================================================
//  class
//------------------------------------------------------------------------------
class Renderer {
public:
  Renderer() {};
  ~Renderer();

  bool init(const App* app);
  bool draw(node* baceNode);
  void update();

  LPDIRECT3DDEVICE9 getDevice() const { return _pD3DDevice; }
  const Matrix& getProjMtx() const { return _mtxProj; }

  Texture* getTexture() const { return _texture; }
  Camera* getCamera() const { return _camera; }
  Shader* getShader() const { return _shader; }
  PostEffect* getPostEffect() const { return _postEffect; }

  void setFadeBG(Sprite2D* sprite) { _fadeBG = sprite; }
  Sprite2D* getFadeBG() const { return _fadeBG; }

  LPDIRECT3DTEXTURE9 getNormalDepthTex() { return _TexNormalDepth; }
  LPDIRECT3DTEXTURE9 getPosTex() { return _TexPos; }
  LPDIRECT3DTEXTURE9 getColorTex() { return _TexColor; }

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

  // フェード
  Sprite2D* _fadeBG;

  // ポストエフェクト
  PostEffect* _postEffect;

  // 法線 + 深度
  LPDIRECT3DTEXTURE9 _TexNormalDepth;
  LPDIRECT3DSURFACE9 _SurNormalDepth;

  // ポジション
  LPDIRECT3DTEXTURE9 _TexPos;
  LPDIRECT3DSURFACE9 _SurPos;

  // カラーテクスチャ
  LPDIRECT3DTEXTURE9 _TexColor;
  LPDIRECT3DSURFACE9 _SurColor;
};

#endif
//EOF
