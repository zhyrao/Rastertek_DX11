#include "cameraclass.h"



CameraClass::CameraClass()
{
	m_PositionX = 0;
	m_PositionY = 0;
	m_PositionZ = 0;

	m_RotationX = 0;
	m_RotationY = 0;
	m_RotationZ = 0;
}

CameraClass::CameraClass(const CameraClass &)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_PositionX = x;
	m_PositionY = y;
	m_PositionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_RotationX = x;
	m_RotationY = y;
	m_RotationZ = z;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_PositionX, m_PositionY, m_PositionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_RotationX, m_RotationY, m_RotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookat;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// set up the vertex that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// set the position of the camera in the world
	position.x = m_PositionX;
	position.y = m_PositionY;
	position.z = m_PositionZ;

	// set where the camera look at
	lookat.x = 0.0f;
	lookat.y = 0.0f;
	lookat.z = 1.0f;

	// set the yaw(Y axis) pitch(X axis) roll(Z axis) rotations in radians
	pitch = m_RotationX * 0.0174532925f;
	yaw   = m_RotationY * 0.0174532925f;
	roll  = m_RotationZ * 0.0174532925f;

	// create the rotation matrix
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// transform the lookat and up vector by the rotation matrix
	D3DXVec3TransformCoord(&lookat, &lookat, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// translate the rotated camera position to the location of viewer
	lookat = position + lookat;

	// create the view matrix from the threed updated vectors
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &lookat, &up);
}

void CameraClass::GetViewMatrix(D3DXMATRIX & view)
{
	view = m_ViewMatrix;
}
