#include "colorshaderclass.h"



ColorShaderClass::ColorShaderClass()
{
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_Layout = 0;
	m_MatrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{

}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device * device, HWND hWnd)
{
	bool result;

	// initialize the vertex and pixel shader
	result = InitializeShader(device, hWnd, "color.vs", "color.ps");
	if (!result)
		return false;

	return true;
}

void ColorShaderClass::ShutDown()
{
	// shut down the vertex and pixel shaders
	ShutDownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	bool result;

	// set the shader parameters that it will be use for rendering
	result = SetShaderParameters(deviceContext, world, view, proj);
	if (!result)
		return false;

	// now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device * device, HWND hWnd, LPCSTR vs, LPCSTR ps)
{
	HRESULT result;
	ID3D10Blob* errorMessage = 0;
	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* pixelShaderBuffer = 0;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// compile the vertex shader code
	result = D3DX11CompileFromFile(vs, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		// if the shader failed to compile it should have message to the error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd, vs);
		}
		// if there was nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hWnd, vs, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// compile the pixel shader
	result = D3DX11CompileFromFile(ps, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// if the shader failed to compile it should have message to the error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd, ps);
		}
		// if there was nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hWnd, ps, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// set up the layout of the data that gose into the shader
	// this setup needs to match the VertexType structure in mode class and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	
	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_Layout);
	if (FAILED(result))
	{
		return false;
	}

	// release the vertex shader buffer and pixel shader buffer since they are no longer need
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// set up the dynamic matrix constant buffer
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// create the constant buffer pointer 
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
	if (FAILED(result))
		return false;

	return true;
}

void ColorShaderClass::ShutDownShader()
{
	// release the matrix constant buffer
	if (m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	// release the layout
	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	// release the pixel shader
	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	// release the vertex shader
	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob * errorBlob, HWND hWnd, LPCSTR file)
{
	char* compileErrors;
	unsigned long buffersize;
	std::ofstream fout;

	// get a pointer to the error message text buffer
	compileErrors = (char*)(errorBlob->GetBufferPointer());

	// get the length of the message
	buffersize = errorBlob->GetBufferSize();

	// open a file to write the error message
	fout.open("shader-error.txt");

	// write out the error message
	for (unsigned long i = 0; i < buffersize; i++)
	{
		fout << compileErrors[i];
	}

	// close the file
	fout.close();

	// release the error message
	errorBlob->Release();
	errorBlob = 0;

	// pop message box
	MessageBox(hWnd, "Error compiling shader. Check shader-error.txt for message", file, MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext * deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// transpose the matrices to prepare them for shader
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	// lock the constant buffer so it can be written to
	result = deviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	// get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// copy the matrices into the constant buffer
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// unlock the constant buffer
	deviceContext->Unmap(m_MatrixBuffer, 0);
	
	// set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// finally set the constant buffer in the vertex shader with the upload values
	deviceContext->VSSetConstantBuffers(0, 1, &m_MatrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	// set the vertex shader layout
	deviceContext->IASetInputLayout(m_Layout);

	// set the vertex and pixel shader thats will be used to render this triangle
	deviceContext->VSSetShader(m_VertexShader, NULL, 0);
	deviceContext->PSSetShader(m_PixelShader, NULL, 0);

	// render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
