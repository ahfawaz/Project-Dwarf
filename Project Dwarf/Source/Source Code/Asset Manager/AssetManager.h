#pragma once

//STL includes
#include <map>
#include <string>

#include <d3d11.h>
#pragma comment(lib, "d3d11")

#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk")

//Class includes

//Namespaces
using namespace std;

//Forward Declarations
class CMesh;
class CAnimation;
struct tAnim_Data;

class CAssetManager
{ 
	map<string, CMesh*> m_vMeshList;
	map<string, CAnimation*> m_vAnimList;
	map<string, ID3D11ShaderResourceView*> m_vTextureList;

	ID3D11ShaderResourceView* m_pDefaultTexture;
	FbxManager* m_pManager;

	ID3D11Device * _pDevice;

public:
	CAssetManager();
	~CAssetManager();

	bool Initialize(ID3D11Device * _device);
	void Shutdown();

	//Accessors
	CMesh* GetMesh(string _name);
	CAnimation* GetAnimation(string _name);
	ID3D11ShaderResourceView* GetTexture(string _name);

	//Mutators?

	//Helper Funcs
	bool LoadMesh(string _filename, CMesh ** _mesh_out, bool _animated = false);
	bool LoadAnim(string _filename);
	bool LoadTexture(string _filename, ID3D11ShaderResourceView** _texture);

	void InitializeFBX();
	void ShutdownFBX();
	void ClearAssets();

	//Private Helper Functions
private:
	bool CreateFbxScene(string _filename, FbxScene** _scene);
	bool GetFbxMesh(FbxScene* _scene, FbxMesh** _mesh);
	void LoadSkin(FbxCluster * pCluster, int joint_index, tAnim_Data* _data);

	///Texture loaders
	//Loads in DDS texture files.
	//Input: 
	//string _filename = the relative file path to the location of the texture.
	//ID3D11ShaderResourceView **_texture = the destination to load the texture into.
	//Output: 
	//bool = if loading succeeded. 
	bool LoadDDSTexture(string _filename, ID3D11ShaderResourceView** _texture);
	//Loads in other types of texture files. (ex: bmp, tga, png, etc.)
	//Input: 
	//string _filename = the relative file path to the location of the texture.
	//ID3D11ShaderResourceView **_texture = the destination to load the texture into.
	//Output: 
	//bool = if loading succeeded. 
	bool LoadOtherTexture(string _filename, ID3D11ShaderResourceView** _texture);



};