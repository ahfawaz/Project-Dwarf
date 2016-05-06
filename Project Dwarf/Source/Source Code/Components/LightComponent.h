#pragma once

//STL Includes
#include <DirectXMath.h>
#include <vector>

//Header Includes
#include "Component.h"
//#include "../Renderer/Shaders/LightBuffers.h"

//Namespaces
using namespace DirectX;

//Forward Includes
class CLightMemoryManager;

//Light Structs
struct tPoint_Light
{
	XMFLOAT4	fLight_Pos;
	XMFLOAT3	fLight_Col;
	float	fRadius;
	UINT	uActive;
};

struct tCone_Light
{
	XMFLOAT4	fLight_Pos;
	XMFLOAT3	fLight_Dir;
	float	fRadius;
	XMFLOAT3	fLight_Col;
	UINT	uActive;
	XMFLOAT2	fCone_Ratios;
};

struct tDirectional_Light
{
	XMFLOAT4	fLight_Pos;
	XMFLOAT3	fLight_Dir;
	UINT	uActive;
	XMFLOAT3	fLight_Col;
};

struct tAmbient_Light
{
	XMFLOAT3 fLight_Col;
};

class CPoint_Light : public CComponent
{

	tPoint_Light* m_tCurrent;
	tPoint_Light* m_tNext;

	unsigned char * m_pRenderLightAddress;

	CLightMemoryManager* _pLightMM;

	//Time data so each peice can be controlled independantly.
	float	m_fPosTime; //Holds the time for the dynamic positioning.
	float	m_fColTime;	//Holds the time for the dynamic color change.
	float	m_fRadTime; //Holds the time for the dynamic radius change.

	float	m_fPosTick; //Holds the elapsed time for the dynamic positioning.
	float	m_fColTick;	//Holds the elapsed time for the dynamic color change.
	float	m_fRadTick; //Holds the elapsed time for the dynamic radius change.


public:

	CPoint_Light();
	~CPoint_Light();
	CPoint_Light(CPoint_Light& cpy);
	CPoint_Light& operator=(CPoint_Light& assign);


	bool Initialize(XMFLOAT4 _pos, XMFLOAT3 _color, float _radius, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(XMFLOAT4 _pos, XMFLOAT4 _color, float _radius, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(tPoint_Light _light_data, CLightMemoryManager* _LightMemoryManager);
	void Shutdown();

	void UpdateComponent();

	tPoint_Light GetLightData() const;

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT3 _color, float _time = 0.0F);

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT4 _color, float _time = 0.0F);

	//Change the position of the light
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void MoveLight(XMFLOAT4 _pos, float _time = 0.0F);

	//Change the size of the radius
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeRadius(float _radius, float _time = 0.0F);

	void ChangeActive(bool _active);
	void SwitchActive();

	void SetAddress(uint8_t ** _address) { m_pRenderLightAddress = *_address; }

};

class CCone_Light : public CComponent
{
	tCone_Light* m_tCurrent;
	tCone_Light* m_tNext; //Used for transitionsa
	
	unsigned char * m_pRenderLightAddress;

	CLightMemoryManager* _pLightMM;

	//Time data so each peice can be controlled independantly.
	float	m_fPosTime; //Holds the time for the dynamic positioning.
	float	m_fColTime;	//Holds the time for the dynamic color change.
	float	m_fRadTime; //Holds the time for the dynamic radius change.
	float	m_fDirTime; //Holds the time for the dynamic direction change.
	float	m_fRatioTime; //Holds the time for the dynamic cone ratio change.

	float	m_fPosTick; //Holds the elapsed time for the dynamic positioning.
	float	m_fColTick;	//Holds the elapsed time for the dynamic color change.
	float	m_fRadTick; //Holds the elapsed time for the dynamic radius change.
	float	m_fDirTick; //Holds the elapsed time for the dynamic direction change.
	float	m_fRatioTick; //Holds the elapsed time for the dynamic cone ratio change.

public:

	CCone_Light();
	~CCone_Light();
	CCone_Light(CCone_Light& cpy);
	CCone_Light& operator=(CCone_Light& assign);

