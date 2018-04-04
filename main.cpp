#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR psCmdline, int iCmdshow)
{
	bool result;

	// system 객체를 생성한다.
	System* system = new System;
	if (!system)
	{
		return 0;
	}

	// system 객체를 초기화하고 run을 호출한다.
	result = system->initialize();
	if (result)
	{
		system->run();
	}

	// system 객체를 종료하고 메모리를 반환한다.
	system->shutdown();
	delete system;
	system = 0;

	return 0;
}


