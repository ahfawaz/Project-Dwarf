#include "../Application/stdafx.h"

//Header Includes
#include "LevelManager.h"
#include "../Utilities/Util.h"
#include "../Core/Core.h"
#include "../Renderer/RenderMain.h"
#include "../Renderer/CameraManager.h"
#include "../Renderer/LightMemoryManager.h"
#include "../Object Manager/ObjectManager.h"
#include "../Object Manager/GameObject.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/Mesh.h"

//Component includes
#include "../Components/Component.h"
#include "../Components/RenderComponent.h"
#include "../Components/TestingComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"

//Preprocessor defines


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
	CRenderer& _pRenderer = *_core->GetRenderer();

	_pAsset_Manager.InitializeFBX();

	//Common Data amongst all the light data.
	CComponent* pLight_Comp = nullptr;
	tPoint_Light Point_Light;
	tCone_Light Cone_Light;
	tDirectional_Light Dir_Light;
	vector<tPoint_Light> Point_Light_List;
	vector<tCone_Light> Cone_Light_List;
	vector<tDirectional_Light> Dir_Light_List;
	vector<CPoint_Light*> Point_Light_Comps;
	vector<CCone_Light*> Cone_Light_Comps;
	vector<CDirectional_Light*> Dir_Light_Comps;
	string Light_Tag;

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


	///Test Batch call
	//Create a game object to be added to the object manager
	Test_Model = new CGameObject;

	//Creat the render data
	render_comp = new CRenderComponent();
	_pAsset_Manager.LoadMesh("../Assets/Character Models/Player/Test_Model_2.fbx", &data.pMesh);

	//Create a batch draw
	tBatchDraw test_batch;
	test_batch.bDraw = true;
	//float x_avg = 0; //Testing numbers. 
	//float z_avg = 0;
	XMFLOAT4 vec_pos;
	XMFLOAT4X4 mat_pos;



	for (size_t i = 0; i < 1024u; i++)
	{
		vec_pos = { Dwarf_Util::SignedRandFloat()* 50.0f, Dwarf_Util::UnsignedRandFloat()* 100.0f, Dwarf_Util::SignedRandFloat()* 50.0f, 1.0f }; //store the pos for the use in creating the lights.
		XMStoreFloat4x4(&mat_pos, XMMatrixTranslation(vec_pos.x, vec_pos.y, vec_pos.z));
		//x_avg += pos._41;
		//z_avg += pos._43;
		test_batch.worldPos.push_back(mat_pos);

		//Testing Light Loading and testing rendering with lights.
		{
			//Point Light Loading
			Point_Light.fLight_Col = Dwarf_Util::RandomColor();
			Point_Light.fLight_Pos = vec_pos;
			Point_Light.fLight_Pos.y += 5.0f;
			Point_Light.fRadius = 15.0f * Dwarf_Util::UnsignedRandFloat() + 15.0f;
			Point_Light.uActive = ACTIVE;
			Point_Light_List.push_back(Point_Light);

			pLight_Comp = new CPoint_Light;
			static_cast<CPoint_Light*>(pLight_Comp)->Initialize(Point_Light, _pRenderer.GetLightMemoryManager());
			Light_Tag = "Point_Light.0" + i;
			pLight_Comp->SetCompName(Light_Tag);
			pLight_Comp->SetCompType(eCompTypes::ePOINT_LIGHT);
			pLight_Comp->SetOwner(Test_Model);
			Point_Light_Comps.push_back(static_cast<CPoint_Light*>(pLight_Comp));
			pLight_Comp = nullptr;

			Point_Light.fLight_Col = Dwarf_Util::RandomColor();
			Point_Light.fLight_Pos.y -= 5.0f;
			Point_Light.fRadius = 15.0f * Dwarf_Util::UnsignedRandFloat() + 15.0f;
			Point_Light_List.push_back(Point_Light);

			pLight_Comp = new CPoint_Light;
			static_cast<CPoint_Light*>(pLight_Comp)->Initialize(Point_Light, _pRenderer.GetLightMemoryManager());
			Light_Tag = "Point_Light.0" + (i + 1);
			pLight_Comp->SetCompName(Light_Tag);
			pLight_Comp->SetCompType(eCompTypes::ePOINT_LIGHT);
			pLight_Comp->SetOwner(Test_Model);
			Point_Light_Comps.push_back(static_cast<CPoint_Light*>(pLight_Comp));
			pLight_Comp = nullptr;

			//Cone Light Loading
			Cone_Light.fLight_Col = Dwarf_Util::RandomColorConstrained(0.75F,1.0F);
			Cone_Light.fLight_Pos = { vec_pos.x - 20.0F, vec_pos.y + 20.0F, vec_pos.z - 20.0F, 1.0f };
			{
				XMVECTOR source = XMLoadFloat4(&Cone_Light.fLight_Pos);
				XMVECTOR dest = XMLoadFloat4(&vec_pos);
				source.m128_f32[3] = 0.0F;
				dest.m128_f32[3] = 0.0F;
				XMStoreFloat3(&Cone_Light.fLight_Dir,XMVector3Normalize(dest - source));
			}
			Cone_Light.fCone_Ratios = { Dwarf_Util::SignedRandFloat(0.95f, 0.9f), Dwarf_Util::SignedRandFloat(0.85f, 0.75f) };
			Cone_Light.fRadius = 10.0F;
			Cone_Light.uActive = ACTIVE;
			Cone_Light_List.push_back(Cone_Light);

			pLight_Comp = new CCone_Light;
			static_cast<CCone_Light*>(pLight_Comp)->Initialize(Cone_Light, _pRenderer.GetLightMemoryManager());
			Light_Tag = "Cone_Light.0" + i;
			pLight_Comp->SetCompName(Light_Tag);
			pLight_Comp->SetCompType(eCompTypes::eCONE_LIGHT);
			pLight_Comp->SetOwner(Test_Model);
			Cone_Light_Comps.push_back(static_cast<CCone_Light*>(pLight_Comp));
			pLight_Comp = nullptr;

			Cone_Light.fLight_Col = Dwarf_Util::RandomColorConstrained(0.75F, 1.0F);
			Cone_Light.fLight_Pos = { vec_pos.x + 20.0F, vec_pos.y - 20.0F, vec_pos.z + 20.0F, 1.0f };
			{
				XMVECTOR source = XMLoadFloat4(&Cone_Light.fLight_Pos);
				XMVECTOR dest = XMLoadFloat4(&vec_pos);
				source.m128_f32[3] = 0.0F;
				dest.m128_f32[3] = 0.0F;
				XMStoreFloat3(&Cone_Light.fLight_Dir, XMVector3Normalize(dest - source));
			}
			Cone_Light.fCone_Ratios = { Dwarf_Util::SignedRandFloat(0.95f, 0.9f), Dwarf_Util::SignedRandFloat(0.85f, 0.75f) };
			Cone_Light_List.push_back(Cone_Light);

			pLight_Comp = new CCone_Light;
			static_cast<CCone_Light*>(pLight_Comp)->Initialize(Cone_Light, _pRenderer.GetLightMemoryManager());
			Light_Tag = "Cone_Light.0" + (i + 1);
			pLight_Comp->SetCompName(Light_Tag);
			pLight_Comp->SetCompType(eCompTypes::eCONE_LIGHT);
			pLight_Comp->SetOwner(Test_Model);
			Cone_Light_Comps.push_back(static_cast<CCone_Light*>(pLight_Comp));
			pLight_Comp = nullptr;
		}

	}

	//x_avg /= 1024.0f;
	//z_avg /= 1024.0f;

	//Initialize the component and add it to the game object
	render_comp->Initiailze(&data, &test_batch);
	XMStoreFloat4x4(&Test_Model->GetWorldMat(), XMMatrixTranslation(-5.0f, 0, 0));
	Test_Model->AddComponent(render_comp);
	Test_Model->GetObjectType() = eObjTypes::eDEF_OBJ;

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

	//Now that we have our list of lights lets store them in light memory manager.
	_pRenderer.GetLightMemoryManager()->LoadLightData(Point_Light_Comps, Cone_Light_Comps, Dir_Light_Comps, Point_Light_List, Cone_Light_List, Dir_Light_List);
}