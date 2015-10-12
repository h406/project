//==============================================================================
//
// qrdecode[qrdecode.h]
// Author : Yasuaki Yamashita : 2015/10/13
//
//==============================================================================

#pragma once

#ifndef _QRDECODE_H_
#define _QRDECODE_H_


#include "decodeqr.h"
#pragma comment(lib, "WS2_32.LIB")

#ifdef _DEBUG
#pragma comment(lib, "libdecodeqr_d.lib")
#else
#pragma comment(lib, "libdecodeqr.lib")
#endif


#endif
//EOF