//==============================================================================
//
// common[common.h]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================

#pragma once

#ifndef _COMMON_H_
#define _COMMON_H_

//==============================================================================
// 
//------------------------------------------------------------------------------
template<class _T>
inline void SafeRelease(_T*& p) {
  if(p != nullptr) {
    p->Release();
    p = nullptr;
  }
}

template<class _T>
inline void SafeDelete(_T*& p) {
  if(p != nullptr) {
    delete p;
    p = nullptr;
  }
}

template<class _T>
inline void SafeDeleteArray(_T*& p) {
  if(p != nullptr) {
    delete[] p;
    p = nullptr;
  }
}

// égÇÌÇ»Ç¢ïœêîóp
template <class  _T>
void inline UnusedParam(_T param) {
  (param);
}

#endif
//EOF