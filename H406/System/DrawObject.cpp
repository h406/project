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


void DrawObject::setTexture(unsigned int id) {
  _textureID = id;
}

void DrawObject::setTexture(const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
}

DrawObject::DrawObject()
  :_textureID(0) {

}


//EOF