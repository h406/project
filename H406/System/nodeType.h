//==============================================================================
//
// nodeType[nodeType.h]
// Author : Yasuaki Yamashita : 2015/10/08
//
//==============================================================================

#pragma once

#ifndef _NODETYPE_H_
#define _NODETYPE_H_

//==============================================================================
// node type
//------------------------------------------------------------------------------
enum class NodeType {
  normal3D = 0,
  lightOff3D,
  normal2D,
  effect,
  default = (int)NodeType::normal3D,
};

#endif
//EOF