//==============================================================================
//
// DrawObject[DrawObject.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _DRAWOBJECT_H_
#define _DRAWOBJECT_H_

/*
表示するオブジェクト
*/
class Renderer;

//==============================================================================
// 
//==============================================================================
class DrawObjectBace{
public:
  void setTexture(unsigned int id);
  void setTexture(const char* file);

  DrawObjectBace();
protected:
  unsigned int _vtxShaderID;
  unsigned int _textureID;
};

class DrawObject : public DrawObjectBace,public node {};
class DrawObject2D : public DrawObjectBace,public node2d {};



#endif
//EOF