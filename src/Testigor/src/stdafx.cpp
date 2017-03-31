// stdafx.cpp : source file that includes just the standard includes
// Testigor.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib, "Igor_d.lib")
#pragma comment(lib, "IgorAux_d.lib")
#pragma comment(lib, "OMPF_d.lib")
#else
#pragma comment(lib, "Igor.lib")
#pragma comment(lib, "IgorAux.lib")
#pragma comment(lib, "OMPF.lib")
#endif

// and not in this file
