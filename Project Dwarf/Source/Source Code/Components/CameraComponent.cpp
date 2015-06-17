#include "../Application/stdafx.h"

#include "CameraComponent.h"

//STL Includes

//Header Includes 
#include "../Object Manager/GameObject.h"
#include "../Utilities/TimeManager.h"
#include "../Renderer/CameraManager.h"

//Namespaces Used

CCameraComponent::CCameraComponent() :CComponent(eCompTypes::eCAMERA_COMP)
{
	GetCursorPos(&m_Cursor);
}

CCameraComponent::~CCameraComponent()
{

}

void CCameraComponent::UpdateComponent()
{
	XMFLOAT3 direction = { 0.0f, 0.0f, 0.0f };

	if (GetAsyncKeyState('W') & 1)
	{
		direction.z = 1.0f * CTimeManager::GetTimeDelta();
	}
	if (GetAsyncKeyState('S') & 1)
	{
		direction.z = -1.0f * CTimeManager::GetTimeDelta();
	}
	if (GetAsyncKeyState('A') & 1)
	{
		direction.x = -1.0f * CTimeManager::GetTimeDelta();
	}
	if (GetAsyncKeyState('D') & 1)
	{
		direction.x = 1.0f * CTimeManager::GetTimeDelta();
	}
	if (GetAsyncKeyState('Q') & 1)
	{
		direction.y = -1.0f * CTimeManager::GetTimeDelta();
	}
	if (GetAsyncKeyState('E') & 1)
	{
		direction.y = 1.0f * CTimeManager::GetTimeDelta();
	}
	if (GetAsyncKeyState('P') & 1)
	{
		_pCameraManager->ResetCamera();
	}

	_pCameraManager->SetVelocity(direction);

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		POINT currPos;
		GetCursorPos(&currPos);
		currPos.x = currPos.x - m_Cursor.x;
		currPos.y = currPos.y - m_Cursor.y;
		//view = XMMatrixRotationX((float)currPos.y * (float)XTimer.Delta() * 5) * view * XMMatrixRotationY((float)currPos.x * (float)XTimer.Delta() * 2);
		_pCameraManager->RotateView(float(currPos.x) * CTimeManager::GetTimeDelta(), float(currPos.y) * CTimeManager::GetTimeDelta());
	}
	GetCursorPos(&m_Cursor);

}

bool CCameraComponent::Initialize(CCameraManager * _CameraManager)
{
	if (!_CameraManager)
		return false;

	_pCameraManager = _CameraManager;

	return true;
}

void CCameraComponent::Shutdown()
{

}