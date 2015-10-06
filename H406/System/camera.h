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
public :

  void update();

  Camera();
  ~Camera();

  CameraBace* createCamera();
  void releaseCamera(CameraBace* camera);

  void setCurrentCamera(CameraBace* camera) { _currentCam = camera; }

  const Matrix& getViewMtx() { return _currentCam->getViewMtx(); }

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
};

#endif
//EOF