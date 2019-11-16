#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX worldMat;
		D3DXMATRIX viewMat;
		D3DXMATRIX projectionMat;
	};
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass& other);
	~TextureShaderClass();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int indecCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* textureSRV);

private:
	bool InitializeShader(ID3D11Device* device, HWND hWnd, LPCSTR vs, LPCSTR ps);
	void ShutDownShaders();
	void OutputErrorMessage(ID3D10Blob* erroBlob, HWND hWnd, LPCSTR file);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX project, ID3D11ShaderResourceView* textureSRV);
	bool RenderShader(ID3D11DeviceContext* deviceContext, int indexCont);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader*  m_PixelShader;
	ID3D11InputLayout* m_Layout;
	ID3D11Buffer*	m_MatrixBuffer;

	// sampler state
	ID3D11SamplerState* m_SamplerState;
};

