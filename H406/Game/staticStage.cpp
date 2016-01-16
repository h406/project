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

//  auto sky = MeshDome::create(20, 20, 300.f, 300.f);
//  sky->setTexture("./data/texture/sky.jpg");
//  sky->setPosY(-450.f);
//  this->addChild(sky);

  //XFileObject* backField = XFileObject::create("./data/model/Building.x");
  XFileObject* backField = XFileObject::create("./data/model/Bill_03.x"); 
  backField->setPos(Vec3(0, -420, 0));
  backField->setScl(Vec3(20,20,20));
  this->addChild(backField);

  XFileObject* higthway = XFileObject::create("./data/model/hightway.x");
  higthway->setPos(Vec3(0, -10, 0));
  higthway->setScl(Vec3(45, 45, 45));
  this->addChild(higthway);

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