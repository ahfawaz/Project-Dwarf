#include "../Application/stdafx.h"

#include "RenderController.h"

//STL includes
#include <DirectXColors.h>

//Header incldes
#include "RenderMain.h"
#include "../Core/Core.h"
#include "../Object Manager/GameObject.h"
#include "../Object Manager/ObjectManager.h"
#include "../Components/RenderComponent.h"
#include "../Asset Manager/Mesh.h"
#include "ConstantBufferManager.h"
#include "ContextManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "Shaders\VertexBuffers.h"
#include "DepthStencilStateManager.h"
#include "../Utilities/Util.h"

//Namespaces used

//Definitions used.

CRenderController::CRenderController()
{
}

CRenderController::~CRenderController()
{
}

bool CRenderController::Initialize(CRenderer* const _renderer, Core* _core)
{
	HRESULT hr = S_OK;
	if (_renderer == nullptr || _core == nullptr)
		return false;

	_pRenderer = _renderer;

	//Set ViewPort
	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
	m_Viewport.Width = (FLOAT)_core->GetWindowWidth();
	m_Viewport.Height = (FLOAT)_core->GetWindowHeight();
	m_Viewport.MaxDepth = MAX_DEPTH;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;

	hr = CreateSwapChainandDevice(_core->GetWindow(), _core->IsWindowed());
	if (hr != S_OK)
		return false;

	hr = CreateRenderTarget();
	if (hr != S_OK)
		return false;

	hr = CreateDepthBuffer();
	if (hr != S_OK)
		return false;

	_pCore = _core;

	m_pContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
	m_pContext->RSSetViewports(1, &m_Viewport);

	SetD3DName(m_pDSV, "Depth Stencil View");
	SetD3DName(m_pDepthBuffer, "Depth Buffer");
	SetD3DName(m_pBackBuffer, "Back Buffer");
	SetD3DName(m_pRTV, "Render Target View");
	SetD3DName(m_pContext, "Device Context");
	SetD3DName(m_pSwapChain, "Swap Chain");
	SetD3DName(m_pDevice, "The Device");

	
	//m_pDevice->SetPrivateData(, , "The Device");

	

	return true;
}

void CRenderController::Shutdown()
{
	SAFE_RELEASE(m_pDSV);
	SAFE_RELEASE(m_pDepthBuffer);
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);

	_pCore = nullptr;
	_pRenderer = nullptr;
}

//Accessors
ID3D11Device* CRenderController::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext* CRenderController::GetContext() const
{
	return m_pContext;
}

float CRenderController::GetWindowWidth() const
{
	return float(m_Viewport.Width);
}

float CRenderController::GetWindowHeight() const
{
	return float(m_Viewport.Height);
}

//Mutators

#pragma region Private Functions
//Private Functions
HRESULT CRenderController::CreateSwapChainandDevice(HWND _hWnd, bool _windowed)
{
	UINT mode_flag;

	DXGI_SWAP_CHAIN_DESC swapchain_DESC;
	ZeroMemory(&swapchain_DESC, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchain_DESC.BufferCount = 1;
	swapchain_DESC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_DESC.BufferDesc.Width = (UINT)m_Viewport.Width;
	swapchain_DESC.BufferDesc.Height = (UINT)m_Viewport.Height;
	swapchain_DESC.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_DESC.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_DESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_DESC.OutputWindow = _hWnd;
	swapchain_DESC.SampleDesc.Count = 1;
	swapchain_DESC.SampleDesc.Quality = 0;
	swapchain_DESC.Windowed = _windowed;
	swapchain_DESC.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL lvl[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	UINT numFeature = ARRAYSIZE(lvl);
	D3D_FEATURE_LEVEL suppLVL;

#if _DEBUG
	mode_flag = D3D11_CREATE_DEVICE_DEBUG;
#elif MULTITHREADED == 1
	mode_flag = D3D11_CREATE_DEVICE_SINGLETHREADED;
#else
	mode_flag = NULL;
#endif

	return D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		mode_flag,
		lvl,
		numFeature,
		D3D11_SDK_VERSION,
		&swapchain_DESC,
		&m_pSwapChain,
		&m_pDevice,
		&suppLVL,
		&m_pContext
		);


}

HRESULT CRenderController::CreateRenderTarget()
{
	HRESULT hr = S_OK;

	hr = m_pSwapChain->GetBuffer(0, __uuidof(m_pBackBuffer), reinterpret_cast<void**>(&m_pBackBuffer));

	if (hr != S_OK)
		return hr;

	hr = m_pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pRTV);

	return hr;

}

