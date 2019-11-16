#pragma once

#include <d3d11.h>
#include <D3DX11tex.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Initialize(ID3D11Device* device, LPCSTR file);
	void ShutDown();


	inline ID3D11Texture2D* GetTexture2D() { return m_Texture; }
	inline ID3D11ShaderResourceView* GetTextureSRV() { return m_TextureSRV; }
private:
	ID3D11Texture2D* m_Texture;
	ID3D11ShaderResourceView* m_TextureSRV;
};

