//==============================================================================
//
// staticStage[staticStage.cpp]
// Author : Yasuaki Yamashita : 2015/10/19
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "staticStage.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool StaticStage::init() {
  //auto backBG = MeshCylinder::create(60,10,1000.f,30.f);
  //backBG->setColor(D3DXCOLOR(1,1,1,1));
  //backBG->setTexture("./data/texture/akira001.png");
  //this->addChild(backBG);

  //auto ground = MeshField::create(100,100,5000.f,5000.f);
  //ground->setTexture("./data/texture/pave006.bmp");
  //ground->setPos(Vec3(0,-1,0));
  //this->addChild(ground);

  //MeshField* fence = MeshField::create(10,10,1000.f,100.f);
  //fence->setTexture("./data/texture/face_fence01.png");
  //fence->setRot(Vec3(D3DX_PI * -0.5f,0,0));
  //fence->setPos(Vec3(0,50,500));
  //this->addChild(fence);

  //fence = MeshField::create(10,10,1000.f,100.f);
  //fence->setTexture("./data/texture/face_fence01.png");
  //fence->setRot(Vec3(D3DX_PI * -0.5f,D3DX_PI * -0.5f,0));
  //fence->setPos(Vec3(-500,50,0));
  //this->addChild(fence);
  //
  //fence = MeshField::create(10,10,1000.f,100.f);
  //fence->setTexture("./data/texture/face_fence01.png");
  //fence->setRot(Vec3(D3DX_PI * -0.5f,D3DX_PI * 0.5f,0));
  //fence->setPos(Vec3(500,50,0));
  //this->addChild(fence);

  XFileObject* backField = XFileObject::create("./data/model/bill.x");
  backField->setPos(Vec3(130,-500,70));
  backField->setScl(Vec3(20,20,20));
  this->addChild(backField);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void StaticStage::update() {

}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void StaticStage::uninit() {

}
//EOF