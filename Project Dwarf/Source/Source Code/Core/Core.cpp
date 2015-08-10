#include "../Application/stdafx.h"

//STL Includes

//Header Includes
#include "Core.h"
#include "../Object Manager/ObjectManager.h"
#include "../Asset Manager/AssetManager.h"
#include "../Level Manager/LevelManager.h"

//Namespaces used

#define TEST_LEVEL 1
#define TEST_FP "N/A"


Core::Core()
{
	m_pObjectManager = nullptr;
	m_pAssetManager = nullptr;
	m_pLevelManager = nullptr;
}

Core::~Core()
{
}

void Core::Update()
{
	//Update the Game Clock
	m_TimeManager.Update();

	//Update the Game Logic

	//Update the Objects
	if (m_pObjectManager)
		m_pObjectManager->Update();

	//Update the Physics

	//Update the Particles

	//Update the Animations

	//Draw the Objects
	if (m_pRenderer)
		m_pRenderer->UpdateDraw();
}

bool Core::Initialize(HWND _hWnd, HINSTANCE _hInstance, int _width, int _height, bool _windowed)
{
	m_Window = _hWnd;
	m_hInstance = _hInstance;
	m_nWidth = _width;
	m_nHeight = _height;
	m_bWindowed = _windowed;

	//Initialize the TimeManager
	m_TimeManager.Initialize();

	//Initialize the Renderer
	m_pRenderer = new CRenderer;
	if (!m_pRenderer->Initialize(this))
	{
		SAFE_SHUTDOWN(m_pRenderer);
	}

	//Initialize the Animation System

	//Initialize the Particle System

	//Initialize the Physic System

	//Initialize the Asset Manager
	m_pAssetManager = new CAssetManager;
	if (m_pRenderer)
	{
		if (!m_pAssetManager->Initialize(m_pRenderer->GetRenderController()->GetDevice())) //must change to get the right device.
		{
			SAFE_SHUTDOWN(m_pAssetManager);
		}
	}
	else if (!m_pAssetManager->Initialize(NULL)) //must change to get the right device.
	{
		SAFE_SHUTDOWN(m_pAssetManager);
	}

	//Initialize the Object Manager
	m_pObjectManager = new CObjectManager;
	if (!m_pObjectManager->Initialize())
	{
		SAFE_SHUTDOWN(m_pObjectManager);
	}

	//Initialize the Level Manager
	m_pLevelManager = new CLevelManager;
	if (!m_pLevelManager->Initialize())
	{
		SAFE_SHUTDOWN(m_pLevelManager);
	}

	//Initialize the Game Logic

	//Load Level
	if (m_pLevelManager)
	{
#ifdef TEST_LEVEL
		m_pLevelManager->LoadTestLevel(this);
#else
		m_pLevelManager->LoadLevel(TEST_FP, this);
#endif
	}

	//Store the HWND, HINSTANCE, width, height, and the Window Mode

	return true;
}

void Core::Shutdown()
{
	//Shutdown the Game Logic

	//Shutdown the Level Manager
	SAFE_SHUTDOWN(m_pLevelManager);

	//Shutdown the Object Manager
	SAFE_SHUTDOWN(m_pObjectManager);

	//Shutdown the Asset Manager
	SAFE_SHUTDOWN(m_pAssetManager);

	//Shutdown the Physic System

	//Shutdown the Particle System

	//Shutdown the Animation System

	//Shutdown the Renderer
	SAFE_SHUTDOWN(m_pRenderer);

}

//Accessors;
CObjectManager* Core::GetObjectManager() const
{
	return m_pObjectManager;
}

CAssetManager* Core::GetAssetManager() const
{
	return m_pAssetManager;
}

CRenderer* Core::GetRenderer() const
{
	return m_pRenderer;
}

HWND Core::GetWindow() const
{
	return m_Window;
}

HINSTANCE Core::GetInstance() const
{
	return m_hInstance;
}

UINT Core::GetWindowWidth() const
{
	return m_nWidth;
}

UINT Core::GetWindowHeight() const
{
	return m_nHeight;
}

bool Core::IsWindowed() const
{
	return m_bWindowed;
}

//Mutators
void Core::SetWindow(HWND _window)
{
	m_Window = _window;
}

void Core::SetInstance(HINSTANCE _instance)
{
	m_hInstance = _instance;
}

void Core::SetWindowWidth(UINT _width)
{
	m_nWidth = _width;
}

void Core::SetWindowHeight(UINT _height)
{
	m_nHeight = _height;
}

void Core::SetIsWindowed(bool _windowed)
{
	m_bWindowed = _windowed;
}


