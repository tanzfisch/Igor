// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaSystem.h>

#ifdef IGOR_WINDOWS
#include <Windows.h>
#include <DbgHelp.h>
#endif

#ifdef __IGOR_LINUX__
#include <execinfo.h>
#endif

namespace iaux
{

	/*! thank you Macmade
	http://stackoverflow.com/questions/5693192/win32-backtrace-from-c-code
	*/
	void getCallStack(std::vector<iaString> &callStack)
	{
		callStack.clear();

#ifdef IGOR_WINDOWS
		void *stack[100];

		HANDLE process = GetCurrentProcess();
		SymInitialize(process, NULL, TRUE);

		unsigned short frames = CaptureStackBackTrace(0, 100, stack, NULL);
		SYMBOL_INFO *symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		for (unsigned int i = 1; i < frames; i++)
		{
			SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
			callStack.push_back(symbol->Name);
		}

		free(symbol);
#endif

#ifdef __IGOR_LINUX__
		const int buffSize = 100;

		void *buffer[buffSize];
		int count = backtrace(buffer, buffSize);

		char **strings = backtrace_symbols(buffer, count);
		if (strings != nullptr && count > 2)
		{
			for (int i = 2; i < count; ++i)
			{
				callStack.push_back(iaString(strings[i]) + iaString('\n'));
			}
		}

		free(strings);
#endif
	}

} // namespace iaux
