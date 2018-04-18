#pragma once

#include <D3DX10math.h>

class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void setDiffuseColor(float, float, float, float);
	void setDirection(float, float, float);

	D3DXVECTOR4 getDiffuseColor();
	D3DXVECTOR3 getDirection();

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};
