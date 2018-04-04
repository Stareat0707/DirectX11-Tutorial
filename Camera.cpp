#include "Camera.h"

Camera::Camera()
{
	m_positionX = 0;
	m_positionY = 0;
	m_positionZ = 0;

	m_rotationX = 0;
	m_rotationY = 0;
	m_rotationZ = 0;
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::setPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::setRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 Camera::getPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 Camera::getRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::render()
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

void Camera::getViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
