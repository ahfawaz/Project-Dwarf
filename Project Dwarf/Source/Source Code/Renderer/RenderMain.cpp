#include "../Application/stdafx.h"

#include "RenderMain.h"

//STL includes

//Header incldes
#include "RenderController.h"
#include "ShaderManager.h"
#include "DepthStencilStateManager.h"
#include "ConstantBufferManager.h"
#include "CameraManager.h"
#include "LightMemoryManager.h"
#include "../Core/Core.h"

//Namespaces used

//Definitions used.

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}


bool CRenderer::Initialize(Core* _core)
{
	//Initialize the Render Controller
	m_pRenderController = new CRenderController();
	if (!m_pRenderController->Initialize(this, _core))
		return false;

	m_pShaderManager = new CShaderManager;
	if (!m_pShaderManager->Initialize(m_pRenderController->GetDevice()))
		return false;

	m_pDPSManager = new CDepthStencilStateManager;
	if (!m_pDPSManager->Initialize(m_pRenderController->GetDevice()))
		return false;

	m_pConstantBuffer = new CConstantBufferManager;
	if (!m_pConstantBuffer->Initialize(m_pRenderController->GetDevice(), m_pRenderController->GetContext()))
		return false;

	m_pCameraManager = new CCameraManager;
	if (!m_pCameraManager->Initialize(_core))
		return false;

	m_pLightMM = new CLightMemoryManager; //Just create the object and have it ready to load data into it.... should probably create a function that says load data since 
	if (!m_pLightMM->Initialize())
		return false;

	return true;
}

void CRenderer::Shutdown()
{
	SAFE_SHUTDOWN(m_pCameraManager);
	SAFE_SHUTDOWN(m_pConstantBuffer);
	SAFE_SHUTDOWN(m_pDPSManager);
	SAFE_SHUTDOWN(m_pShaderManager);
	SAFE_SHUTDOWN(m_pRenderController); //This is always the last thing to shutdown.
}

void CRenderer::UpdateDraw()
{
	//More to do here
	m_pCameraManager->Update();
	m_pRenderController->ClearScene();
	m_pRenderController->DrawScene();
	m_pRenderController->PresentScene();
}

//Accessors
CRenderController* CRenderer::GetRenderController() const
{
	return m_pRenderController;
}

CShaderManager* CRenderer::GetShaderManager() const
{
	return m_pShaderManager;
}

CDepthStencilStateManager* CRenderer::GetDPSManager() const
{
	return m_pDPSManager;
}

CConstantBufferManager* CRenderer::GetConstantBuffers() const
{
	return m_pConstantBuffer;
}

CCameraManager* CRenderer::GetCameraManager() const
{
	return m_pCameraManager;
}

CLightMemoryManager* CRenderer::GetLightMemoryManager() const
{
	return m_pLightMM;
}

//Mutators


