#include "../Application/stdafx.h"

#include "ContextManager.h"

//STL includes

//Header includes
#include "../Object Manager/GameObject.h"
#include "../Components/RenderComponent.h"

//Namespaces used

CContextManager::CContextManager()
{
}

CContextManager::~CContextManager()
{
}


bool CContextManager::Initialize(CRenderer* _data)
{
	return true;
}

void CContextManager::Shutdown()
{

}

//Function Calls
void CContextManager::ContextDrawSwitch(CGameObject* _data)
{
	CRenderComponent* render_comp = static_cast<CRenderComponent*>(_data->GetComponentByType(eRENDER_COMP));
	switch (_data->GetObjectType())
	{
	case eDEF_OBJ:
		break;
	case eTEST_OBJ:
		break;
	default:
		break;
	}
}

//Accessors
ID3D11DeviceContext* CContextManager::GetContext() const
{
	return _pContext;
}
