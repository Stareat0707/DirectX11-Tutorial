#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR psCmdline, int iCmdshow)
{
	bool result;

	// system ��ü�� �����Ѵ�.
	SystemClass* System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// system ��ü�� �ʱ�ȭ�ϰ� run�� ȣ���Ѵ�.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// system ��ü�� �����ϰ� �޸𸮸� ��ȯ�Ѵ�.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}


