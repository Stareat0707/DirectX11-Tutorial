#pragma once

#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "TextureShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool initialize(int&, int&, HWND);
	void shutdown();
	bool frame();

private:
	bool render();

private:
	D3D* m_d3d;
	Camera* m_camera;	
	Model* m_model;
	TextureShader* m_textureShader;
};
