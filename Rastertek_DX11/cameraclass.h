#pragma once

// includes
#include <D3DX10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX& view);

private:
	float m_PositionX, m_PositionY, m_PositionZ;
	float m_RotationX, m_RotationY, m_RotationZ;
	D3DXMATRIX m_ViewMatrix;
};

