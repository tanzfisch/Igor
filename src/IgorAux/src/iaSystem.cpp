// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaSystem.h>

#ifdef __IGOR_WINDOWS__
#include <Windows.h>
#include <DbgHelp.h>
#endif

namespace IgorAux
{

	/*! thank you Macmade
	http://stackoverflow.com/questions/5693192/win32-backtrace-from-c-code
	*/
	void getCallStack(std::vector<iaString>& callStack)
	{
#ifdef __IGOR_WINDOWS__
		void* stack[100];

		HANDLE process = GetCurrentProcess();
		SymInitialize(process, NULL, TRUE);

		unsigned short  frames = CaptureStackBackTrace(0, 100, stack, NULL);
		SYMBOL_INFO* symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		for (unsigned int i = 1; i < frames; i++)
		{
			SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
			callStack.push_back(symbol->Name);
		}

		free(symbol);
#endif
	}

}
