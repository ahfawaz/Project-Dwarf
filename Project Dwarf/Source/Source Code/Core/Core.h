#pragma once 

//STL Includes

//Header Includes
#include "../Renderer/RenderMain.h"
#include "../Utilities/TimeManager.h"

//Namespaces used

//Forward Declares
class CAssetManager;
class CObjectManager;
class CLevelManager;
class CAnimationSystem;
class CParticleSystem;
class CGameLogic;
class CPhysicSystem;
class CRenderer;


class Core
{
	//Members
	CRenderer* m_pRenderer;
	CObjectManager * m_pObjectManager;
	CAssetManager * m_pAssetManager;
	CLevelManager * m_pLevelManager;

	HWND		m_Window;
	HINSTANCE	m_hInstance;
	UINT		m_nWidth;
	UINT		m_nHeight;
	bool		m_bWindowed;

	CTimeManager m_TimeManager;

public:
	Core();
	~Core();

	void Update();
	bool Initialize(HWND _hWnd, HINSTANCE _hInstance, int _width, int _height, bool _windowed);
	void Shutdown();

	//Accessors
	CObjectManager* GetObjectManager() const;
	CAssetManager* GetAssetManager() const;
	CRenderer* GetRenderer() const;
	HWND GetWindow() const;
	HINSTANCE GetInstance() const;
	UINT GetWindowWidth() const;
	UINT GetWindowHeight() const;
	bool IsWindowed() const;

	//Mutators
	void SetWindow(HWND _window);
	void SetInstance(HINSTANCE _instance);
	void SetWindowWidth(UINT _width);
	void SetWindowHeight(UINT _height);
	void SetIsWindowed(bool _windowed);

	//Helper Funcs

private:
	//Class only Helper Funcs

};

