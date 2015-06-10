#pragma once 

//STL Includes 

//Header Includes 
#include <d3d11_2.h>

//Namespaces used

//Forward Declares

enum eDepthStencilStates{ eDSS_DEFAULT, eDSS_SIZE};

class CDepthStencilStateManager
{
	//Class Members
	vector<ID3D11DepthStencilState*> m_pDSSList;

public:
	CDepthStencilStateManager();
	~CDepthStencilStateManager();

	bool Initialize(ID3D11Device* _device);
	void Shutdown();

	bool EnableDepthStencilState(eDepthStencilStates _state, ID3D11DeviceContext* _context);

private:

};
