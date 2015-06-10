#pragma once

//STL includes 

//Header includes

//Namespaces used

//Forward Declares
class CRenderer;
class CGameObject;

class CContextManager
{
	//class members.
	ID3D11DeviceContext* _pContext;
	CRenderer* _pRenderer;

public:
	CContextManager();
	~CContextManager();

	bool Initialize(CRenderer* _data);
	void Shutdown();

	//Function Calls
	void ContextDrawSwitch(CGameObject* _data);

	//Accessors
	ID3D11DeviceContext* GetContext() const;

private: //Private Helper Functions
	//Draw functions
	HRESULT BindPixelShader();

	void BatchDraw();
	void BasicDraw();
	void TexturedDraw();


};
