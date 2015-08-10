#pragma once

//STL Includes
#include <d3d11.h>
#pragma comment(lib, "d3d11")

//Header Includes
//#include "RenderMain.h"

//Namespaces
using namespace std;

//Forward Declares
class Core;
class CRenderer;
class CGameObject;
class CRenderComponent;

class CRenderController
{
	ID3D11Device*			m_pDevice;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11DeviceContext*	m_pContext;
	ID3D11RenderTargetView*	m_pRTV;
	ID3D11Texture2D*		m_pDepthBuffer;
	ID3D11Resource*			m_pBackBuffer;
	ID3D11DepthStencilView* m_pDSV;
	ID3D11RasterizerState*	m_pRasterizerState;
	D3D11_VIEWPORT			m_Viewport;

	CRenderer*				_pRenderer;
	Core*					_pCore;

public:
	CRenderController();
	~CRenderController();

	bool Initialize(CRenderer* const _renderer, Core* _core);
	void Shutdown();

	//Accessors
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;

	//Mutators

	//Function
	void ClearScene();
	void DrawScene();
	void PresentScene();

private:
	//Private Helper Functions
	HRESULT CreateSwapChainandDevice(HWND _hWnd, bool _windowed);
	HRESULT CreateRenderTarget();
	HRESULT CreateDepthBuffer();
	HRESULT CreateRasterizerState();

	void SetDepthStencilState(UINT _render_type);
	void DrawObject(CGameObject * _obj, CRenderComponent * _comp);
	UINT GetStrideSize(UINT _flag);

};

