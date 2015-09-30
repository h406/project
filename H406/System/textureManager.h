//==============================================================================
//
// textureManager[textureManager.h]
// Author : Yasuaki Yamashita : 2015/09/29
//
//==============================================================================

#pragma once

#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

class Renderer;

//==============================================================================
//  
//------------------------------------------------------------------------------
class TextureManager {
public:
  TextureManager(const Renderer* renderer);
  ~TextureManager();

  unsigned int createTexture(const char* filename);

  LPDIRECT3DTEXTURE9 getTexture(unsigned int id);
  LPDIRECT3DTEXTURE9 getTexture(const char* filename);

private:
  vector<LPDIRECT3DTEXTURE9> _TexList;
  map<string,unsigned int> _TexMap;
  const Renderer* _renderer;


};

#endif
//EOF