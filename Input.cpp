#include "Input.h"

Input::Input()
{
}

Input::Input(const Input& other)
{
}

Input::~Input()
{
}

void Input::initialize()
{
	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ�մϴ�.
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}

	return;
}

void Input::keyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����մϴ�.
	m_keys[input] = true;
	return;
}

void Input::keyUp(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����մϴ�.
	m_keys[input] = false;
	return;
}

bool Input::isKeyDown(unsigned int key)
{
	// ���� Ű�� ����/�� �������� ��ȯ�մϴ�.
	return m_keys[key];
}
