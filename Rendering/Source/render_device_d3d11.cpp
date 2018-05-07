#include "render_device_d3d11.h"

#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "vertex_buffer_d3d11.h"
#include "index_buffer_d3d11.h"
#include <D3Dcompiler.h>
#include <DirectXMath.h>

namespace Ming3D
{
	RenderDeviceD3D11* GRenderDeviceD3D11 = nullptr;

	RenderDeviceD3D11::RenderDeviceD3D11()
	{
		__AssertComment(GRenderDeviceD3D11 == nullptr, "Can only have one GRenderDeviceD3D11");
		GRenderDeviceD3D11 = this;

		D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &mDevice, NULL, &mDeviceContext);

		IDXGIDevice * dxgiDevice = 0;
		HRESULT hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)& dxgiDevice);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR() << "Failed to create IDXGIDevice";
			return;
		}

		IDXGIAdapter * dxgiAdapter = 0;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)& dxgiAdapter);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR() << "Failed to create IDXGIAdapter";
			return;
		}

		mDXGIFactory = 0;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)& mDXGIFactory);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR() << "Failed to create IDXGIFactory";
			return;
		}
	}

	RenderDeviceD3D11::~RenderDeviceD3D11()
	{
		mDevice->Release();
		mDeviceContext->Release();
	}

	size_t RenderDeviceD3D11::GetShaderUniformSize(const ShaderUniformInfo& inShaderUniform)
	{
		switch (inShaderUniform.mType)
		{
		case ShaderVariableType::Float:
			return sizeof(float);
			break;
		case ShaderVariableType::Int:
			return sizeof(int);
			break;
		case ShaderVariableType::Mat4x4:
			return sizeof(DirectX::XMFLOAT4X4);
			break;
		case ShaderVariableType::Vec4:
			return sizeof(DirectX::XMFLOAT4);
			break;
		default:
			__AssertComment(false, "Unhandled shader uniform type");
			return 0;
		}
	}

	RenderTarget* RenderDeviceD3D11::CreateRenderTarget(WindowBase* inWindow)
	{
		return new RenderTargetD3D11(inWindow);
	}

	VertexBuffer* RenderDeviceD3D11::CreateVertexBuffer(VertexData* inVertexData)
	{
		VertexBufferD3D11* vertexBuffer = new VertexBufferD3D11();
		ID3D11Buffer* vBuffer;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = inVertexData->GetVertexSize() * inVertexData->GetNumVertices();																		
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mDevice->CreateBuffer(&vertexBufferDesc, NULL, &vBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		mDeviceContext->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

		memcpy(ms.pData, inVertexData->GetDataPtr(), inVertexData->GetVertexSize() * inVertexData->GetNumVertices());
		
		mDeviceContext->Unmap(vBuffer, NULL);

		vertexBuffer->SetD3DBuffer(vBuffer);
		vertexBuffer->SetVertexLayout(inVertexData->GetVertexLayout());
		return vertexBuffer;
	}

	IndexBuffer* RenderDeviceD3D11::CreateIndexBuffer(IndexData* inIndexData)
	{
		IndexBufferD3D11* indexBuffer = new IndexBufferD3D11();
		ID3D11Buffer* iBuffer;

		D3D11_BUFFER_DESC indexBufferDesc;
		
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * inIndexData->GetNumIndices();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		
		indexData.pSysMem = inIndexData->GetData();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		HRESULT result = mDevice->CreateBuffer(&indexBufferDesc, &indexData, &iBuffer);
		if (FAILED(result))
		{
			LOG_ERROR() << "Failed to create index buffer";
			return nullptr;
		}
		indexBuffer->SetD3DBuffer(iBuffer);
		indexBuffer->SetNumIndices(inIndexData->GetNumIndices());
		return indexBuffer;
	}

	ShaderProgram* RenderDeviceD3D11::CreateShaderProgram(const std::string& inVertexShaderCode, const std::string& inFragmentShaderCode, const ShaderProgramConstructionInfo& inConstructionInfo)
	{
		ID3D10Blob* vsBlob;
		ID3D10Blob* psBlob;
		ID3D11VertexShader* pVS;
		ID3D11PixelShader* pPS;
		ID3D11InputLayout* inputLayout;

		ID3DBlob* errorBlob;
		D3DCompile(inVertexShaderCode.data(), inVertexShaderCode.size(), "", NULL, NULL, "main", "vs_4_0", 0, NULL, &vsBlob, &errorBlob);
		D3DCompile(inFragmentShaderCode.data(), inFragmentShaderCode.size(), "", NULL, NULL, "main", "ps_4_0", 0, NULL, &psBlob, &errorBlob);

		HRESULT vsRes = mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &pVS);
		HRESULT psRes = mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pPS);

		mDeviceContext->VSSetShader(pVS, 0, 0);
		mDeviceContext->PSSetShader(pPS, 0, 0);

        const char* vertCompNames[] = { "POSITION", "NORMAL", "TEXCOORD", "COLOUR" }; // TEMP - TODO
        const DXGI_FORMAT vertFormats[] = { DXGI_FORMAT_R32G32B32_FLOAT , DXGI_FORMAT_R32G32B32_FLOAT , DXGI_FORMAT_R32G32_FLOAT , DXGI_FORMAT_R32G32B32A32_FLOAT }; // TODO
		UINT byteOffset = 0;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
		for (const EVertexComponent& vertexComp : inConstructionInfo.mVertexLayout.VertexComponents)
		{
			D3D11_INPUT_ELEMENT_DESC desc = { vertCompNames[vertexComp], 0, vertFormats[vertexComp], 0, byteOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			inputElements.push_back(desc);
            byteOffset += VertexData::GetVertexComponentSize(vertexComp);
		}

		mDevice->CreateInputLayout(inputElements.data(), inputElements.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);

		std::unordered_map<std::string, ShaderConstantD3D11> shaderConstantMap;

		size_t currentUniformOffset = 0;
		for (const ShaderUniformInfo& uniformInfo : inConstructionInfo.mUniforms)
		{
			const size_t currentUniformSize = GetShaderUniformSize(uniformInfo);
			shaderConstantMap.emplace(uniformInfo.mName, ShaderConstantD3D11(currentUniformOffset, currentUniformSize));
			currentUniformOffset += currentUniformSize;
		}

		ID3D11Buffer*   constantBuffer = NULL;

		struct VS_CONSTANT_BUFFER
		{
			DirectX::XMFLOAT4X4 MVP;
			DirectX::XMFLOAT4 test;
		};

		VS_CONSTANT_BUFFER* VsConstData = new VS_CONSTANT_BUFFER();
		//VsConstData->test = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // TEST

		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresData;
		subresData.pSysMem = VsConstData;
		subresData.SysMemPitch = 0;
		subresData.SysMemSlicePitch = 0;

		HRESULT hr = mDevice->CreateBuffer(&cbDesc, &subresData, &constantBuffer);

		if (FAILED(hr))
		{
			LOG_ERROR() << "Failed to create constant buffer";
			return nullptr;
		}

		mDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);



		ShaderProgramD3D11* shaderProgram = new ShaderProgramD3D11();
		shaderProgram->mInputLayout = inputLayout;
		shaderProgram->mVS = pVS;
		shaderProgram->mPS = pPS;
		shaderProgram->mConstantBuffer = constantBuffer;
		shaderProgram->mConstantData = VsConstData;
		shaderProgram->mShaderConstantMap = shaderConstantMap;
		shaderProgram->mShaderConstantsSize = currentUniformOffset;
		return shaderProgram;
	}

    void RenderDeviceD3D11::SetTexture(Texture* inTexture)
    {

    }

	void RenderDeviceD3D11::SetRenderTarget(RenderTarget* inTarget)
	{
		mRenderTarget = (RenderTargetD3D11*)inTarget;
	}

	void RenderDeviceD3D11::SetActiveShaderProgram(ShaderProgram* inProgram)
	{
		ShaderProgramD3D11* dxShaderProgram = (ShaderProgramD3D11*)inProgram;
		mActiveShaderProgram = dxShaderProgram;
		mDeviceContext->VSSetShader(dxShaderProgram->mVS, 0, 0);
		mDeviceContext->PSSetShader(dxShaderProgram->mPS, 0, 0);
	}

	void RenderDeviceD3D11::BeginRendering()
	{
		__Assert(mRenderTarget != nullptr);

		mDeviceContext->ClearRenderTargetView(mRenderTarget->GetBackBuffer(), D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	}

	void RenderDeviceD3D11::EndRendering()
	{
		__Assert(mRenderTarget != nullptr);

		mRenderTarget->GetSwapChain()->Present(0, 0);
	}

	void RenderDeviceD3D11::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer)
	{
		VertexBufferD3D11* vertexBufferDX = (VertexBufferD3D11*)inVertexBuffer;
		IndexBufferD3D11* indexBufferDX = (IndexBufferD3D11*)inIndexBuffer;

		ID3D11Buffer* vb = vertexBufferDX->GetD3DBuffer();

		mDeviceContext->IASetInputLayout(mActiveShaderProgram->mInputLayout);

		UINT stride = inVertexBuffer->GetVertexSize();
		UINT offset = 0;
		mDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		mDeviceContext->IASetIndexBuffer(indexBufferDX->GetD3DBuffer(), DXGI_FORMAT_R32_UINT, 0);
		
		mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		mDeviceContext->DrawIndexed(inIndexBuffer->GetNumIndices(), 0, 0);
	}

	void RenderDeviceD3D11::SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat)
	{
		__Assert(mActiveShaderProgram != nullptr);

		auto shaderConstIter = mActiveShaderProgram->mShaderConstantMap.find(inName);
		if (shaderConstIter == mActiveShaderProgram->mShaderConstantMap.end())
		{
			LOG_ERROR() << "Could not find shader constant: " << inName;
			return;
		}
		const ShaderConstantD3D11 shaderConst = shaderConstIter->second;

		void* shaderConstData = mActiveShaderProgram->mConstantData;

		glm::mat4 mat = glm::transpose(inMat);
		*((DirectX::XMFLOAT4X4*)((char*)shaderConstData + shaderConst.mOffset)) = DirectX::XMFLOAT4X4(&mat[0][0]);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		mDeviceContext->Map(mActiveShaderProgram->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy(mappedResource.pData, shaderConstData, mActiveShaderProgram->mShaderConstantsSize);

		mDeviceContext->Unmap(mActiveShaderProgram->mConstantBuffer, 0);
	}

	void RenderDeviceD3D11::SetShaderUniformVec4(const char* inName, const glm::vec4 inVec)
	{
		__Assert(mActiveShaderProgram != nullptr);

		auto shaderConstIter = mActiveShaderProgram->mShaderConstantMap.find(inName);
		if (shaderConstIter == mActiveShaderProgram->mShaderConstantMap.end())
		{
			LOG_ERROR() << "Could not find shader constant: " << inName;
			return;
		}
		const ShaderConstantD3D11 shaderConst = shaderConstIter->second;

		void* shaderConstData = mActiveShaderProgram->mConstantData;

		*((DirectX::XMFLOAT4*)((char*)shaderConstData + shaderConst.mOffset)) = DirectX::XMFLOAT4(&inVec[0]);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		mDeviceContext->Map(mActiveShaderProgram->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy(mappedResource.pData, shaderConstData, mActiveShaderProgram->mShaderConstantsSize);

		mDeviceContext->Unmap(mActiveShaderProgram->mConstantBuffer, 0);
	}

}
