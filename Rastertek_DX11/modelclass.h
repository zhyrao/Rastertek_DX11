#pragma once

// includes
#include <d3d11.h>
#include <d3dx10math.h>

class ModelClass
{
private:
	// define for vertex type, must match the layout in the shader class
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* device);
	void ShutDown();
	void Render(ID3D11DeviceContext* deviceContext);

	inline int GetIndexCount() { return m_IndexCount; }
private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutDownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;
};

