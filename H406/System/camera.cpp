//==============================================================================
//
// camera[camera.cpp]
// Author : Yasuaki Yamashita : 2015/10/01
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "camera.h"

//==============================================================================
// 
//------------------------------------------------------------------------------
void Camera::CameraEx::update() {
  // ビューマトリックス
  D3DXMatrixIdentity(&_mtxView); // ビューマトリックスの初期化
  D3DXMatrixLookAtLH(&_mtxView,&_posCameraP,&_posCameraR,&_vecCameraU);

  _update = false;
}

//==============================================================================
//
//------------------------------------------------------------------------------
Camera::Camera()
  :_currentCam(nullptr)
  ,_moveToCamera(nullptr)
  ,_nowFrame(0)
  ,_maxFrame(0)
{
  // デフォルトカメラ
  _defaultCam = createCamera();
  _defaultCam->setPosP(Vec3(0,50,-200));
  _defaultCam->setPosR(Vec3(0,0,0));
  _defaultCam->setVecU(Vec3(0,1,0));

  _destCamera = createCamera();
  setCamera(_defaultCam);
}

//==============================================================================
//
//------------------------------------------------------------------------------
Camera::~Camera() {
  cameraAllRelease();
}

//==============================================================================
// createCamera
//------------------------------------------------------------------------------
CameraBace* Camera::createCamera() {
  auto cam = new CameraEx();
  _cameraList.push_back(cam);
  return cam;
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Camera::releaseCamera(CameraBace* camera) {
  auto it = remove_if(_cameraList.begin(),_cameraList.end(),[camera](CameraEx* cam) {return camera == cam;});
  _cameraList.erase(it);
  SafeDelete(camera);
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Camera::setCamera(CameraBace* moveToCamera,int frame) {
  if(_moveToCamera != nullptr) return;
  if(frame == 0) {
    setCamera(moveToCamera);
    return;
  }

  _moveToCamera = moveToCamera;
  _maxFrame = frame;
  _nowFrame = 0;

  memcpy(_destCamera,_currentCam,sizeof(CameraBace));
  memcpy(_defaultCam,_currentCam,sizeof(CameraBace));
  setCamera(_defaultCam);
}

//==============================================================================
// createCamer
//-------------------------------------------------------------------------------
void Camera::createCameraAllRelease() {
  memcpy(_defaultCam,_currentCam,sizeof(CameraBace));
  setCamera(_defaultCam);

  for(auto& cam : _cameraList) {
    if(_defaultCam != cam) {
      releaseCamera(cam);
    }
  }
}

//==============================================================================
///
//------------------------------------------------------------------------------
void Camera::cameraAllRelease() {
  for(auto& cam : _cameraList) {
    SafeDelete(cam);
  }
  _cameraList.clear();
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Camera::update() {
  if(_moveToCamera != nullptr) {
    float time = sinf(D3DX_PI * 0.5f * (_nowFrame / (float)_maxFrame));
    _defaultCam->setPosP(_destCamera->getPosP() * (1 - time) + _moveToCamera->getPosP() * time);
    _defaultCam->setPosR(_destCamera->getPosR() * (1 - time) + _moveToCamera->getPosR() * time);
    _defaultCam->setVecU(_destCamera->getVecU() * (1 - time) + _moveToCamera->getVecU() * time);
    _nowFrame++;

    if(_nowFrame > _maxFrame) {
      setCamera(_moveToCamera);
      _nowFrame = 0;
      _maxFrame = 0;
      _moveToCamera = nullptr;
    }
  }

  for(auto& cam : _cameraList) {
    if(cam->isUpdate()) {
      cam->update();
    }
  }
}

//EOF