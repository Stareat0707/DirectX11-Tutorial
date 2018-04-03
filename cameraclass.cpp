#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_positionX = 0;
	m_positionY = 0;
	m_positionZ = 0;

	m_rotationX = 0;
	m_rotationY = 0;
	m_rotationZ = 0;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	// ������ ����Ű���� ���͸� �����մϴ�.
	D3DXVECTOR3 up;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// ������ ī�޶� ��ġ�� �����մϴ�.
	D3DXVECTOR3 position;
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// �⺻������ ī�޶� ���� �ִ� ��ġ�� �����մϴ�.
	D3DXVECTOR3 lookAt;
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Yaw(Y��), Pitch(X��), Roll(Z��)ȸ���� �������� �����մϴ�.
	float pitch = m_rotationX * 0.0174532925f;
	float yaw = m_rotationY * 0.0174532925f;
	float roll = m_rotationZ * 0.0174532925f;

	//�¿� �̵�, ��ġ �� �� ������ ȸ�� ��Ʈ���� ����
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// �������� ���Ⱑ �ùٸ��� ȸ���ϵ��� ȸ�� ��Ʈ������ ���� ���� �� ���� ���͸� ��ȯ�մϴ�.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// ȸ���� ī�޶� ��ġ�� ��� ��ġ�� ��ȯ�մϴ�.
	lookAt += position;

	// ������Ʈ�� �� ���� ���Ϳ��� �� ��Ʈ������ �����մϴ�.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
