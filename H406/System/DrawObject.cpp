//==============================================================================
//
// DrawObject[DrawObject.cpp]
// Author : Yasuaki Yamashita : 2015/10/12
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "DrawObject.h"

#include "app.h"
#include "renderer.h"
#include "texture.h"
#include "node.h"

//==============================================================================
// 
//------------------------------------------------------------------------------
void DrawObjectBace::setTexture(unsigned int id) {
  _textureID = id;
}

void DrawObjectBace::setTexture(const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
}

DrawObjectBace::DrawObjectBace()
  :_textureID(0) {

}


//EOF