#include "../Application/stdafx.h"

#include "Component.h"
//
//CComponent::CComponent() : m_eType(eDEFAULT), m_nCompIndex(-1)
//{
//
//}
//
//CComponent::CComponent(eCompTypes _type, int _index = -1) : m_eType(_type), m_nCompIndex(_index)
//{
//
//}
//
//CComponent::~CComponent() 
//{
//
//}
//
//void CComponent::UpdateComponent() 
//{
//
//}
//
////If Type == eDEFAULT then component is not set properly
//eCompTypes CComponent::GetCompType() const 
//{ 
//	return m_eType;
//}
//
////If Index == -1 then component is not set properly
//int	CComponent::GetCompIndex() const 
//{ 
//	return m_nCompIndex; 
//}
//
////Get the Owner. If nullptr then owner was not set.
//CGameObject* CComponent::GetOwner() const
//{
//	if (!m_pOwner)
//		return nullptr;
//	return m_pOwner;
//}
//
//void CComponent::SetCompType(eCompTypes _type) 
//{ 
//	m_eType = _type; 
//}
//
//void CComponent::SetCompIndex(int _index)
//{
//	m_nCompIndex = _index; 
//}
