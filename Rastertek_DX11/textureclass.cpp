#include "textureclass.h"



TextureClass::TextureClass()
{
	m_TextureSRV = 0;
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device * device, LPCSTR file)
{
	HRESULT result;

// 	D3D11_TEXTURE2D_DESC texDesc;
// 	texDesc.Width = 
// 	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
// 	texDesc.ArraySize = 0;
// 	texDesc.CPUAccessFlags = 0;
// 	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	result = D3DX11CreateShaderResourceViewFromFile(device, file, NULL, NULL, &m_TextureSRV, NULL);
	if (FAILED(result))
		return false;
		
	return true;
}

void TextureClass::ShutDown()
{
	if (m_TextureSRV)
	{
		m_TextureSRV->Release();
		m_TextureSRV = 0;
	}
}
