#include "../Application/stdafx.h"

#include "DepthStencilStateManager.h"
#include "../Utilities/Util.h"

#if _DEBUG
string debug_name;
#define SET_DEBUG_NAME(x) debug_name = x;
#define GET_DEBUG_NAME debug_name.c_str()
#else
#define SET_DEBUG_NAME
#define GET_DEBUG_NAME
#endif


CDepthStencilStateManager::CDepthStencilStateManager()
{

}

CDepthStencilStateManager::~CDepthStencilStateManager()
{

}

bool CDepthStencilStateManager::Initialize(ID3D11Device* _device)
{
	bool passed = true;
	HRESULT hr = S_OK;

	CD3D11_DEPTH_STENCIL_DESC DSS_DESC;
	DSS_DESC.DepthEnable = true;
	DSS_DESC.StencilEnable = false;
	ID3D11DepthStencilState* pDSS = nullptr;

	for (UINT i = 0; i < eDSS_SIZE; i++)
	{
		eDepthStencilStates state = eDepthStencilStates(i);

		switch (state)
		{
		case eDSS_DEFAULT:
			DSS_DESC.DepthEnable = true;
			DSS_DESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			DSS_DESC.DepthFunc = D3D11_COMPARISON_LESS;
			DSS_DESC.StencilEnable = true;
			DSS_DESC.StencilReadMask = 0xFF;
			DSS_DESC.StencilWriteMask = 0xFF;
			DSS_DESC.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DSS_DESC.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			DSS_DESC.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DSS_DESC.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			DSS_DESC.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DSS_DESC.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			DSS_DESC.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DSS_DESC.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			SET_DEBUG_NAME("DSS Default");
			break;
		case eDSS_SIZE:
		default:
			break;
		}

		hr = _device->CreateDepthStencilState(&DSS_DESC, &pDSS);

		if (hr != S_OK)
		{
			SAFE_RELEASE(pDSS);
			return false;
		}

		SetD3DName(pDSS, GET_DEBUG_NAME);
		
		m_pDSSList.push_back(pDSS);
		pDSS = nullptr;

	}
	return true;
}

void CDepthStencilStateManager::Shutdown()
{
	for (UINT i = 0; i < m_pDSSList.size(); i++)
	{
		SAFE_RELEASE(m_pDSSList[i]);
	}
	m_pDSSList;
}

bool CDepthStencilStateManager::EnableDepthStencilState(eDepthStencilStates _state, ID3D11DeviceContext* _context)
{
	switch (_state)
	{
	case eDSS_DEFAULT:
		_context->OMSetDepthStencilState(m_pDSSList[eDSS_DEFAULT], 1);
		break;
	case eDSS_SIZE:
	default:
		//printf("Incorrect Depth Stencil State");
		return false;
	}

	return true;
}
