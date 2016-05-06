#include "../Application/stdafx.h"

//STL Includes

//Header Includes
#include "RenderComponent.h"
#include "../Asset Manager/Mesh.h"
#include "../Object Manager/GameObject.h"
#include "../Core/Core.h"

//Name Spaces

CRenderComponent::CRenderComponent() : CComponent(eRENDER_COMP)
{
	m_pMesh = nullptr;
	m_pDiffuse = nullptr;
	m_pNormals = nullptr;
	m_pSpecular = nullptr;
	m_bBatchDraw = false;
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::UpdateComponent()
{

}

bool CRenderComponent::Initiailze(tModel * _data, tBatchDraw * _batch)
{
	if (!_data)
		return false;

	m_pMesh = _data->pMesh;
	m_pDiffuse = _data->pDiffuse;
	m_pNormals = _data->pNormals;
	m_pSpecular = _data->pSpecular;

	if (!m_pMesh->IsAnimated() && !m_pMesh->IsTangents() && !_batch)
		m_nFlagTypes = eDEF_RENDER;
	else
	{
		if (m_pMesh->IsAnimated())
			m_nFlagTypes |= eANIM_RENDER;
		if (m_pMesh->IsTangents())
			m_nFlagTypes |= eTANGENT_RENDER;
	}

	if (_batch)
	{
		m_vBatchPos = _batch->worldPos;
		m_bBatchDraw = _batch->bDraw;
		m_nFlagTypes |= eBATCH_RENDER;
	}

	return true;
}

void CRenderComponent::Shutdown()
{
	m_pMesh = nullptr;
	m_pDiffuse = nullptr;
	m_pNormals = nullptr;
	m_pSpecular = nullptr;
}

UINT CRenderComponent::GetRenderType() const
{
	return m_nFlagTypes;
}

vector<XMFLOAT4X4>& CRenderComponent::GetBatchPositions()
{
	return m_vBatchPos;
}

CMesh* CRenderComponent::GetMesh() const
{
	return m_pMesh;
}

ID3D11ShaderResourceView* CRenderComponent::GetDiffuse() const
{
	return m_pDiffuse;
}

ID3D11ShaderResourceView* CRenderComponent::GetNormals() const
{
	return m_pNormals;
}

ID3D11ShaderResourceView* CRenderComponent::GetSpecular() const
{
	return m_pSpecular;
}
