#include "../Application/stdafx.h"

#include "Mesh.h"

//STL includes
#include <d3d11_2.h>
#pragma comment(lib, "d3d11")

//Header includes
#include "../Renderer/Shaders/VertexBuffers.h"



CMesh::CMesh()
{
	m_pVerts = NULL;
	m_pIndex = NULL;
	m_nNumVerts = NULL;
	m_nNumIndex = NULL;
	m_bAnimated = false;
	m_bTangents = false;
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVerts);
	SAFE_RELEASE(m_pIndex);
}

bool CMesh::LoadStaticMesh(ID3D11Device* _device, tMesh_Data& _data)
{
	if (_device == nullptr)
		return false;

	m_nNumVerts = (UINT)_data.verts.size();
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC BufferDESC;
	D3D11_SUBRESOURCE_DATA initData;

	//if (_data.tangs.size() > 0)
	//{
	//	vector<v_StaticWithTangs> buffer;
	//	v_StaticWithTangs temp_vert;
	//	for (UINT i = 0; i < _data.verts.size(); i++)
	//	{
	//		temp_vert.pos = _data.verts[i];
	//		temp_vert.tang = _data.tangs[i];
	//		temp_vert.norm = _data.norms[i];
	//		temp_vert.uv = _data.uvs[i];

	//		buffer.push_back(temp_vert);
	//	}

	//	ZeroMemory(&BufferDESC, sizeof(D3D11_BUFFER_DESC));
	//	BufferDESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	BufferDESC.Usage = D3D11_USAGE_DEFAULT;
	//	BufferDESC.ByteWidth = sizeof(v_StaticWithTangs)* m_nNumVerts;

	//	initData.pSysMem = &buffer[0];
	//	initData.SysMemPitch = NULL;
	//	initData.SysMemSlicePitch = NULL;

	//	hr = _device->CreateBuffer(&BufferDESC, &initData, &m_pVerts);

	//	if (hr != S_OK)
	//	{
	//		return false;
	//	}

	//	m_bTangents = true;
	//}
	//else
	{
		vector<v_StaticNoTangs> buffer;
		v_StaticNoTangs temp_vert;
		for (UINT i = 0; i < _data.verts.size(); i++)
		{
			temp_vert.pos = _data.verts[i];
			temp_vert.norm = _data.norms[i];
			temp_vert.uv = _data.uvs[i];

			buffer.push_back(temp_vert);
		}

		ZeroMemory(&BufferDESC, sizeof(D3D11_BUFFER_DESC));
		BufferDESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDESC.Usage = D3D11_USAGE_DEFAULT;
		BufferDESC.ByteWidth = sizeof(v_StaticNoTangs)* m_nNumVerts;

		initData.pSysMem = &buffer[0];
		initData.SysMemPitch = NULL;
		initData.SysMemSlicePitch = NULL;

		hr = _device->CreateBuffer(&BufferDESC, &initData, &m_pVerts);

		if (hr != S_OK)
		{
			return false;
		}

	}

	m_nNumIndex = (UINT)_data.indicies.size();
	ZeroMemory(&BufferDESC, sizeof(D3D11_BUFFER_DESC));
	BufferDESC.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDESC.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDESC.ByteWidth = sizeof(UINT)* m_nNumIndex;

	initData.pSysMem = &_data.indicies[0];
	initData.SysMemPitch = NULL;
	initData.SysMemSlicePitch = NULL;

	hr = _device->CreateBuffer(&BufferDESC, &initData, &m_pIndex);

	if (hr != S_OK)
	{
		return false;
	}


	return true;
}
bool CMesh::LoadAnimMesh(ID3D11Device* _device, tMesh_Data& _data, tAnim_Data& _anim)
{
	if (_device == nullptr)
		return false;

	m_nNumVerts = (UINT)_data.verts.size();
	HRESULT hr = S_OK;

	if (_data.tangs.size() > 0)
	{
		vector<v_AnimWithTangs> buffer;
		v_AnimWithTangs temp_vert;
		for (UINT i = 0; i < _data.verts.size(); i++)
		{
			temp_vert.pos = _data.verts[i];
			temp_vert.tang = _data.tangs[i];
			temp_vert.norm = _data.norms[i];
			temp_vert.uv = _data.uvs[i];
			temp_vert.joint_inf = _anim.joint_inf[i];
			temp_vert.weights = _anim.weights[i];

			buffer.push_back(temp_vert);
		}

		D3D11_BUFFER_DESC BufferDESC;
		ZeroMemory(&BufferDESC, sizeof(D3D11_BUFFER_DESC));
		BufferDESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDESC.Usage = D3D11_USAGE_DEFAULT;
		BufferDESC.ByteWidth = sizeof(v_AnimWithTangs)* m_nNumVerts;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &buffer[0];
		initData.SysMemPitch = NULL;
		initData.SysMemSlicePitch = NULL;

		hr = _device->CreateBuffer(&BufferDESC, &initData, &m_pVerts);

		if (hr != S_OK)
		{
			return false;
		}

		m_bTangents = true;

	}
	else
	{
		vector<v_AnimNoTangents> buffer;
		v_AnimNoTangents temp_vert;
		for (UINT i = 0; i < _data.verts.size(); i++)
		{
			temp_vert.pos = _data.verts[i];
			temp_vert.norm = _data.norms[i];
			temp_vert.uv = _data.uvs[i];
			temp_vert.joint_inf = _anim.joint_inf[i];
			temp_vert.weights = _anim.weights[i];

			buffer.push_back(temp_vert);
		}

		D3D11_BUFFER_DESC BufferDESC;
		ZeroMemory(&BufferDESC, sizeof(D3D11_BUFFER_DESC));
		BufferDESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDESC.Usage = D3D11_USAGE_DEFAULT;
		BufferDESC.ByteWidth = sizeof(v_AnimNoTangents)* m_nNumVerts;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &buffer[0];
		initData.SysMemPitch = NULL;
		initData.SysMemSlicePitch = NULL;

		hr = _device->CreateBuffer(&BufferDESC, &initData, &m_pVerts);

		if (hr != S_OK)
		{
			return false;
		}
	}

	m_nNumIndex = (UINT)_data.indicies.size();
	D3D11_BUFFER_DESC BufferDESC;
	ZeroMemory(&BufferDESC, sizeof(D3D11_BUFFER_DESC));
	BufferDESC.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDESC.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDESC.ByteWidth = sizeof(UINT)* m_nNumIndex;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &_data.indicies[0];
	initData.SysMemPitch = NULL;
	initData.SysMemSlicePitch = NULL;

	hr = _device->CreateBuffer(&BufferDESC, &initData, &m_pIndex);

	if (hr != S_OK)
	{
		return false;
	}

	m_bAnimated = true;

	return true;
}

ID3D11Buffer* CMesh::GetVertexBuffer() const
{
	return m_pVerts;
}
ID3D11Buffer* CMesh::GetIndexBuffer() const
{
	return m_pIndex;
}
UINT CMesh::GetNumVerticies() const
{
	return m_nNumVerts;
}
UINT CMesh::GetNumIndicies() const
{
	return m_nNumIndex;
}

bool CMesh::IsAnimated() const
{
	return m_bAnimated;
}

bool CMesh::IsTangents() const
{
	return m_bTangents;
}
