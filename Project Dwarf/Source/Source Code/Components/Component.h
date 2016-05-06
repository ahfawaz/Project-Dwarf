#pragma once

#include "../Object Manager/GameObject.h"
#include <string>

class CComponent
{
	eCompTypes m_eType;
	UINT m_nCompIndex;
	string m_sCompName;
	CGameObject * m_pOwner;

public:
	CComponent() : m_eType(eCompTypes::eDEFAULT), m_nCompIndex(-1), m_sCompName("Default") {}
	CComponent(eCompTypes _type, int _index = -1) : m_eType(_type), m_nCompIndex(_index) {}
	virtual ~CComponent() {}

	virtual void UpdateComponent() = 0;
	virtual bool Initialize() { return true; }
	virtual void Shutdown() = 0;

	//If Type == eDEFAULT then component is not set properly
	eCompTypes GetCompType() const { return m_eType; }

	//If Index == -1 then component is not set properly
	int	GetCompIndex() const { return m_nCompIndex; }

	string GetCompName() const { return m_sCompName; }
	void SetCompName(string _name) { m_sCompName = _name; }

	//Get the Owner. If nullptr then owner was not set.
	CGameObject* GetOwner() const
	{
		if (!m_pOwner)
			return nullptr;
		return m_pOwner;
	}

	void SetOwner(CGameObject* _owner) { m_pOwner = _owner; }
	void SetCompType(eCompTypes _type) { m_eType = _type; }
	void SetCompIndex(UINT _index) { m_nCompIndex = _index; }
};

