#include "../Application/stdafx.h"

#include "AssetManager.h"

//STL includes
#include <vector>
#include <DirectXMath.h>
#include <algorithm>

//Class includes
#include "Mesh.h"

//Namespaces
using namespace DirectX;

//Define values

CAssetManager::CAssetManager()
{

}
CAssetManager::~CAssetManager()
{

}

bool CAssetManager::Initialize(ID3D11Device * _device)
{
	if (_device)
		_pDevice = _device;
	else
		return false;

	m_pManager = nullptr;

	if (!LoadTexture("../Assets/Default_Texture.dds", &m_pDefaultTexture))
		return false;

	return true;
}
void CAssetManager::Shutdown()
{
	_pDevice = nullptr;

	//Shutdown the FBX Manager if it is still active.
	ShutdownFBX();

	//Clear Lists
	ClearAssets();

	//Remove default texture.
	SAFE_RELEASE(m_pDefaultTexture);
}

//Accessors
CMesh* CAssetManager::GetMesh(string _name)
{
	if (m_vMeshList.find(_name) == m_vMeshList.end())
		return nullptr;

	return m_vMeshList[_name];
}
CAnimation* CAssetManager::GetAnimation(string _name)
{
	if (m_vAnimList.find(_name) == m_vAnimList.end())
		return nullptr;

	return m_vAnimList[_name];
}
ID3D11ShaderResourceView* CAssetManager::GetTexture(string _name)
{
	if (m_vTextureList.find(_name) == m_vTextureList.end())
		return nullptr;

	return m_vTextureList[_name];
}

//Mutators?

//Helper Funcs
bool CAssetManager::LoadMesh(string _filename, CMesh** _mesh_out, bool _animated)
{
	//load only FBX files. No other files type are used.
	string temp = _filename;
	if (temp.substr(temp.find_last_of(".") + 1) != "fbx")
	{
		*_mesh_out = nullptr;
		return false;
	}

	//Make sure it is not a duplicate
	*_mesh_out = GetMesh(_filename);
	if (*_mesh_out != nullptr)
	{
		return true;
	}

	//try to open up that file and load it.
	FbxScene * pScene = nullptr;
	if (!CreateFbxScene(_filename, &pScene))
	{
		*_mesh_out = nullptr;
		return false;
	}

	//Get the Mesh for the file
	FbxMesh * pMesh = nullptr;
	if (!GetFbxMesh(pScene, &pMesh))
	{
		*_mesh_out = nullptr;
		return false;
	}

	//read all the mesh data
	tMesh_Data mesh_data;
	ZeroMemory(&mesh_data, sizeof(tMesh_Data));

	FbxVector4 tempVector; //Used for temp storage of vectors.
	FbxVector2 tempUV; //Used for temp storage of UVs.

	//Get the total number of Triangles and the Vert Count
	int tri_Count = pMesh->GetPolygonCount();
	int vert_count = pMesh->GetPolygonVertexCount();

	//Resize the vectors to hold only the correct amount of data.
	vector<XMFLOAT3>verts;
	verts.resize(vert_count);
	vector<XMFLOAT3>norms;
	norms.resize(vert_count);
	vector<XMFLOAT3>tangs;
	tangs.resize(vert_count);
	vector<XMFLOAT2>uvs;
	uvs.resize(vert_count);
	vector<UINT>indicies;

	int highest_index = 0;

	//Grab the index array
	for (int i = 0; i < vert_count; i++)
		indicies.push_back((UINT)pMesh->GetPolygonVertices()[i]);

	//Grab the Tangents and Normals from the FbxMesh.
	FbxGeometryElementTangent* vertTangents = pMesh->GetElementTangent();
	FbxGeometryElementNormal* vertNormals = pMesh->GetElementNormal();

	if (vertNormals == nullptr)
	{
		*_mesh_out = nullptr;
		return false;
	}

	//Grab the UV names
	FbxStringList texture_names;
	pMesh->GetUVSetNames(texture_names);
	int num_of_sets = texture_names.GetCount();

	if (num_of_sets == 0)
	{
		*_mesh_out = nullptr;
		return false;
	}

	for (int triIndex = 0; triIndex < tri_Count; triIndex++)
	{
		int startIndex = pMesh->GetPolygonVertexIndex(triIndex);
		for (int vertIndex = 0; vertIndex < 3; vertIndex++, startIndex++)
		{
			highest_index = max(highest_index, (INT)indicies[startIndex]);

			//Read the vert for this index.
			int ctrl_Point = pMesh->GetPolygonVertex(triIndex, vertIndex);
			XMFLOAT3 vert((float)pMesh->GetControlPointAt(ctrl_Point)[0], (float)pMesh->GetControlPointAt(ctrl_Point)[1], -(float)pMesh->GetControlPointAt(ctrl_Point)[2]);
			verts[indicies[startIndex]] = vert;

			//Read the Tangents for this vert
			if (vertTangents != nullptr)
			{
				switch (vertTangents->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					tangs[indicies[startIndex]] = XMFLOAT3((float)vertTangents->GetDirectArray().GetAt(indicies[startIndex])[0],
						(float)vertTangents->GetDirectArray().GetAt(indicies[startIndex])[1],
						-(float)vertTangents->GetDirectArray().GetAt(indicies[startIndex])[2]);
					break;
				}
				case FbxGeometryElement::eIndex:
					break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertTangents->GetIndexArray().GetAt(startIndex);
					tangs[indicies[startIndex]] = XMFLOAT3((float)vertTangents->GetDirectArray().GetAt(index)[0],
						(float)vertTangents->GetDirectArray().GetAt(index)[1],
						-(float)vertTangents->GetDirectArray().GetAt(index)[2]);
					break;
				}
				default:
					break;
				}
			}

			//Read the Normals for this vert
			if (vertNormals != nullptr)
			{
				switch (vertNormals->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					norms[indicies[startIndex]] = XMFLOAT3((float)vertNormals->GetDirectArray().GetAt(indicies[startIndex])[0],
						(float)vertNormals->GetDirectArray().GetAt(indicies[startIndex])[1],
						-(float)vertNormals->GetDirectArray().GetAt(indicies[startIndex])[2]);
					break;
				}
				case FbxGeometryElement::eIndex:
				{
					pMesh->GetPolygonVertexNormal(triIndex, vertIndex, tempVector);
					norms[indicies[startIndex]] = XMFLOAT3((float)tempVector[0],
						(float)tempVector[1],
						-(float)tempVector[2]);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertNormals->GetIndexArray().GetAt(startIndex);
					norms[indicies[startIndex]] = XMFLOAT3((float)vertNormals->GetDirectArray().GetAt(index)[0],
						(float)vertNormals->GetDirectArray().GetAt(index)[1],
						-(float)vertNormals->GetDirectArray().GetAt(index)[2]);
					break;
				}
				default:
					break;
				}
			}

			//Read the UVs for this vert
			for (int uvIndex = 0; uvIndex < num_of_sets; uvIndex++)
			{
				FbxGeometryElementUV * vertUV = pMesh->GetElementUV(texture_names[uvIndex]);
				switch (vertUV->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					uvs[indicies[startIndex]] = XMFLOAT2((float)vertNormals->GetDirectArray().GetAt(indicies[startIndex])[0],
						1.0f - (float)vertNormals->GetDirectArray().GetAt(indicies[startIndex])[1]);
					break;
				}
				case FbxGeometryElement::eIndex:
				{
					bool mapped = true;
					pMesh->GetPolygonVertexUV(triIndex, vertIndex, texture_names[uvIndex], tempUV, mapped);
					uvs[indicies[startIndex]] = XMFLOAT2((float)tempUV[0],
						1.0f - (float)tempUV[1]);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertUV->GetIndexArray().GetAt(startIndex);
					uvs[indicies[startIndex]] = XMFLOAT2((float)vertNormals->GetDirectArray().GetAt(index)[0],
						1.0f - (float)vertNormals->GetDirectArray().GetAt(index)[1]);
					break;
				}
				default:
					break;
				}
			}
		}
	}

	highest_index++;
	//mesh_data.verts.resize(highest_index);
	//mesh_data.norms.resize(highest_index);
	//mesh_data.tangs.resize(highest_index);
	//mesh_data.uvs.resize(highest_index);

	//highest_index--;