	bool Initialize(XMFLOAT4 _pos, XMFLOAT3 _color, XMFLOAT3 _dir, float _radius, float _inner_ratio, float _outer_ratio, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(XMFLOAT4 _pos, XMFLOAT4 _color, XMFLOAT3 _dir, float _radius, float _inner_ratio, float _outer_ratio, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(tCone_Light _light_data, CLightMemoryManager* _LightMemoryManager);
	void Shutdown();

	void UpdateComponent();

	tCone_Light GetLightData() const;

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT3 _color, float _time = 0.0F);

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT4 _color, float _time = 0.0F);

	//Change the position of the light
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void MoveLight(XMFLOAT4 _pos, float _time = 0.0F);

	//Change the size of the radius
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeRadius(float _radius, float _time = 0.0F);

	//Change the direction of the light
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeDirection(XMFLOAT3 _dir, float _time = 0.0F);

	//Change the Cone's inner and outer ratios
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeConeRatios(float _inner, float _outer, float _time = 0.0F);

	//Change the Cone's inner and outer ratios
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	//Input: _ratios.x == inner ratio | _ratios.y == outer ratio
	void ChangeConeRatios(XMFLOAT2 _ratios, float _time = 0.0F);

	void ChangeActive(bool _active);
	void SwitchActive();

	void SetAddress(uint8_t ** _address) { m_pRenderLightAddress = *_address; }


};

class CDirectional_Light : public CComponent
{

	tDirectional_Light* m_tCurrent;
	tDirectional_Light* m_tNext;

	unsigned char * m_pRenderLightAddress;

	CLightMemoryManager* _pLightMM;

	//Time data so each peice can be controlled independantly.
	float	m_fPosTime; //Holds the time for the dynamic positioning.
	float	m_fColTime;	//Holds the time for the dynamic color change.
	float	m_fDirTime; //Holds the time for the dynamic direction change.

	float	m_fPosTick; //Holds the elapsed time for the dynamic positioning.
	float	m_fColTick;	//Holds the elapsed time for the dynamic color change.
	float	m_fDirTick; //Holds the elapsed time for the dynamic direction change.
public:

	CDirectional_Light();
	~CDirectional_Light();
	CDirectional_Light(CDirectional_Light& cpy);
	CDirectional_Light& operator=(CDirectional_Light& assign);

	bool Initialize(XMFLOAT4 _pos, XMFLOAT3 _dir, XMFLOAT3 _color, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(XMFLOAT4 _pos, XMFLOAT3 _dir, XMFLOAT4 _color, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(tDirectional_Light _light_data, CLightMemoryManager* _LightMemoryManager);
	void Shutdown();

	void UpdateComponent();

	tDirectional_Light GetLightData() const;

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT3 _color, float _time = 0.0F);

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT4 _color, float _time = 0.0F);

	//Change the position of the light
	//If time is > 0 the change will happen over time.
	//IF time is <= 0 the change will happen immediately
	void MoveLight(XMFLOAT4 _pos, float _time = 0.0F);

	//Change the direction of the light
	//IF time is <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeDirection(XMFLOAT3 _dir, float _time = 0.0F);

	void ChangeActive(bool _active);
	void SwitchActive();

	void SetAddress(uint8_t ** _address) { m_pRenderLightAddress = *_address; }
};

class CAmbient_Light : public CComponent
{
	tAmbient_Light* m_tCurrent;
	tAmbient_Light* m_tNext;

	unsigned char * m_pRenderLightAddress;

	CLightMemoryManager* _pLightMM;

	//Time data so each peice can be controlled independantly.
	float	m_fColTime;	//Holds the time for the dynamic color change.
	float	m_fColTick;	//Holds the elapsed time for the dynamic color change.

public:

	CAmbient_Light();
	~CAmbient_Light();
	CAmbient_Light(CAmbient_Light& cpy);
	CAmbient_Light& operator=(CAmbient_Light& assign);

	///Currently _active does not do anything. Will be changed in the future
	bool Initialize(XMFLOAT3 _color, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	bool Initialize(XMFLOAT4 _color, CLightMemoryManager* _LightMemoryManager, UINT _active = 1U);
	void Shutdown();

	void UpdateComponent();

	tAmbient_Light GetLightData() const;

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT3 _color, float _time = 0.0F);

	//Change the color of the light
	//If time is  <= 0 the change will happen immediately
	//If time is > 0 the change will happen over time.
	void ChangeColors(XMFLOAT4 _color, float _time = 0.0F);
};

