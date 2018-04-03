#include "inputclass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ�մϴ�.
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����մϴ�.
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����մϴ�.
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// ���� Ű�� ����/�� �������� ��ȯ�մϴ�.
	return m_keys[key];
}
