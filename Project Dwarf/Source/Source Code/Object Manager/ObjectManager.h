#pragma once

//STL Includes
#include <vector>

//Header Includes

//Name Spaces that are being used
using namespace std;

//Forward declared Classes
class CGameObject;

class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();

	bool Initialize();
	void Shutdown();

	//Function Calls
	void Update();
	bool AddObject(CGameObject* _obj);
	//Function Call to Sort the Render List
	void RenderSort();

	//Accessors
	vector<CGameObject*>& GetRenderList();

private:

	//Lists of Game Objects
	vector<CGameObject *> m_vObjectList;

	//Specific lists
	///Animated Objects

	///Physics Based Objects

	///Particle Objects

	///Rendered Objects
	vector<CGameObject *> m_vRenderList;

	//Private Functions

};