#if 1
	//This ensures that I only get the largest vertex size there really is.
	mesh_data.verts.assign(verts.begin(), verts.begin() + highest_index);
	if (vertNormals)
		mesh_data.norms.assign(norms.begin(), norms.begin() + highest_index);
	if (vertTangents)
		mesh_data.tangs.assign(tangs.begin(), tangs.begin() + highest_index);
	if (num_of_sets > 0)
		mesh_data.uvs.assign(uvs.begin(), uvs.begin() + highest_index);
#else
	mesh_data.verts = verts;
	if (vertNormals)
		mesh_data.norms = norms;
	if (vertTangents)
		mesh_data.tangs = tangs;
	if (num_of_sets > 0)
		mesh_data.uvs = uvs;
#endif

	//Reverse the index array
	reverse(indicies.begin(), indicies.end());
	mesh_data.indicies = indicies;

	//if the object is animated get that data too
	tAnim_Data anim_data;
	ZeroMemory(&anim_data, sizeof(tAnim_Data));
	if (_animated)
	{
		anim_data.joint_inf.resize(mesh_data.verts.size());
		anim_data.weights.resize(mesh_data.verts.size());

		//Setting the first joint to make sure that each inf is set.
		for (size_t i = 0; i < anim_data.joint_inf.size(); i++)
			anim_data.joint_inf[i].x = -1;

		FbxSkin * pSkin = static_cast<FbxSkin*>(pMesh->GetDeformer(0));

		int clusterCount = pSkin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
			LoadSkin(pSkin->GetCluster(clusterIndex), clusterIndex, &anim_data);
	}

	//create the mesh
	CMesh* mesh = new CMesh;
	if (_animated)
	{
		if (!mesh->LoadAnimMesh(_pDevice, mesh_data, anim_data))
		{
			delete mesh;
			mesh = nullptr;

			*_mesh_out = nullptr;
			return false;
		}
	}
	else
	{
		if (!mesh->LoadStaticMesh(_pDevice, mesh_data))
		{
			delete mesh;
			mesh = nullptr;

			*_mesh_out = nullptr;
			return false;
		}
	}

	//store it in the list
	m_vMeshList[_filename] = mesh;
	*_mesh_out = mesh;

	return true;
}
bool CAssetManager::LoadAnim(string _filename)
{
	return false;
}
bool CAssetManager::LoadTexture(string _filename, ID3D11ShaderResourceView** _texture)
{
	bool loaded = false;
	//load DDS
	loaded = LoadDDSTexture(_filename, _texture);
	//load png
	loaded = LoadPNGTexture(_filename, _texture);
	//load tga
	loaded = LoadTgaTexture(_filename, _texture);
	//loag jpeg
	loaded = LoadJpegTexture(_filename, _texture);

	return loaded;
}

