#include "StdAfx.h"
#include "PumpMessages.h"

void g_PumpMessages()
{
	MSG msg;
	while(::PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}