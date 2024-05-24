// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef STDAFX_H
#define STDAFX_H

// add headers that you want to pre-compile here

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <intrin.h>
#include "QueryPerfCounter.h"
#include <process.h>
#include "ProcessorInfo.h"
#include "Util.h"

#endif //STDAFX_H
