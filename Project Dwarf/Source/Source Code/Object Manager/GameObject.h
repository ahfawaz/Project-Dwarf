#pragma once

//STL Includes
#include <vector>
#include <string>
#include <DirectXMath.h>

//Header Includes

//Name Spaces that are being used
using namespace std;
using namespace DirectX;

//Forward declared Classes
class CComponent;

//Component Types
//eDEFAULT == Unset Component
//eMAIN_COMP == The main Components that know about the other components
//All other types are the components that make up the gameobject.
enum eCompTypes { eDEFAULT, eMAIN_COMP, eRENDER_COMP, eNUM_OF_COMP};
enum eObjTypes { eDEF_OBJ, eTEST_OBJ };

#define MAIN_COMP 0

class CGameObject
{
	//Members
	vector<CComponent*> m_vCompList;
	string m_sTag;
	eObjTypes m_eType;
	XMFLOAT4X4 m_fLocalMat;
	XMFLOAT4X4 m_fWorldMat;
	bool m_bActive;

public:
	CGameObject();
	~CGameObject();

	void UpdateComponents();

	///Accessors and Mutators
	//Returns the main component that knows all 
	//its other components associated with it.
	CComponent* GetMainComponent() const;

	//Run through the list of components and return the Component in question;
	CComponent* GetComponentByType(eCompTypes _type);

	XMFLOAT4X4& GetLocalMat();
	XMFLOAT4X4& GetWorldMat();

	string& GetTag();

	eObjTypes& GetObjectType();

	bool IsActive() const;
	void SetActive(bool _active);

	//Helper Funcs
	bool AddComponent(CComponent * _comp);

};

