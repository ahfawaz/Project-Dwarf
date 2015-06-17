#pragma once

//STL Includes
#include <vector>
#include <d3d11.h>
#pragma comment(lib, "d3d11")

//Header Includes
#include "Component.h"

//Name Spaces that are being used
using namespace std;
using namespace DirectX;

//Forward declared Classes
class CMesh;

struct tBatchDraw{
	bool bDraw;
	vector<XMFLOAT4X4> worldPos;
};

struct tModel{
	CMesh * pMesh;
	ID3D11ShaderResourceView* pDiffuse;
	ID3D11ShaderResourceView* pSpecular;
	ID3D11ShaderResourceView* pNormals;

	tModel() : pMesh(nullptr), pDiffuse(nullptr), pSpecular(nullptr), pNormals(nullptr) {}
};

enum eRenderTypes{ eNONE, eDEF_RENDER = 0x1, eBATCH_RENDER = 0x2, eANIM_RENDER = 0x4, eTANGENT_RENDER = 0x8, 
					eBATCH_W_TANGENTS = 0xA, eANIM_W_TANGENTS = 0xC, };

class CRenderComponent : public CComponent
{
	//Variables for Batch Drawing
	bool m_bBatchDraw;
	vector<XMFLOAT4X4> m_vBatchPos;

	UINT m_nFlagTypes;

	CMesh * m_pMesh;
	ID3D11ShaderResourceView* m_pDiffuse;
	ID3D11ShaderResourceView* m_pSpecular;
	ID3D11ShaderResourceView* m_pNormals;

public:
	CRenderComponent();
	virtual ~CRenderComponent();
	virtual void UpdateComponent();
	virtual bool Initiailze(tModel * _data, tBatchDraw * _batch = nullptr);
	virtual void Shutdown();

	UINT GetRenderType() const;
	vector<XMFLOAT4X4>& GetBatchPositions();

	CMesh* GetMesh() const;
	ID3D11ShaderResourceView* GetDiffuse() const;
	ID3D11ShaderResourceView* GetNormals() const;
	ID3D11ShaderResourceView* GetSpecular() const;

};