HRESULT CRenderController::CreateDepthBuffer()
{
	HRESULT hr = S_OK;

	//D3D11_TEXTURE2D_DESC zBufferDESC;
	//ZeroMemory(&zBufferDESC, sizeof(zBufferDESC));
	//zBufferDESC.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//zBufferDESC.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//zBufferDESC.Width = (UINT)m_Viewport.Width;
	//zBufferDESC.Height = (UINT)m_Viewport.Height;
	//zBufferDESC.MipLevels = 1;
	//zBufferDESC.ArraySize = 1;
	//zBufferDESC.SampleDesc.Count = 1;
	//zBufferDESC.SampleDesc.Quality = 0;
	//zBufferDESC.Usage = D3D11_USAGE_DEFAULT;

	D3D11_TEXTURE2D_DESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.Width = (UINT)m_Viewport.Width;
	tDesc.Height = (UINT)m_Viewport.Height;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	DXGI_SWAP_CHAIN_DESC TempSwapDesc;
	m_pSwapChain->GetDesc(&TempSwapDesc);
	tDesc.SampleDesc = TempSwapDesc.SampleDesc;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D(&tDesc, NULL, &m_pDepthBuffer);
	if (hr != S_OK)
		return hr;
	///////////////////////////////////////////
	//Snippet
	//Author: Daniel Stover
	///////////////////////////////////////////
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
#if USING_MSAA == 1
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
#else 
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
#endif
	descDSV.Texture2D.MipSlice = 0;

	///////////////////////////////////////////////
	//End Snippet
	///////////////////////////////////////////////

	//Create the Depth Stencil View
	hr = m_pDevice->CreateDepthStencilView(m_pDepthBuffer, &descDSV, &m_pDSV);

	return hr;
}

#pragma endregion

//Function Calls
void CRenderController::ClearScene()
{
	//m_pContext->ClearState();

	m_pContext->ClearRenderTargetView(m_pRTV, Colors::Black);

	m_pContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, MAX_DEPTH, NULL);
}

void CRenderController::DrawScene()
{
	CRenderer& renderer = *_pRenderer;
	Core& core = *_pCore;

	renderer.GetConstantBuffers()->UpdateViewAndProjBuffer(renderer.GetCameraManager()->GetViewMatrix(), renderer.GetCameraManager()->GetProjMatrix());

	vector<CGameObject*> render_list = core.GetObjectManager()->GetRenderList();

	CGameObject* render_obj = nullptr;
	CRenderComponent* render_comp = nullptr;

	for (UINT i = 0; i < render_list.size(); i++)
	{
		render_obj = render_list[i];
		render_comp = static_cast<CRenderComponent*>(render_obj->GetComponentByType(eCompTypes::eRENDER_COMP));

		DrawObject(render_obj, render_comp);		

	}
}

void CRenderController::PresentScene()
{
	m_pSwapChain->Present(0, 0);
}

void CRenderController::DrawObject(CGameObject * _obj, CRenderComponent * _comp)
{
	if (!_obj || !_comp)
		return;

	static UINT current_type = UINT_MAX;
	if (current_type != _comp->GetRenderType())
	{
		current_type = _comp->GetRenderType();
		SetDepthStencilState(current_type);
	}


	CRenderer& renderer = *_pRenderer;

	//Bind Shaders
	renderer.GetShaderManager()->BindShaders(m_pContext, _comp);

	//Set Input Layout
	renderer.GetShaderManager()->SetInputLayout(m_pContext, _comp);

	//Update and Bind Constant Buffers
	renderer.GetConstantBuffers()->UpdateConstBuffers(_obj, _comp);
	renderer.GetConstantBuffers()->BindConstBuffer(_comp);

	//Set vertex buffer info
	UINT stride = GetStrideSize(_comp->GetRenderType()), zero = NULL;
	ID3D11Buffer* mesh_data = _comp->GetMesh()->GetVertexBuffer();
	m_pContext->IASetVertexBuffers(0, 1, &mesh_data, &stride, &zero);

	//set primitive topology
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set Index buffer
	m_pContext->IASetIndexBuffer(_comp->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, NULL);

	//draw the obj based on whether it's a batch draw or not.
	///Assume it is not batch draw right now
	m_pContext->DrawIndexed(_comp->GetMesh()->GetNumIndicies(), NULL, NULL);

}

UINT CRenderController::GetStrideSize(UINT _flag)
{
	UINT stride_size = 0;

	switch (_flag)
	{
	case eNONE:
		break;
	case eDEF_RENDER:
	case eBATCH_RENDER:
		stride_size = sizeof(v_StaticNoTangs);
		break;
	case eANIM_RENDER:
		stride_size = sizeof(v_AnimNoTangents);
		break;
	case eTANGENT_RENDER:
	case eBATCH_W_TANGENTS:
		stride_size = sizeof(v_StaticWithTangs);
		break;
	case eANIM_W_TANGENTS:
		stride_size = sizeof(v_AnimNoTangents);
		break;
	default:
		break;
	}

	return stride_size;
}

void CRenderController::SetDepthStencilState(UINT _render_type)
{
	switch (_render_type)
	{
	case eNONE:
		break;
	case eDEF_RENDER:
	case eBATCH_RENDER:
	case eANIM_RENDER:
	case eTANGENT_RENDER:
	case eBATCH_W_TANGENTS:
	case eANIM_W_TANGENTS:
		_pRenderer->GetDPSManager()->EnableDepthStencilState(eDSS_DEFAULT, m_pContext);
		break;
	default:
		break;
	}
}
