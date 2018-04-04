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
	// 모든 키들을 눌리지 않은 상태로 초기화합니다.
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}

	return;
}

void Input::keyDown(unsigned int input)
{
	// 키가 눌렸다면 그 상태를 배열에 저장합니다.
	m_keys[input] = true;
	return;
}

void Input::keyUp(unsigned int input)
{
	// 키가 눌렸다면 그 상태를 배열에 저장합니다.
	m_keys[input] = false;
	return;
}

bool Input::isKeyDown(unsigned int key)
{
	// 현재 키가 눌림/뗌 상태인지 반환합니다.
	return m_keys[key];
}