void CAssetManager::InitializeFBX()
{
	m_pManager = FbxManager::Create();
}
void CAssetManager::ShutdownFBX()
{
	if (m_pManager)
		m_pManager->Destroy();

	m_pManager = nullptr;
}

void CAssetManager::ClearAssets()
{
	//Clear Mesh List
	for (auto iter = m_vMeshList.begin(); iter != m_vMeshList.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
	m_vMeshList.clear();

	//Clear Animation List
	//for (auto iter = m_vAnimList.begin(); iter != m_vAnimList.end(); iter++)
	//{
	//	SAFE_DELETE(iter->second);
	//}
	//m_vAnimList.clear();

	//Clear Texture List
	for (auto iter = m_vTextureList.begin(); iter != m_vTextureList.end(); iter++)
	{
		SAFE_RELEASE(iter->second);
	}
	m_vTextureList.clear();
}

//Private Helper Functions
bool CAssetManager::CreateFbxScene(string _filename, FbxScene** _scene)
{
	if (m_pManager == nullptr)
	{
		*_scene = nullptr;
		return false;
	}

	FbxString Filename = _filename.c_str();
	if (!FbxFileUtils::Exist(Filename.Buffer()))
	{
		*_scene = nullptr;
		return false;
	}

	FbxImporter * pImporter = FbxImporter::Create(m_pManager, "");

	pImporter->Initialize(Filename.Buffer());

	FbxScene * pScene = FbxScene::Create(m_pManager, Filename.Buffer());

	if (!pImporter->Import(pScene))
	{
		*_scene = nullptr;
		return false;
	}

	pImporter->Destroy();

	*_scene = pScene;

	return true;
}

bool CAssetManager::GetFbxMesh(FbxScene* _scene, FbxMesh** _mesh)
{
	FbxNode * pNode = nullptr;
	FbxMesh * pMesh = nullptr;
	int nodeCount = _scene->GetNodeCount();

	//Find the first node with a mesh;
	for (int i = 0; i < nodeCount; i++)
	{
		pNode = _scene->GetNode(i);

		pMesh = pNode->GetMesh();
		if (pMesh)
			break;
	}

	if (pMesh == nullptr)
	{
		*_mesh = nullptr;
		return false;
	}

	*_mesh = pMesh;
	return true;
}

void CAssetManager::LoadSkin(FbxCluster * pCluster, int joint_index, tAnim_Data* _data)
{
	tAnim_Data& anim = *_data;
	int count = pCluster->GetControlPointIndicesCount();
	for (int index = 0; index < count; index++)
	{
		int point = pCluster->GetControlPointIndices()[index];
		float weight = (float)pCluster->GetControlPointWeights()[index];

		if (weight < 1.0f)
		{
			if (anim.joint_inf[point].x == -1)
			{
				anim.joint_inf[point].x = joint_index;
				anim.weights[point].x = weight;
			}
			else if (anim.joint_inf[point].y == NULL)
			{
				anim.joint_inf[point].y = joint_index;
				anim.weights[point].y = weight;
			}
			else if (anim.joint_inf[point].z == NULL)
			{
				anim.joint_inf[point].z = joint_index;
				anim.weights[point].z = weight;
			}
			else if (anim.joint_inf[point].w == NULL)
			{
				anim.joint_inf[point].w = joint_index;
				anim.weights[point].w = weight;
			}
		}
	}
}

bool CAssetManager::LoadDDSTexture(string _filename, ID3D11ShaderResourceView** _texture)
{
	return true;
}
bool CAssetManager::LoadPNGTexture(string _filename, ID3D11ShaderResourceView** _texture)
{
	return true;
}
bool CAssetManager::LoadJpegTexture(string _filename, ID3D11ShaderResourceView** _texture)
{
	return true;
}
bool CAssetManager::LoadTgaTexture(string _filename, ID3D11ShaderResourceView** _texture)
{
	return true;
}
