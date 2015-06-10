#include "../Application/stdafx.h"

#include "ObjectManager.h"
#include "GameObject.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
}

bool CObjectManager::Initialize()
{
	return true;
}

void CObjectManager::Shutdown()
{
	for (UINT i = 0; i < m_vObjectList.size(); i++)
	{
		SAFE_DELETE(m_vObjectList[i]);
	}
}

void CObjectManager::Update()
{
	for (UINT i = 0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->UpdateComponents();
	}
}

bool CObjectManager::AddObject(CGameObject* _obj)
{
	bool bObjGood = true;

	for (UINT i = 0; i < eCompTypes::eNUM_OF_COMP; i++)
	{
		switch (eCompTypes(i))
		{
		case eDEFAULT:
			bObjGood = false;
			break;
		case eMAIN_COMP:
			break;
		case eRENDER_COMP:
			if (_obj->GetComponentByType(eRENDER_COMP))
				m_vRenderList.push_back(_obj);
			break;
		default:
			break;
		}
	}

	if (!bObjGood)
		_obj->SetActive(false);
	
	m_vObjectList.push_back(_obj);

	return bObjGood;
}

vector<CGameObject*>& CObjectManager::GetRenderList()
{
	return m_vRenderList;
}