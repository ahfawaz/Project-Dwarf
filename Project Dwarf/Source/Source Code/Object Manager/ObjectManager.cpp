#include "../Application/stdafx.h"

//Header Includes
#include "ObjectManager.h"
#include "GameObject.h"
#include "../Components/RenderComponent.h"

//STL Includes

//Namespaces 

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

void CObjectManager::RenderSort()
{
	UINT min = 0;
	CGameObject* temp = NULL;
	CRenderComponent *render_comp1 = NULL, *render_comp2 = NULL;

	for (UINT i = 0; i < m_vRenderList.size(); i++)
	{
		min = i;
		render_comp1 = static_cast<CRenderComponent*>(m_vRenderList[i]->GetComponentByType(eRENDER_COMP));
		for (UINT j = i; j < m_vRenderList.size(); j++)
		{
			render_comp2 = static_cast<CRenderComponent*>(m_vRenderList[j]->GetComponentByType(eRENDER_COMP));

			if (render_comp2->GetRenderType() < render_comp1->GetRenderType())
			{
				min = j;
				render_comp1 = render_comp2;
			}
		}

		temp = m_vRenderList[i];
		m_vRenderList[i] = m_vRenderList[min];
		m_vRenderList[min] = temp;
		temp = NULL;
	}
}


vector<CGameObject*>& CObjectManager::GetRenderList()
{
	return m_vRenderList;
}