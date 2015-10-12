//==============================================================================
//
// create func
// Author : Yasuaki Yamashita : 2014/12/29
//
//==============================================================================

#pragma once

#ifndef _ICREATEFUNC_H_
#define _ICREATEFUNC_H_

//==============================================================================
// classs
//------------------------------------------------------------------------------
template<class Derived>
struct CreateFunc {
  template<class... Args>
  static Derived* create(Args&&... args) {
    auto p = new Derived();
    if(p->init(std::forward<Args>(args)...)) {
      return p;
    } else {
      delete p;
      return nullptr;
    }
  }
};
#endif
//EOF