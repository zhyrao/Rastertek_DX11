#pragma once


// includes
#include <d3dx11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>
#include <fstream>

class ColorShaderClass
{

private:
	struct MatrixBufferType 
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass& other);
	~ColorShaderClass();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

private:
	bool InitializeShader(ID3D11Device* device, HWND hWnd, LPCSTR  vs, LPCSTR  ps);
	void ShutDownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorBlob, HWND hWnd, LPCSTR file);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);
	void RenderShader(ID3D11DeviceContext* deviceContext, int);

private:
	ID3D11VertexShader*		m_VertexShader;
	ID3D11PixelShader*		m_PixelShader;
	ID3D11InputLayout*		m_Layout;
	ID3D11Buffer*			m_MatrixBuffer;
};

