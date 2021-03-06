#include "../Application/stdafx.h"

#include "ShaderManager.h"

//STL includes

//Header includes
#include "../Core/Core.h"
#include "../Components/RenderComponent.h"
#include "../Utilities/Util.h"

//Shader Includes
///Vertex Shaders
#include "Shaders\Vertex Shaders\EmptyVertex.csh"
#include "Shaders\Vertex Shaders\StaticVertsUsingNoTangents.csh"
#include "Shaders\Vertex Shaders\StaticVertUsingTangents.csh"
#include "Shaders\Vertex Shaders\StaticBatchVertsNoTangents.csh"

///Geometry Shaders

///Pixel Shaders
#include "Shaders/Pixel Shaders/BasicPixelShader.csh"
#include "Shaders/Pixel Shaders/TexturePixelShader.csh"

//Namespaces used

//Definitions & Globals

CShaderManager::CShaderManager() : m_ShaderFlag(0), m_LayoutFlag(0)
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Initialize(ID3D11Device* _device)
{
	HRESULT hr = S_OK;

	//Vertex Shader Initializations
	hr = LoadVertexShaders(_device);
	if (FAILED(hr))
		return false;

	//Geometry Shader Initializations
	hr = LoadGeometryShaders(_device);
	if (FAILED(hr))
		return false;

	//Pixel Shader Initializations
	hr = LoadPixelShaders(_device);
	if (FAILED(hr))
		return false;

	//Input Layout Initializations
	hr = LoadInputLayouts(_device);
	if (FAILED(hr))
		return false;

	return true;
}

void CShaderManager::Shutdown()
{
	//Shutdown of Vertex Shaders
	SAFE_RELEASE(m_vsEmptyVertex);
	SAFE_RELEASE(m_vsStaticNoTangents);
	SAFE_RELEASE(m_vsStaticWithTangents);
	SAFE_RELEASE(m_vsBatchNoTangents)

	//Shutdown of Geometry Shaders

	//Shutdown of Pixel Shaders
	SAFE_RELEASE(m_psBasic);
	SAFE_RELEASE(m_psTexture);

	//shutdown of Input Layouts
	SAFE_RELEASE(m_ilStaticNoTangents);
	SAFE_RELEASE(m_ilStaticWithTangents);
}

//Private Helper Functions
HRESULT CShaderManager::LoadVertexShaders(ID3D11Device* _device)
{
	HRESULT hr = S_OK;

	hr = _device->CreateVertexShader(EmptyVertex, sizeof(EmptyVertex), NULL, &m_vsEmptyVertex);
	if (hr != S_OK)
		return hr;

	hr = _device->CreateVertexShader(StaticVertsUsingNoTangents, sizeof(StaticVertsUsingNoTangents), NULL, &m_vsStaticNoTangents);
	if (hr != S_OK)
		return hr;

	hr = _device->CreateVertexShader(StaticVertUsingTangents, sizeof(StaticVertUsingTangents), NULL, &m_vsStaticWithTangents);
	if (hr != S_OK)
		return hr;

	hr = _device->CreateVertexShader(StaticBatchVertsNoTangents, sizeof(StaticBatchVertsNoTangents), NULL, &m_vsBatchNoTangents);
	if (hr != S_OK)
		return hr;

	SetD3DName(m_vsEmptyVertex, "Empty Vertex Shader");
	SetD3DName(m_vsStaticNoTangents, "Static No Tangent Vertex Shader");
	SetD3DName(m_vsStaticWithTangents, "Static Tangent Vertex Shader");
	SetD3DName(m_vsBatchNoTangents, "Static Batch Verts No Tangents");

	return hr;
}

HRESULT CShaderManager::LoadGeometryShaders(ID3D11Device* _device)
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CShaderManager::LoadPixelShaders(ID3D11Device* _device)

{
	HRESULT hr = S_OK;

	hr = _device->CreatePixelShader(BasicPixelShader, sizeof(BasicPixelShader), NULL, &m_psBasic);
	if (hr != S_OK)
		return hr;

	hr = _device->CreatePixelShader(TexturePixelShader, sizeof(TexturePixelShader), NULL, &m_psTexture);
	if (hr != S_OK)
		return hr;

	SetD3DName(m_psBasic, "Basic Pixel Shader");
	SetD3DName(m_psTexture, "Texture Pixel Shader");

	return hr;
}

HRESULT CShaderManager::LoadInputLayouts(ID3D11Device* _device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC static_noTangents[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = _device->CreateInputLayout
		(
		static_noTangents,
		ARRAYSIZE(static_noTangents),
		StaticVertsUsingNoTangents,
		sizeof(StaticVertsUsingNoTangents),
		&m_ilStaticNoTangents
		);

	if (hr != S_OK)
		return hr;

	D3D11_INPUT_ELEMENT_DESC static_Tangents[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = _device->CreateInputLayout
		(
		static_Tangents,
		ARRAYSIZE(static_Tangents),
		StaticVertUsingTangents,
		sizeof(StaticVertUsingTangents),
		&m_ilStaticWithTangents
		);

	if (hr != S_OK)
		return hr;

	SetD3DName(m_ilStaticNoTangents, "Input Layout No Tangents");
	SetD3DName(m_ilStaticWithTangents, "Input Layout Tangents");

	return hr;
}

void CShaderManager::BindShaders(ID3D11DeviceContext* _context, CRenderComponent* _comp)
{
	UINT flag = _comp->GetRenderType();
	if (flag == m_ShaderFlag)
		return;
	else
		m_ShaderFlag = flag;

	switch (flag)
	{
	case eNONE:
		break;
	case eDEF_RENDER:
		_context->VSSetShader(m_vsStaticNoTangents, NULL, NULL);
		_context->PSSetShader(m_psBasic, NULL, NULL);
		break;
	case eBATCH_RENDER:
		_context->VSSetShader(m_vsBatchNoTangents, NULL, NULL);
		_context->PSSetShader(m_psBasic, NULL, NULL);
		break;
	case eANIM_RENDER:
		break;
	case eTANGENT_RENDER:
		break;
	case eBATCH_W_TANGENTS:
		break;
	case eANIM_W_TANGENTS:
		break;
	default:
		break;
	}
}

void CShaderManager::SetInputLayout(ID3D11DeviceContext* _context, CRenderComponent* _comp)
{
	UINT flag = _comp->GetRenderType();
	if (flag == m_LayoutFlag)
		return;
	else
		m_LayoutFlag = flag;

	switch (flag)
	{
	case eNONE:
		break;
	case eDEF_RENDER:
		_context->IASetInputLayout(m_ilStaticNoTangents);
		break;
	case eBATCH_RENDER:
		_context->IASetInputLayout(m_ilStaticNoTangents);
		break;
	case eANIM_RENDER:
		break;
	case eTANGENT_RENDER:
		_context->IASetInputLayout(m_ilStaticWithTangents);
		break;
	case eBATCH_W_TANGENTS:
		_context->IASetInputLayout(m_ilStaticWithTangents);
		break;
	case eANIM_W_TANGENTS:
		break;
	default:
		break;
	}
}
