#include "stdafx.h"
#include <conio.h>
#include <intrin.h>
#include "QueryPerfCounter.h"
#include "MultiThreadTest.h"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	QCInit();

	srand(1);

	// multi thread test
	wprintf_s(L"<Multi-Threasd Test>---------------------------------------------------\n");

	TestMultiThread();
	
	wprintf_s(L"-----------------------------------------------------------------------\n\n\n");
	//
	wprintf_s(L"Test completed. Press any key.\n");


#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

	_getch();
}