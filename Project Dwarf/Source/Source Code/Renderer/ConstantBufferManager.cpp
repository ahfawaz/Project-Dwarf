#include "../Application/stdafx.h"

#include "ConstantBufferManager.h"

//STL Includes 

//Header Includes

#include "Shaders/BufferDefines.h"
#include "Shaders/ConstantBuffers.h"
#include "ContextManager.h"
#include "../Object Manager/GameObject.h"
#include "../Components/RenderComponent.h"
#include "../Utilities/Util.h"

//Namespaces Used

CConstantBufferManager::CConstantBufferManager()
{

}

CConstantBufferManager::~CConstantBufferManager()
{

}


bool CConstantBufferManager::Initialize(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	if (!_device || !_context)
		return false;

	_pContext = _context;

	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC const_desc;
	ZeroMemory(&const_desc, sizeof(D3D11_BUFFER_DESC));
	const_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	const_desc.Usage = D3D11_USAGE_DYNAMIC;
	const_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//Create each needed Buffer

	//World View Data
	const_desc.ByteWidth = sizeof(cb_WorldView);
	hr = _device->CreateBuffer(&const_desc, NULL, &m_pWorldView);
	if (hr != S_OK)
		return false;

	//World Position Data
	const_desc.ByteWidth = sizeof(cb_WorldView);
	hr = _device->CreateBuffer(&const_desc, NULL, &m_pWorldPos);
	if (hr != S_OK)
		return false;

	//World Position Data
	const_desc.ByteWidth = sizeof(cb_BatchPos);
	hr = _device->CreateBuffer(&const_desc, NULL, &m_pBatchPos);
	if (hr != S_OK)
		return false;
	
	SetD3DName(m_pWorldView, "World View");
	SetD3DName(m_pWorldPos, "World Pos");
	SetD3DName(m_pBatchPos, "Batch Buffer");

	return true;
}

void CConstantBufferManager::Shutdown()
{
	SAFE_RELEASE(m_pWorldPos);
	SAFE_RELEASE(m_pWorldView);
	SAFE_RELEASE(m_pBatchPos);

	_pContext = nullptr;
}

void CConstantBufferManager::UpdateConstBuffers(CGameObject* _obj, CRenderComponent* _comp)
{
	UINT flag = _comp->GetRenderType();

	switch (flag)
	{
	case eNONE:
		break;
	case eDEF_RENDER:
		BufferUpdate(&m_pWorldPos, _obj->GetWorldMat());
		break;
	case eBATCH_RENDER:
		BufferUpdate(&m_pBatchPos, _comp->GetBatchPositions()[0], sizeof(XMFLOAT4X4) * (UINT)_comp->GetBatchPositions().size());
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

void CConstantBufferManager::UpdateViewAndProjBuffer(XMFLOAT4X4 _view, XMFLOAT4X4 _proj)
{
	cb_WorldView temp = { _view, _proj };
	BufferUpdate(&m_pWorldView, temp);
}

void CConstantBufferManager::BindConstBuffer(CRenderComponent* _comp) const
{

	UINT flag = _comp->GetRenderType();

	switch (flag)
	{
	case eNONE:
		break;
	case eDEF_RENDER:
		_pContext->VSSetConstantBuffers(0, 1, &m_pWorldView);
		_pContext->VSSetConstantBuffers(1, 1, &m_pWorldPos);
		break;
	case eBATCH_RENDER:
		_pContext->VSSetConstantBuffers(0, 1, &m_pWorldView);
		_pContext->VSSetConstantBuffers(2, 1, &m_pBatchPos);
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


template <class TYPE>
void CConstantBufferManager::BufferUpdate(ID3D11Buffer ** _buffer, TYPE & _data)
{
	D3D11_MAPPED_SUBRESOURCE map_resource;
	_pContext->Map(*_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map_resource);
	memcpy_s(map_resource.pData, sizeof(TYPE), &_data, sizeof(TYPE));
	_pContext->Unmap(*_buffer, 0);
}

template <class TYPE>
void CConstantBufferManager::BufferUpdate(ID3D11Buffer ** _buffer, TYPE & _data, UINT _data_size)
{
	D3D11_MAPPED_SUBRESOURCE map_resource;
	_pContext->Map(*_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map_resource);
	memcpy_s(map_resource.pData, _data_size, &_data, sizeof(TYPE));
	_pContext->Unmap(*_buffer, 0);
}
