#pragma once

//STL includes 
#include <DirectXMath.h>

//Header includes

//Namespaces used

//Forward Declares
class Core;

class CCameraManager
{
	XMFLOAT4X4 m_fViewMatrix;
	XMFLOAT4X4 m_fProjMatrix;

	XMFLOAT3 m_fVelocity;
	XMFLOAT3 m_fDirection;

public:
	CCameraManager();
	~CCameraManager();

	bool Initialize(Core* _core);
	void Shutdown();

	void Update();
	void ResetCamera();

	void SetVelocity(XMFLOAT3 _data);
	void SetVelocity(XMVECTOR _data);
	void SetVelocity(float _x, float _y, float _z);

	//Rotates the View Matrix (m_fViewMat) by the passed in data
	//_x is the amount to rotate on the X axis in radians
	//_y is the amount to rotate on the Y axis in radians
	void RotateView(float _x, float _y);

	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetProjMatrix() const;

	//Private Functions
private:

};


