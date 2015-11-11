//==============================================================================
//
// camera[camera.h]
// Author : Yasuaki Yamashita : 2015/10/01
//
//==============================================================================

#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "cameraBace.h"
//==============================================================================
// 
//------------------------------------------------------------------------------
class Camera {
public:

  void update();

  Camera();
  ~Camera();

  CameraBace* createCamera();
  void releaseCamera(CameraBace* camera);

  void setCamera(CameraBace* camera) { _currentCam = camera; }

  const Matrix& getViewMtx() const { return _currentCam->getViewMtx(); }
  const CameraBace* getCurrentCamera() const { return _currentCam; }

  void setCamera(CameraBace* camera,int frame);

  void cameraAllRelease();

  void createCameraAllRelease();

private:
  class CameraEx : public CameraBace {
  public:
    void update();
    CameraEx() {};
    ~CameraEx() {};

    bool isUpdate() { return _update; }
  };

  list<CameraEx*> _cameraList;

  CameraBace* _currentCam;
  CameraBace* _defaultCam;

  CameraBace* _destCamera;
  CameraBace* _moveToCamera;
  int _nowFrame;
  int _maxFrame;
};

#endif
//EOF