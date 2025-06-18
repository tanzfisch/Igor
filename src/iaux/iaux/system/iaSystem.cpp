// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaSystem.h>

#ifdef IGOR_WINDOWS
#include <Windows.h>
#include <DbgHelp.h>
#endif

#ifdef IGOR_LINUX
#include <execinfo.h>
#include <cxxabi.h>
#include <regex>
#endif

namespace iaux
{

#ifdef IGOR_LINUX
	std::string demangle(const char *mangled)
	{
		int status = -1;
		std::unique_ptr<char, void (*)(void *)> demangled(
			abi::__cxa_demangle(mangled, nullptr, nullptr, &status),
			std::free);
		return (status == 0) ? demangled.get() : mangled;
	}
#endif

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

#ifdef IGOR_LINUX
		const int buffSize = 100;

		void *buffer[buffSize];
		int count = backtrace(buffer, buffSize);
		if(count < 2)
		{
			callStack.push_back("no callstack found");
			return;
		}

		char **symbollist = backtrace_symbols(buffer, count);
		std::regex pattern(R"((.+)\(([^+]+)\+0x([0-9a-f]+)\)\s+\[(0x[0-9a-f]+)\])", std::regex::icase);

		if (symbollist == nullptr)
		{
			callStack.push_back("no symbols found");
			return;			
		}

		for (int i = 2; i < count; ++i)
		{
			iaString string;
			std::cmatch match;
			if (std::regex_match(symbollist[i], match, pattern))
			{
				const iaString binary = match[1].str().c_str();
				const iaString offset = match[3].str().c_str();
				const iaString address = match[4].str().c_str();
				const iaString demangled = demangle(match[2].str().c_str()).c_str();

				string = demangled + " +0x" + offset + " at " + address + " (" + binary + ")";
			}
			else
			{
				string = iaString(symbollist[i]);
			}


			callStack.push_back(string);
		}

		free(symbollist);
#endif
	}

} // namespace iaux
