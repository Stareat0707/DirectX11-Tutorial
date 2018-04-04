#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR psCmdline, int iCmdshow)
{
	bool result;

	// system ��ü�� �����Ѵ�.
	System* system = new System;
	if (!system)
	{
		return 0;
	}

	// system ��ü�� �ʱ�ȭ�ϰ� run�� ȣ���Ѵ�.
	result = system->initialize();
	if (result)
	{
		system->run();
	}

	// system ��ü�� �����ϰ� �޸𸮸� ��ȯ�Ѵ�.
	system->shutdown();
	delete system;
	system = 0;

	return 0;
}


