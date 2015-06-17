#pragma once

#include "Component.h"

//STL Includes
#include <DirectXMath.h>

//Header Includes

//Namespaces Used
using namespace DirectX;

//Foward Declares
class CCameraManager;
class CRenderer;

class CCameraComponent : public CComponent
{
	CCameraManager* _pCameraManager;
	POINT m_Cursor;
public:
	
	CCameraComponent();
	~CCameraComponent();

	void UpdateComponent();
	bool Initialize(CCameraManager * _CameraManager);
	void Shutdown();
};
