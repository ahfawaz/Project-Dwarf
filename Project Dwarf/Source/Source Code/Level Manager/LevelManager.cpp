#include "../Application/stdafx.h"

#include "LevelManager.h"
#include "../Core/Core.h"
//#include "../Renderer/RenderMain.h"
#include "../Renderer/CameraManager.h"
#include "../Object Manager/ObjectManager.h"
#include "../Object Manager/GameObject.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/Mesh.h"

//Component includes
#include "../Components/Component.h"
#include "../Components/RenderComponent.h"
#include "../Components/TestingComponent.h"
#include "../Components/CameraComponent.h"

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

	///Test Object 1
	//Create a game object to be add to the object manager
	CGameObject* Test_Model = new CGameObject;
	
	//Create the main component
	TestComponent* test_comp = new TestComponent;
	test_comp->SetOwner(Test_Model);
	//Add test component to the game object
	Test_Model->AddComponent(test_comp);

	//Set the Render Data
	CRenderComponent* render_comp = new CRenderComponent();
	tModel data;
	//Load the mesh for the component
	_pAsset_Manager.LoadMesh("../Assets/Character Models/Player/Test_Model_2.fbx", &data.pMesh);
	//Initialize the component and add it to the game object
	render_comp->Initiailze(&data);
	Test_Model->AddComponent(render_comp);
	Test_Model->GetObjectType() = eObjTypes::eTEST_OBJ;

	_pObj_Manager.AddObject(Test_Model);

	///Test Object 2
	//Create a game object to be add to the object manager
	Test_Model = new CGameObject;

	//Create the main component
	test_comp = new TestComponent;
	test_comp->SetOwner(Test_Model);
	//Add test component to the game object
	Test_Model->AddComponent(test_comp);

	//Set the Render Data
	render_comp = new CRenderComponent();
	//Load the mesh for the component
	_pAsset_Manager.LoadMesh("../Assets/Character Models/Player/Test_Model_1.fbx", &data.pMesh);
	//Initialize the component and add it to the game object
	render_comp->Initiailze(&data);
	XMStoreFloat4x4(&Test_Model->GetWorldMat(), XMMatrixTranslation(5.0f, 0, 0));
	Test_Model->AddComponent(render_comp);
	Test_Model->GetObjectType() = eObjTypes::eTEST_OBJ;

	_pObj_Manager.AddObject(Test_Model);


	///Test Object 2
	//Create a game object to be add to the object manager
	Test_Model = new CGameObject;

	//Create the main component
	test_comp = new TestComponent;
	test_comp->SetOwner(Test_Model);
	//Add test component to the game object
	Test_Model->AddComponent(test_comp);

	//Set the Render Data
	render_comp = new CRenderComponent();
	//Load the mesh for the component
	_pAsset_Manager.LoadMesh("../Assets/Character Models/Player/Dwarf_Model.fbx", &data.pMesh);
	//Initialize the component and add it to the game object
	render_comp->Initiailze(&data);
	XMStoreFloat4x4(&Test_Model->GetWorldMat(), XMMatrixTranslation(-5.0f, 0, 0));
	Test_Model->AddComponent(render_comp);
	Test_Model->GetObjectType() = eObjTypes::eTEST_OBJ;

	_pObj_Manager.AddObject(Test_Model);

	//Creating the Player for Camera controll
	CGameObject* Player = new CGameObject;
	CCameraComponent* cam_comp = new CCameraComponent;
	cam_comp->SetOwner(Player);
	bool happening = cam_comp->Initialize(_core->GetRenderer()->GetCameraManager());
	Player->AddComponent(cam_comp);
	Player->GetObjectType() = eObjTypes::ePLAYER_OBJ;
	_pObj_Manager.AddObject(Player);


	//Shutdown FBX we don't need it anymore.
	_pAsset_Manager.ShutdownFBX();
}