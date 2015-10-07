
//==============================================================================
//
// CameraBace[CameraBace.h]
// Author : Yasuaki Yamashita : 2015/10/01
//
//==============================================================================

#pragma once

#ifndef _CAMERAINSTANCE_H_
#define _CAMERAINSTANCE_H_

//==============================================================================
// 
//==============================================================================
class CameraBace {
public:
  const Matrix& getViewMtx() const { return _mtxView; }

  void setPosP(const Vec3& posP) { _update = true; _posCameraP = posP; }
  void setPosR(const Vec3& posR) { _update = true; _posCameraR = posR; }
  void setVecU(const Vec3& posU) { _update = true; _vecCameraU = posU; }

  const Vec3& getPosP() const { return _posCameraP; }
  const Vec3& getPosR() const { return _posCameraR; }
  const Vec3& getVecU() const { return _vecCameraU; }

  CameraBace()
    :_update(true)
    ,_posCameraP(Vec3(0,0,0))
    ,_posCameraR(Vec3(0,0,0))
    ,_vecCameraU(Vec3(0,1,0)) {
  }
  virtual ~CameraBace() {}

  virtual void update() = 0;

protected:

  Matrix _mtxView;
  Vec3 _posCameraP;
  Vec3 _posCameraR;
  Vec3 _vecCameraU;

  bool _update;
};

#endif
//EOF

