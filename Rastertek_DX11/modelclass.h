#pragma once

// includes
#include <d3d11.h>
#include <d3dx10math.h>

// class include
#include "textureclass.h"

class ModelClass
{
private:
	// define for vertex type, must match the layout in the shader class
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* device, LPCSTR textureFile);
	void ShutDown();
	void Render(ID3D11DeviceContext* deviceContext);


	inline int GetIndexCount() { return m_IndexCount; }
	inline ID3D11ShaderResourceView* GetTextureSRV() { return m_Texture->GetTextureSRV(); }
private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutDownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, LPCSTR file);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;

	TextureClass* m_Texture;
};

