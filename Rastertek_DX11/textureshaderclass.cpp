#include "textureshaderclass.h"



TextureShaderClass::TextureShaderClass()
{
}


TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device * device, HWND hWnd)
{
	bool result;

	result = InitializeShader(device, hWnd, "data/shader/texture.vs", "data/shader/texture.ps");
	if (!result)
		return false;

	return true;
}

void TextureShaderClass::ShutDown()
{
	// shut down the shaders
	ShutDownShaders();
}

bool TextureShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView * textureSRV)
{
	bool result;
	
	// set the shader parameters
	result = SetShaderParameters(deviceContext, world, view, projection, textureSRV);
	if (!result)
		return false;

	// render the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShaderClass::InitializeShader(ID3D11Device * device, HWND hWnd, LPCSTR vs, LPCSTR ps)
{
	HRESULT result;
	ID3D10Blob* errorBlob;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;


	// init
	errorBlob = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// comiple the texture code
	result = D3DX11CompileFromFile(vs, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorBlob, NULL);
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputErrorMessage(errorBlob, hWnd, vs);
		}
		else
		{
			MessageBox(hWnd, vs, "Missing Shader File", MB_OK);
		}
		return false;
	}

	// compile the pixel shader
	result = D3DX11CompileFromFile(ps, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorBlob, NULL);
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputErrorMessage(errorBlob, hWnd, ps);
		}
		else
		{
			MessageBox(hWnd, ps, "Missing Shader File", MB_OK);
		}
		return false;
	}

	// create vertex shader
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(result))
		return false;

	// create the pixel shader
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(result))
		return false;

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_Layout);
	if (FAILED(result))
		return false;

	// release the vertex/pixle buffer
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// matrix constant buffer
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// create the constant buffer 
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
	if (FAILED(result))
		return false;

	// create sampler state
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	// create the sampler satte
	result = device->CreateSamplerState(&samplerDesc, &m_SamplerState);
	if (FAILED(result))
		return false;

	return true;
}

void TextureShaderClass::ShutDownShaders()
{
	if (m_SamplerState)
	{
		m_SamplerState->Release();
		m_SamplerState = 0;
	}

	if (m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}

	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}
}

void TextureShaderClass::OutputErrorMessage(ID3D10Blob * erroBlob, HWND hWnd, LPCSTR file)
{
	char* compileErrors;
	unsigned long bufferSize;
	std::ofstream fout;

	// get pointer to the error message and message size
	compileErrors = (char*)erroBlob->GetBufferPointer();
	bufferSize = erroBlob->GetBufferSize();

	// create file
	fout.open("shader-error.txt");

	// write the image
	for (unsigned long i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// close the file
	fout.close();

	// release the error message
	erroBlob->Release();
	erroBlob = 0;

	// pop a message box the notify
	MessageBox(hWnd, "Error compiling shader.   Check shader-error.txt for message.", file, MB_OK);
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext * deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX project, ID3D11ShaderResourceView * textureSRV)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the metrices to prepare for the shader
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&project, &project);

	// lock the constant buffer so it can be written to
	result = deviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	// get the mapped buffer address
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->worldMat = world;
	dataPtr->viewMat = view;
	dataPtr->projectionMat = project;

	// unlocn the constant buffer
	deviceContext->Unmap(m_MatrixBuffer, NULL);

	// set the position of the constant buffer in the vertex shader(slot)
	bufferNumber = 0;

	// set the constant buffer in the vertex shader with upload values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_MatrixBuffer);

	// set the shader texture in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &textureSRV);

	return true;
}

bool TextureShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCont)
{
	// set the vertex input layout
	deviceContext->IASetInputLayout(m_Layout);

	// set the vertex and pixel shader
	deviceContext->VSSetShader(m_VertexShader, NULL, 0);
	deviceContext->PSSetShader(m_PixelShader, NULL, 0);

	// set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_SamplerState);

	// render the geomotry
	deviceContext->DrawIndexed(indexCont, 0, 0);

	return true;
}
