//==============================================================================
//
// stdafx[stdafx.h]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================

#pragma once

#ifndef _STDAFX_H_
#define _STDAFX_H_

//==============================================================================
// STLのデバッグ機能をOFFにする
//------------------------------------------------------------------------------
//#ifndef _SECURE_SCL
//#define _SECURE_SCL (0)
//#endif
//#ifndef _HAS_ITERATOR_DEBUGGING
//#define _HAS_ITERATOR_DEBUGGING (0)
//#endif
//#ifndef  _ITERATOR_DEBUG_LEVEL
//#define  _ITERATOR_DEBUG_LEVEL 0
//#endif

//==============================================================================
// lib
//------------------------------------------------------------------------------
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")

//==============================================================================
// include system
//------------------------------------------------------------------------------
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#define DIRECTINPUT_VERSION (0x0800)	// dinput.hの前に書くこと！
#include <dinput.h>

//==============================================================================
// include stl
//------------------------------------------------------------------------------
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

#include <crtdbg.h>
#ifdef _DEBUG
#define   new                   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define   malloc(s)             _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   calloc(c, s)          _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   realloc(p, s)         _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _recalloc(p, c, s)    _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _expand(p, s)         _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif
//EOF