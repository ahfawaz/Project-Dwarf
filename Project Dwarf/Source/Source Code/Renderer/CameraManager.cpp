#include "../Application/stdafx.h"

#include "CameraManager.h"

//STL includes 

//Header includes
#include "../Core/Core.h"
#include "../Utilities/TimeManager.h"

//Namespaces used

//Defines
#define CAM_POS XMFLOAT3(0.0f, 1.5f, -2.0f)
#define CAM_FOCUS_POS XMFLOAT3(0.0f, 1.5f, 0.0f)
#define CAM_DIR XMFLOAT3(0.0f, 0.0f, 1.0f)
#define CAM_UP	XMFLOAT3(0.0f, 1.0f, 0.0f)
#define CAM_FOV XMConvertToRadians(90.0f)
//#define ASPECT_RATIO(x, y) FLOAT(x) / FLOAT(y)
#define ASPECT_RATIO(x,y) (16.0f / 9.0f)

#define CAM_MOVE_SPEED 5.0f
#define CAM_TURN_SPEED 5.0f


CCameraManager::CCameraManager()
{
	m_fDirection = CAM_DIR;
	m_fVelocity = { 0, 0, 0 };
}

CCameraManager::~CCameraManager()
{
}

bool CCameraManager::Initialize(Core* _core)
{
	XMMATRIX view_mat = XMMatrixLookAtLH(XMLoadFloat3(&CAM_POS), XMLoadFloat3(&CAM_FOCUS_POS), XMLoadFloat3(&CAM_UP));
	XMMATRIX proj_mat = XMMatrixPerspectiveFovLH(CAM_FOV, ASPECT_RATIO(_core->GetWindowWidth(), _core->GetWindowHeight()), NULL, MAX_DEPTH);

	XMStoreFloat4x4(&m_fViewMatrix, view_mat);
	XMStoreFloat4x4(&m_fProjMatrix, proj_mat);

	return true;
}
void CCameraManager::Shutdown()
{
}

bool CheckVelocity(XMFLOAT3 _check) { if (_check.x == 0 && _check.y == 0 && _check.z == 0) return false; return true; }

void CCameraManager::Update()
{
	if (CheckVelocity(m_fVelocity))
	{
		XMMATRIX view_mat = XMLoadFloat4x4(&m_fViewMatrix);
		view_mat = XMMatrixInverse(NULL, view_mat);

		XMVECTOR velocity = XMLoadFloat3(&m_fVelocity);
		velocity = XMVector3Normalize(velocity);
		velocity = velocity * CAM_MOVE_SPEED * CTimeManager::GetTimeDelta();

		view_mat = XMMatrixTranslationFromVector(velocity) * view_mat;
		view_mat = XMMatrixInverse(NULL, view_mat);
		XMStoreFloat4x4(&m_fViewMatrix, view_mat);
	}

}

void CCameraManager::ResetCamera()
{
	XMMATRIX view_mat = XMMatrixLookAtLH(XMLoadFloat3(&CAM_POS), XMLoadFloat3(&CAM_DIR), XMLoadFloat3(&CAM_UP));

	XMStoreFloat4x4(&m_fViewMatrix, view_mat);
}

void CCameraManager::SetVelocity(XMFLOAT3 _data)
{
	m_fVelocity = _data;
}

void CCameraManager::SetVelocity(XMVECTOR _data)
{
	XMStoreFloat3(&m_fVelocity, _data);
}

void CCameraManager::SetVelocity(float _x, float _y, float _z)
{
	m_fVelocity = { _x, _y, _z };
}



void CCameraManager::RotateView(float _x, float _y)
{
	XMMATRIX view_mat = XMLoadFloat4x4(&m_fViewMatrix);
	view_mat = XMMatrixInverse(NULL, view_mat);
	XMVECTOR view_pos = view_mat.r[3];
	view_mat.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	view_mat = XMMatrixRotationX(_y * CAM_TURN_SPEED) * view_mat * XMMatrixRotationY(_x * CAM_TURN_SPEED);

	view_mat.r[3] = view_pos;

	XMStoreFloat3(&m_fDirection, view_mat.r[2]);
	view_mat = XMMatrixInverse(NULL, view_mat);

	XMStoreFloat4x4(&m_fViewMatrix, view_mat);
}

XMFLOAT4X4 CCameraManager::GetViewMatrix() const
{
	return m_fViewMatrix;
}

XMFLOAT4X4 CCameraManager::GetProjMatrix() const
{
	return m_fProjMatrix;
}

