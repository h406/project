//==============================================================================
//
// textureManager[textureManager.cpp]
// Author : Yasuaki Yamashita : 2015/09/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "texture.h"
#include "renderer.h"

//==============================================================================
// 
//------------------------------------------------------------------------------
Texture::Texture(const Renderer* renderer)
  :_renderer(renderer) {

  // 1x1の白色tgaテクスチャ
  const BYTE WhiteTga1x1TextureFileImage[] =
  {
    0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x18,0x00,0xFF,0xFF,0xFF
  };

  LPDIRECT3DTEXTURE9 tex = nullptr;
  D3DXCreateTextureFromFileInMemory(renderer->getDevice(),WhiteTga1x1TextureFileImage,sizeof(WhiteTga1x1TextureFileImage),&tex);
  _TexList.push_back(tex);
}

//==============================================================================
// 
//------------------------------------------------------------------------------
Texture::~Texture() {
  for(auto& obj : _TexList) {
    SafeRelease(obj);
  }
  _TexList.clear();
  _TexMap.clear();
}

//==============================================================================
// 
//------------------------------------------------------------------------------
unsigned int Texture::createTexture(const char* filename) {
  auto it = _TexMap.find(filename);
  if(it != _TexMap.end()) return it->second;
  LPDIRECT3DTEXTURE9 tex = nullptr;

  D3DXCreateTextureFromFile(_renderer->getDevice(),filename,&tex);
  _TexList.push_back(tex);
  _TexMap.insert(pair<string,int>(filename,_TexList.size() - 1));
  return _TexList.size() - 1;
}

//==============================================================================
// 
//------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 Texture::getTexture(unsigned int id) {
  return _TexList[id];
}

//==============================================================================
// 
//------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 Texture::getTexture(const char* filename) {
  auto it = _TexMap.find(filename);
  if(it == _TexMap.end()) return nullptr;
  return _TexList[it->second];
}

//EOF