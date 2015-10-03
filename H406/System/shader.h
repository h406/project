//==============================================================================
//
// shader[shader.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _SHADER_H_
#define _SHADER_H_

#include "iShader.h"
class Renderer;
class node;

//==============================================================================
// class
//------------------------------------------------------------------------------
class Shader {
public:
  Shader(Renderer* renderer);
  ~Shader();

  inline VtxShader* getVtxShader(unsigned int id) const;
  inline VtxShader* getVtxShader(const char* fileName) const;
  inline PixShader* getPixShader(unsigned int id) const;
  inline PixShader* getPixShader(const char* fileName) const;

  void setVtxShader(unsigned int id);
  void setVtxShader(const char* fileName);
  void setPixShader(unsigned int id);
  void setPixShader(const char* fileName);

  VtxShader* getNowVtxShader() { return _nowVtxShader; }
  PixShader* getNowPixShader() { return _nowPixShader; }

  list<node*> _postEffectNode;

private:
  VtxShader* _nowVtxShader;
  PixShader* _nowPixShader;

  map<string,unsigned int> _vtxShaderMap;
  vector<VtxShader*> _vtxShaderList;

  std::map<string,unsigned int> _pixShaderMap;
  vector<PixShader*> _pixShaderList;

  unsigned int createVtxShader(const char* file);
  unsigned int createPixShader(const char* file);

  Renderer* _renderer;

};

#endif
//EOF
