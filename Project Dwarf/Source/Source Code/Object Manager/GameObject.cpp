#include "../Application/stdafx.h"

//STL Includes

//Header Includes
#include "GameObject.h"
#include "../Components/Component.h"

//Name Spaces


CGameObject::CGameObject()
{
	XMStoreFloat4x4(&m_fLocalMat, XMMatrixIdentity());
	XMStoreFloat4x4(&m_fWorldMat, XMMatrixIdentity());
}

CGameObject::~CGameObject()
{
	for (size_t i = 0; i < m_vCompList.size(); i++)
	{
		SAFE_SHUTDOWN(m_vCompList[i])
	}
	m_vCompList.clear();
}

void CGameObject::UpdateComponents()
{
	for (size_t i = 0; i < m_vCompList.size(); i++)
	{
		m_vCompList[i]->UpdateComponent();
	}
}


CComponent* CGameObject::GetMainComponent() const
{
	if (m_vCompList[MAIN_COMP]->GetCompType() != eMAIN_COMP)
		return nullptr;

	return m_vCompList[MAIN_COMP];

}

CComponent* CGameObject::GetComponentByType(eCompTypes _type)
{
	for (size_t i = 0; i < m_vCompList.size(); i++)
	{
		if (m_vCompList[i]->GetCompType() == _type)
			return m_vCompList[i];
	}

	return nullptr;
}


XMFLOAT4X4& CGameObject::GetLocalMat()
{
	return m_fLocalMat;
}

XMFLOAT4X4& CGameObject::GetWorldMat()
{
	return m_fWorldMat;
}

string& CGameObject::GetTag()
{
	return m_sTag;
}

eObjTypes& CGameObject::GetObjectType()
{
	return m_eType;
}

bool CGameObject::IsActive() const
{
	return m_bActive;
}
void CGameObject::SetActive(bool _active)
{
	m_bActive = _active;
}

//Helper Funcs
bool CGameObject::AddComponent(CComponent * _comp)
{
	if (!_comp)
		return false;

	_comp->SetCompIndex((UINT)m_vCompList.size());
	m_vCompList.push_back(_comp);

	return true;
}