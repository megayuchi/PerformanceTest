#pragma once


#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <ctype.h>
#include <combaseapi.h>
#include <Ole2.h>
#include <initguid.h>
#include <math.h>
#include <float.h>

#include "emul_x86_simd.inl"
#include "typedef.h"
#include "math.inl"
//#include "../MegayuchiMathLib/math3d.h"
//#include "../MegayuchiGenericLib/generic_lib.h"	
#include "QueryPerfCounter.h"
#include "ProcessorInfo.h"

