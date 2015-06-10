#pragma once

#include <vector>
#include <DirectXMath.h>

struct ID3D11Buffer;
struct ID3D11Device2;

using namespace std;
using namespace DirectX;

struct tMesh_Data{
	vector<XMFLOAT3> verts;
	vector<XMFLOAT3> norms;
	vector<XMFLOAT3> tangs;
	vector<XMFLOAT2> uvs;
	vector<UINT> indicies;
};

struct tAnim_Data{
	vector<XMUINT4> joint_inf;
	vector<XMFLOAT4> weights;
};

class CMesh
{
	ID3D11Buffer* m_pVerts;
	ID3D11Buffer* m_pIndex;
	UINT m_nNumVerts;
	UINT m_nNumIndex;
	
	bool m_bAnimated;
	bool m_bTangents;

public:
	CMesh();
	~CMesh();

	bool LoadStaticMesh(ID3D11Device* _device, tMesh_Data& _data);
	bool LoadAnimMesh(ID3D11Device* _device, tMesh_Data& _data, tAnim_Data& _anim);

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	UINT GetNumVerticies() const;
	UINT GetNumIndicies() const;

	bool IsAnimated() const;
	bool IsTangents() const;
};