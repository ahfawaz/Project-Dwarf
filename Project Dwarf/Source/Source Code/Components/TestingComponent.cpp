#include "../Application/stdafx.h"

//STL Includes

//Header Includes
#include "TestingComponent.h"
#include "../Object Manager/GameObject.h"
#include "../Utilities/TimeManager.h"

//Name Spaces

TestComponent::TestComponent() : CComponent(eCompTypes::eMAIN_COMP, 0)
{
	XMStoreFloat4x4(&m_fPosMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_fRotMatrix, XMMatrixIdentity());
}

TestComponent::~TestComponent()
{
}

void TestComponent::UpdateComponent()
{
	//If there is any special logic with this Component put it here
	if (GetOwner())
	{
		bool change = false;
		XMMATRIX rotation = XMMatrixIdentity();
		XMMATRIX translation = XMMatrixIdentity();
		XMMATRIX world_mat = XMLoadFloat4x4(&GetOwner()->GetWorldMat());
		XMMATRIX pos_mat = XMLoadFloat4x4(&m_fPosMatrix);
		XMMATRIX rot_mat = XMLoadFloat4x4(&m_fRotMatrix);

		if (GetAsyncKeyState(VK_RIGHT))
		{
			change = true;
			rotation = XMMatrixRotationY(XMConvertToRadians(45.0f) * CTimeManager::GetTimeDelta());
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			change = true;
			rotation = XMMatrixRotationY(XMConvertToRadians(-45.0f) * CTimeManager::GetTimeDelta());
		}
		if (GetAsyncKeyState(VK_UP))
		{
			change = true;
			translation = XMMatrixTranslation(0, 0, -5.0f * CTimeManager::GetTimeDelta());
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			change = true;
			translation = XMMatrixTranslation(0, 0, 5.0f * CTimeManager::GetTimeDelta());
		}
		if (GetAsyncKeyState('P'))
		{
			XMStoreFloat4x4(&GetOwner()->GetWorldMat(), XMMatrixIdentity());
		}

		if (change)
		{
			pos_mat = pos_mat * translation;
			XMStoreFloat4x4(&m_fPosMatrix, pos_mat);
			//pos_mat = XMMatrixInverse(NULL, pos_mat);
			rot_mat = rot_mat * rotation;
			XMStoreFloat4x4(&m_fRotMatrix, rot_mat);
			

			world_mat = XMMatrixInverse(NULL, world_mat);
			world_mat = world_mat * translation * rotation;
			//world_mat.r[3] = pos_mat.r[3];
			world_mat = XMMatrixInverse(NULL, world_mat);

			//world_mat = pos_mat * rot_mat;


			XMStoreFloat4x4(&GetOwner()->GetWorldMat(), world_mat);
		}

	}
}

bool TestComponent::Initialize()
{
	return true;
}

void TestComponent::Shutdown()
{
}

//Accessors
CRenderComponent* TestComponent::GetRenderComp() const
{
	if (!m_pGORenderComp)
	{
		//printf_s("%s's Render Comp is NULL", GetOwner()->GetTag());
		return nullptr;
	}

	return m_pGORenderComp;
}

//Mutators
void TestComponent::SetRenderComp(CRenderComponent* _comp)
{
	m_pGORenderComp = _comp;
}