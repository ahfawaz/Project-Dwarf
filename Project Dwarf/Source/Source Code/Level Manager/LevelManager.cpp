#include "../Application/stdafx.h"

#include "LevelManager.h"
#include "../Core/Core.h"
#include "../Object Manager/ObjectManager.h"
#include "../Object Manager/GameObject.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/Mesh.h"

//Component includes
#include "../Components/Component.h"
#include "../Components/RenderComponent.h"
#include "../Components/TestingComponent.h"

CLevelManager::CLevelManager()
{

}

CLevelManager::~CLevelManager()
{

}

bool CLevelManager::Initialize()
{
	return true;
}

void CLevelManager::Shutdown()
{

}

void CLevelManager::LoadLevel(string _filename, Core* _core)
{

}

void CLevelManager::LoadTestLevel(Core* _core)
{
	if (!_core->GetAssetManager() || !_core->GetObjectManager())
		return;

	CAssetManager& _pAsset_Manager = *_core->GetAssetManager();
	CObjectManager& _pObj_Manager = *_core->GetObjectManager();

		_pAsset_Manager.InitializeFBX();

	//Create a game object to be add to the object manager
	CGameObject* player = new CGameObject;
	
	//Create the main component
	TestComponent* test_comp = new TestComponent;
	test_comp->SetOwner(player);
	//Add test component to the game object
	player->AddComponent(test_comp);

	//Set the Render Data
	CRenderComponent* render_comp = new CRenderComponent();
	tModel data;
	//Load the mesh for the component
	_pAsset_Manager.LoadMesh("../Assets/Character Models/Player/Dwarf_Model.fbx", &data.pMesh);
	//Initialize the component and add it to the game object
	render_comp->Initiailze(&data);
	player->AddComponent(render_comp);


	_pObj_Manager.AddObject(player);

	_pAsset_Manager.ShutdownFBX();
}