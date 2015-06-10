#pragma once

//Main Component needed
#include "Component.h"

//STL Includes

//Header Includes

//Name Spaces that are being used

//Forward declared Classes
class CRenderComponent;

class TestComponent : public CComponent
{
	//All the Components that are associated with this Object
	CRenderComponent * m_pGORenderComp;
	XMFLOAT4X4 m_fPosMatrix;
	XMFLOAT4X4 m_fRotMatrix;

public:
	TestComponent();
	~TestComponent();

	virtual void UpdateComponent();
	virtual bool Initialize();
	virtual void Shutdown();

	//Accessors
	CRenderComponent* GetRenderComp() const;

	//Mutators
	void SetRenderComp(CRenderComponent* _comp);
};

