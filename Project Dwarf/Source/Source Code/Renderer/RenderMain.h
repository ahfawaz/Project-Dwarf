#pragma once

//STL Includes
#include <d3d11.h>
#pragma comment(lib, "d3d11")

//Header Includes
#include "RenderController.h"

//Namespaces
using namespace std;
using namespace DirectX;

//Forward Declares
class Core;
class CContextManager;
class CShaderManager;
class CComputeManager;
class CConstantBufferManager;
class CDepthStencilStateManager;
class CBlendStateManager;
class CCameraManager;
class CLightMemoryManager;

class CRenderer
{
	//Private Members
	CRenderController*			m_pRenderController;
	CContextManager*			m_pContextManager;
	CShaderManager*				m_pShaderManager;
	CComputeManager*			m_pComputeManager;
	CConstantBufferManager*		m_pConstantBuffer;
	CDepthStencilStateManager*	m_pDPSManager;
	CBlendStateManager*			m_pBlendStateManager;
	CCameraManager*				m_pCameraManager;
	CLightMemoryManager*		m_pLightMM;



public:
	CRenderer();
	~CRenderer();

	bool Initialize(Core* _core);
	void Shutdown();
	void UpdateDraw();

	//Accessors
	CRenderController* GetRenderController() const;
	CShaderManager* GetShaderManager() const;
	CDepthStencilStateManager* GetDPSManager() const;
	CConstantBufferManager* GetConstantBuffers() const;
	CCameraManager* GetCameraManager() const;
	CLightMemoryManager* GetLightMemoryManager() const;
	//Mutators

private:


};

