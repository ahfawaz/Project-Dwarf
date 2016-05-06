#include "../Application/stdafx.h"

//STL Includes

//Header Includes
#include "LightComponent.h"
#include "../Utilities/TimeManager.h"
#include "../Renderer/LightMemoryManager.h"

//Namespaces used

//Defines used
#define CLAMP(x) ((x > 1.0F) ? 1.0F : ((x < 0.0F) ? 0.0F : x))
#define ZERO_TIME 0.0f
#define LERP_TIME_RATIO(time, tick) (tick / time)

#pragma region POINT LIGHT
CPoint_Light::CPoint_Light() :	m_fColTime(ZERO_TIME), m_fPosTime(ZERO_TIME), m_fRadTime(ZERO_TIME), 
								m_fColTick(ZERO_TIME), m_fPosTick(ZERO_TIME), m_fRadTick(ZERO_TIME),
								_pLightMM(nullptr), m_tCurrent(nullptr), m_tNext(nullptr)
{

}

CPoint_Light::~CPoint_Light()
{
	Shutdown();
}

CPoint_Light::CPoint_Light(CPoint_Light& cpy)
{
	*this = cpy;
}

CPoint_Light& CPoint_Light::operator=(CPoint_Light& assign)
{
	if (m_tCurrent)
		delete m_tCurrent;
	if (m_tNext)
		delete m_tNext;

	m_tCurrent = new tPoint_Light;
	m_tCurrent->fLight_Col = assign.m_tCurrent->fLight_Col;
	m_tCurrent->fLight_Pos = assign.m_tCurrent->fLight_Pos;
	m_tCurrent->fRadius = assign.m_tCurrent->fRadius;
	m_tCurrent->uActive = assign.m_tCurrent->uActive;

	m_tNext = new tPoint_Light;
	ZeroMemory(m_tNext, sizeof(tPoint_Light));

	this->m_fColTick = assign.m_fColTick;
	this->m_fColTime = assign.m_fColTime;
	this->m_fPosTick = assign.m_fPosTick;
	this->m_fPosTime = assign.m_fPosTime;
	this->m_fRadTick = assign.m_fRadTick;
	
	return *this;
}

bool CPoint_Light::Initialize(XMFLOAT4 _pos, XMFLOAT3 _color, float _radius, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	tPoint_Light light_data;
	light_data.fLight_Pos = _pos;
	light_data.fLight_Col = _color;
	light_data.fRadius = _radius;
	light_data.uActive = _active;
	return Initialize(light_data, _LightMemoryManager);

}

bool CPoint_Light::Initialize(XMFLOAT4 _pos, XMFLOAT4 _color, float _radius, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	tPoint_Light light_data;
	light_data.fLight_Pos = _pos;
	light_data.fLight_Col = { _color.x, _color.y, _color.z };
	light_data.fRadius = _radius;
	light_data.uActive = _active;
	return Initialize(light_data, _LightMemoryManager);
}

bool CPoint_Light::Initialize(tPoint_Light _light_data, CLightMemoryManager* _LightMemoryManager)
{

	if (!_LightMemoryManager)
		return false;

	_pLightMM = _LightMemoryManager;
	m_tCurrent = new tPoint_Light;
	m_tNext = new tPoint_Light;
	ZeroMemory(m_tCurrent, sizeof(tPoint_Light));
	ZeroMemory(m_tNext, sizeof(tPoint_Light));

	m_tCurrent->fLight_Col = _light_data.fLight_Col;
	m_tCurrent->fLight_Pos = _light_data.fLight_Pos;
	m_tCurrent->fRadius = _light_data.fRadius;
	m_tCurrent->uActive = _light_data.uActive;
	return true;
}

void CPoint_Light::Shutdown()
{
	if (m_tCurrent)
		delete m_tCurrent;
	if (m_tNext)
		delete m_tNext;
}

void CPoint_Light::UpdateComponent()
{
	bool Light_Value_Changed = false;
	tPoint_Light temp_light = *m_tCurrent;
	XMVECTOR temp_vector_curr, temp_vector_next, temp_vector_final;
	float lerp_ratio = 0.0F;
	float delta_time = CTimeManager::GetTimeDelta();

	if (m_fColTime > ZERO_TIME)
	{
		m_fColTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fColTime, m_fColTick));

		temp_vector_curr = XMLoadFloat3(&m_tCurrent->fLight_Col);
		temp_vector_next = XMLoadFloat3(&m_tNext->fLight_Col);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fColTick > m_fColTime)
		{
			m_fColTime = ZERO_TIME;
			m_tCurrent->fLight_Col = m_tNext->fLight_Col;
		}

		XMStoreFloat3(&temp_light.fLight_Col, temp_vector_final);
		Light_Value_Changed = true;
	}

	if (m_fPosTime > ZERO_TIME)
	{
		m_fPosTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fPosTime, m_fPosTick));

		temp_vector_curr = XMLoadFloat4(&m_tCurrent->fLight_Pos);
		temp_vector_next = XMLoadFloat4(&m_tNext->fLight_Pos);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fPosTick > m_fPosTime)
		{
			m_fPosTime = ZERO_TIME;
			m_tCurrent->fLight_Pos = m_tNext->fLight_Pos;
		}

		XMStoreFloat4(&temp_light.fLight_Pos, temp_vector_final);
		Light_Value_Changed = true;
	}

	if (m_fRadTime > ZERO_TIME)
	{
		m_fRadTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fPosTime, m_fPosTick));

		temp_light.fRadius = m_tCurrent->fRadius + (m_tNext->fRadius - m_tCurrent->fRadius) * lerp_ratio;

		if (m_fRadTick > m_fRadTime)
		{
			m_fRadTime = ZERO_TIME;
			m_tCurrent->fRadius = m_tNext->fRadius;
		}

		Light_Value_Changed = true;
	}

	//Got to let the LightManager know things have changed.
	if (Light_Value_Changed)
	{
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &temp_light);
	}
}

tPoint_Light CPoint_Light::GetLightData() const
{
	tPoint_Light point_light_data_return;
	_pLightMM->GetDataFromBuffer(m_pRenderLightAddress, this->GetCompType(), &point_light_data_return);

	return point_light_data_return;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CPoint_Light::ChangeColors(XMFLOAT3 _color, float _time)
{

	if (_time <= ZERO_TIME)
	{
		m_tCurrent->fLight_Col = _color;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Col = _color;
	m_fColTime = _time;
	m_fColTick = 0.0F;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CPoint_Light::ChangeColors(XMFLOAT4 _color, float _time)
{
	ChangeColors({ _color.x, _color.y, _color.z }, _time);
}

//Change the position of the light
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CPoint_Light::MoveLight(XMFLOAT4 _pos, float _time)
{
	if (_time <= ZERO_TIME)
	{
		m_tCurrent->fLight_Pos = _pos;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Pos = _pos;
	m_fPosTime = _time;
	m_fPosTick = 0.0F;
}

//Change the size of the radius
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CPoint_Light::ChangeRadius(float _radius, float _time)
{
	if (_time <= ZERO_TIME)
	{
		m_tCurrent->fRadius = _radius;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fRadius = _radius;
	m_fRadTime = _time;
	m_fRadTick = 0.0F;
}
#pragma endregion

#pragma region CONE LIGHT
CCone_Light::CCone_Light() :	m_fColTime(ZERO_TIME), m_fPosTime(ZERO_TIME), m_fRadTime(ZERO_TIME), m_fDirTime(ZERO_TIME), m_fRatioTime(ZERO_TIME),
								m_fColTick(ZERO_TIME), m_fPosTick(ZERO_TIME), m_fRadTick(ZERO_TIME), m_fDirTick(ZERO_TIME), m_fRatioTick(ZERO_TIME),
								_pLightMM(nullptr), m_tCurrent(nullptr), m_tNext(nullptr)
{
}

CCone_Light::~CCone_Light()
{
	Shutdown();
}

CCone_Light::CCone_Light(CCone_Light& cpy)
{
	*this = cpy;
}

CCone_Light& CCone_Light::operator=(CCone_Light& assign)
{
	if (m_tCurrent || m_tNext)
		Shutdown();

	m_tCurrent = new tCone_Light;
	m_tCurrent->fLight_Col = assign.m_tCurrent->fLight_Col;
	m_tCurrent->fLight_Pos = assign.m_tCurrent->fLight_Pos;
	m_tCurrent->fRadius = assign.m_tCurrent->fRadius;
	m_tCurrent->fLight_Dir = assign.m_tCurrent->fLight_Dir;
	m_tCurrent->fCone_Ratios = assign.m_tCurrent->fCone_Ratios;
	m_tCurrent->uActive = assign.m_tCurrent->uActive;

	m_tNext = new tCone_Light;
	ZeroMemory(m_tNext, sizeof(tCone_Light));

	this->m_fColTick = assign.m_fColTick;
	this->m_fColTime = assign.m_fColTime;
	this->m_fPosTick = assign.m_fPosTick;
	this->m_fPosTime = assign.m_fPosTime;
	this->m_fRadTick = assign.m_fRadTick;
	this->m_fRadTime = assign.m_fRadTime;
	this->m_fDirTick = assign.m_fDirTick;
	this->m_fDirTime = assign.m_fDirTime;
	this->m_fRatioTick = assign.m_fRatioTick;
	this->m_fRatioTime = assign.m_fRatioTime;

	return *this;
}

bool CCone_Light::Initialize(XMFLOAT4 _pos, XMFLOAT3 _color, XMFLOAT3 _dir, float _radius, float _inner_ratio, float _outer_ratio, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	tCone_Light light_data;
	light_data.fCone_Ratios = { _inner_ratio, _outer_ratio };
	light_data.fLight_Col = { _color.x, _color.y, _color.z };
	light_data.fLight_Dir = _dir;
	light_data.fLight_Pos = _pos;
	light_data.fRadius = _radius;
	light_data.uActive = _active;
	return Initialize(light_data, _LightMemoryManager);
}

bool CCone_Light::Initialize(XMFLOAT4 _pos, XMFLOAT4 _color, XMFLOAT3 _dir, float _radius, float _inner_ratio, float _outer_ratio, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	tCone_Light light_data;
	light_data.fCone_Ratios = { _inner_ratio, _outer_ratio };
	light_data.fLight_Col = { _color.x, _color.y, _color.z };
	light_data.fLight_Dir = _dir;
	light_data.fLight_Pos = _pos;
	light_data.fRadius = _radius;
	light_data.uActive = _active;
	return Initialize(light_data, _LightMemoryManager);
}

bool CCone_Light::Initialize(tCone_Light _light_data, CLightMemoryManager* _LightMemoryManager)
{
	if (!_LightMemoryManager)
		return false;

	_pLightMM = _LightMemoryManager;
	m_tCurrent = new tCone_Light;
	m_tNext = new tCone_Light;
	ZeroMemory(m_tCurrent, sizeof(tCone_Light));
	ZeroMemory(m_tNext, sizeof(tCone_Light));

	m_tCurrent->fLight_Col = _light_data.fLight_Col;
	m_tCurrent->fLight_Dir = _light_data.fLight_Dir;
	m_tCurrent->fLight_Pos = _light_data.fLight_Pos;
	m_tCurrent->fRadius = _light_data.fRadius;
	m_tCurrent->fCone_Ratios = _light_data.fCone_Ratios;
	m_tCurrent->uActive = _light_data.uActive;

	return true;
}

void CCone_Light::Shutdown()
{
	if (m_tCurrent)
		delete m_tCurrent;
	if (m_tNext)
		delete m_tNext;
}

void CCone_Light::UpdateComponent()
{
	bool Light_Value_Changed = false;
	tCone_Light temp_light = *m_tCurrent;
	XMVECTOR temp_vector_curr, temp_vector_next, temp_vector_final;
	float lerp_ratio = 0.0F;
	float delta_time = CTimeManager::GetTimeDelta();

	if (m_fColTime > ZERO_TIME)
	{
		m_fColTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fColTime, m_fColTick));

		temp_vector_curr = XMLoadFloat3(&m_tCurrent->fLight_Col);
		temp_vector_next = XMLoadFloat3(&m_tNext->fLight_Col);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fColTick > m_fColTime)
		{
			m_fColTime = ZERO_TIME;
			m_tCurrent->fLight_Col = m_tNext->fLight_Col;
		}

		XMStoreFloat3(&temp_light.fLight_Col, temp_vector_final);
		Light_Value_Changed = true;
	}

	if (m_fPosTime > ZERO_TIME)
	{
		m_fPosTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fPosTime, m_fPosTick));

		temp_vector_curr = XMLoadFloat4(&m_tCurrent->fLight_Pos);
		temp_vector_next = XMLoadFloat4(&m_tNext->fLight_Pos);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fPosTick > m_fPosTime)
		{
			m_fPosTime = ZERO_TIME;
			m_tCurrent->fLight_Pos = m_tNext->fLight_Pos;
		}

		XMStoreFloat4(&temp_light.fLight_Pos, temp_vector_final);
		Light_Value_Changed = true;
	}

	if (m_fDirTime > ZERO_TIME)
	{
		m_fDirTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fDirTime, m_fDirTick));

		XMVECTOR pos = XMLoadFloat4(&m_tCurrent->fLight_Pos), up = XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);
		XMMATRIX curr_mat = XMMatrixLookToLH(pos, XMLoadFloat3(&m_tCurrent->fLight_Dir), up);
		XMMATRIX next_mat = XMMatrixLookToLH(pos, XMLoadFloat3(&m_tNext->fLight_Dir), up);
		XMVECTOR curr_scale, curr_rot, curr_trans;
		XMVECTOR next_scale, next_rot, next_trans;
		XMMatrixDecompose(&curr_scale, &curr_rot, &curr_trans, curr_mat);
		XMMatrixDecompose(&next_scale, &next_rot, &next_trans, next_mat);
		curr_scale = XMVectorLerp(curr_scale, next_scale, lerp_ratio);
		curr_trans = XMVectorLerp(curr_trans, next_trans, lerp_ratio);
		curr_rot = XMQuaternionSlerp(curr_rot, next_rot, lerp_ratio);
		curr_mat = XMMatrixAffineTransformation(curr_scale, pos, curr_rot, curr_trans);

		if (m_fDirTick > m_fDirTime)
		{
			m_fDirTime = ZERO_TIME;
			m_tCurrent->fLight_Dir = m_tNext->fLight_Dir;
		}

		curr_mat = XMMatrixInverse(NULL, curr_mat);
		XMStoreFloat3(&temp_light.fLight_Dir, curr_mat.r[2]);
		Light_Value_Changed = true;

	}

	if (m_fRadTime > ZERO_TIME)
	{
		m_fRadTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fPosTime, m_fPosTick));

		temp_light.fRadius = m_tCurrent->fRadius + (m_tNext->fRadius - m_tCurrent->fRadius) * lerp_ratio;

		if (m_fRadTick > m_fRadTime)
		{
			m_fRadTime = ZERO_TIME;
			m_tCurrent->fRadius = m_tNext->fRadius;
		}

		Light_Value_Changed = true;
	}

	if (m_fRatioTime > m_fRatioTick)
	{
		m_fRatioTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fRatioTime, m_fRatioTick));

		temp_vector_curr = XMLoadFloat(&m_tCurrent->fCone_Ratios.x);
		temp_vector_next = XMLoadFloat(&m_tNext->fCone_Ratios.x);
		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		XMStoreFloat(&temp_light.fCone_Ratios.x, temp_vector_final);

		temp_vector_curr = XMLoadFloat(&m_tCurrent->fCone_Ratios.y);
		temp_vector_next = XMLoadFloat(&m_tNext->fCone_Ratios.y);
		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		XMStoreFloat(&temp_light.fCone_Ratios.y, temp_vector_final);

		if (m_fRatioTick > m_fRatioTime)
		{
			m_fRatioTime = ZERO_TIME;
			m_tCurrent->fCone_Ratios = m_tNext->fCone_Ratios;
		}
		Light_Value_Changed = true;
	}

	//Got to let the LightManager know things have changed.
	if (Light_Value_Changed)
	{
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &temp_light);
	}
}

tCone_Light CCone_Light::GetLightData() const
{
	tCone_Light light_data_return;
	_pLightMM->GetDataFromBuffer(m_pRenderLightAddress, this->GetCompType(), &light_data_return);

	return light_data_return;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CCone_Light::ChangeColors(XMFLOAT3 _color, float _time)
{
	if (_time < 0.0F)
	{
		m_tNext->fLight_Col = _color;
		return;
	}

	m_tNext->fLight_Col = _color;
	m_fColTime = _time;
	m_fColTick = 0.0F;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CCone_Light::ChangeColors(XMFLOAT4 _color, float _time)
{
	ChangeColors({ _color.x, _color.y, _color.z }, _time);
}

//Change the position of the light
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CCone_Light::MoveLight(XMFLOAT4 _pos, float _time)
{
	if (_time <= 0.0F)
	{
		m_tCurrent->fLight_Pos = _pos;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Pos = _pos;
	m_fPosTime = _time;
	m_fPosTick = 0.0F;
}

//Change the size of the radius
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CCone_Light::ChangeRadius(float _radius, float _time)
{
	if (_time <= 0.0F)
	{
		m_tCurrent->fRadius = _radius;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fRadius = _radius;
	m_fRadTime = _time;
	m_fRadTick = 0.0F;
}

//Change the direction of the light
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CCone_Light::ChangeDirection(XMFLOAT3 _dir, float _time)
{
	XMStoreFloat3(&_dir, XMVector3Normalize(XMVectorSet(_dir.x, _dir.y, _dir.z, 0.0F)));

	if (_time <= 0.0F)
	{
		m_tCurrent->fLight_Dir = _dir;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Dir = _dir;
	m_fDirTime = _time;
	m_fDirTick = 0.0F;
}

//Change the Cone's inner and outer ratios
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CCone_Light::ChangeConeRatios(float _inner, float _outer, float _time)
{
	ChangeConeRatios({ _inner, _outer }, _time);
}

//Change the Cone's inner and outer ratios
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
//Input: _ratios.x == inner ratio | _ratios.y == outer ratio
void CCone_Light::ChangeConeRatios(XMFLOAT2 _ratios, float _time)
{
	if (_time <= 0.0F)
	{
		m_tCurrent->fCone_Ratios = _ratios;
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fCone_Ratios = _ratios;
	m_fRatioTime = _time;
	m_fRatioTick = 0.0F;
}
#pragma endregion

#pragma region DIRECTIONAL LIGHT
CDirectional_Light::CDirectional_Light() :	m_fColTime(ZERO_TIME), m_fPosTime(ZERO_TIME), m_fDirTime(ZERO_TIME),
											m_fColTick(ZERO_TIME), m_fPosTick(ZERO_TIME), m_fDirTick(ZERO_TIME),
											_pLightMM(nullptr), m_tCurrent(nullptr), m_tNext(nullptr)
{

}

CDirectional_Light::~CDirectional_Light()
{
	Shutdown();
}

CDirectional_Light::CDirectional_Light(CDirectional_Light& cpy)
{
	*this = cpy;
}

CDirectional_Light& CDirectional_Light::operator=(CDirectional_Light& assign)
{
	if (m_tCurrent || m_tNext)
		Shutdown();

	m_tCurrent = new tDirectional_Light;
	m_tCurrent->fLight_Col = assign.m_tCurrent->fLight_Col;
	m_tCurrent->fLight_Pos = assign.m_tCurrent->fLight_Pos;
	m_tCurrent->fLight_Dir = assign.m_tCurrent->fLight_Dir;
	m_tCurrent->uActive = assign.m_tCurrent->uActive;

	m_tNext = new tDirectional_Light;
	ZeroMemory(m_tNext, sizeof(tDirectional_Light));

	this->m_fColTick = assign.m_fColTick;
	this->m_fColTime = assign.m_fColTime;
	this->m_fPosTick = assign.m_fPosTick;
	this->m_fPosTime = assign.m_fPosTime;
	this->m_fDirTick = assign.m_fDirTick;
	this->m_fDirTime = assign.m_fDirTime;

	return *this;
}

bool CDirectional_Light::Initialize(XMFLOAT4 _pos, XMFLOAT3 _dir, XMFLOAT3 _color, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	tDirectional_Light light_data;
	light_data.fLight_Col = { _color.x, _color.y, _color.z };
	light_data.fLight_Dir = _dir;
	light_data.fLight_Pos = _pos;
	light_data.uActive = _active;
	return Initialize(light_data, _LightMemoryManager);
}
bool CDirectional_Light::Initialize(XMFLOAT4 _pos, XMFLOAT3 _dir, XMFLOAT4 _color, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	tDirectional_Light light_data;
	light_data.fLight_Col = { _color.x, _color.y, _color.z };
	light_data.fLight_Dir = _dir;
	light_data.fLight_Pos = _pos;
	light_data.uActive = _active;
	return Initialize(light_data, _LightMemoryManager);
}

bool CDirectional_Light::Initialize(tDirectional_Light _light_data, CLightMemoryManager* _LightMemoryManager)
{
	if (!_LightMemoryManager)
		return false;

	_pLightMM = _LightMemoryManager;
	m_tCurrent = new tDirectional_Light;
	m_tNext = new tDirectional_Light;
	ZeroMemory(m_tCurrent, sizeof(tDirectional_Light));
	ZeroMemory(m_tNext, sizeof(tDirectional_Light));

	m_tCurrent->fLight_Col = _light_data.fLight_Col;
	m_tCurrent->fLight_Dir = _light_data.fLight_Dir;
	m_tCurrent->fLight_Pos = _light_data.fLight_Pos;
	m_tCurrent->uActive = _light_data.uActive;

	return true;
}

void CDirectional_Light::Shutdown()
{
	if (m_tCurrent)
		delete m_tCurrent;
	if (m_tNext)
		delete m_tNext;
}

void CDirectional_Light::UpdateComponent()
{
	bool Light_Value_Changed = false;
	tDirectional_Light temp_light = *m_tCurrent;
	XMVECTOR temp_vector_curr, temp_vector_next, temp_vector_final;
	float lerp_ratio = 0.0F;
	float delta_time = CTimeManager::GetTimeDelta();

	if (m_fColTime > ZERO_TIME)
	{
		m_fColTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fColTime, m_fColTick));

		temp_vector_curr = XMLoadFloat3(&m_tCurrent->fLight_Col);
		temp_vector_next = XMLoadFloat3(&m_tNext->fLight_Col);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fColTick > m_fColTime)
		{
			m_fColTime = ZERO_TIME;
			m_tCurrent->fLight_Col = m_tNext->fLight_Col;
		}

		XMStoreFloat3(&temp_light.fLight_Col, temp_vector_final);
		Light_Value_Changed = true;
	}

	if (m_fPosTime > ZERO_TIME)
	{
		m_fPosTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fPosTime, m_fPosTick));

		temp_vector_curr = XMLoadFloat4(&m_tCurrent->fLight_Pos);
		temp_vector_next = XMLoadFloat4(&m_tNext->fLight_Pos);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fPosTick > m_fPosTime)
		{
			m_fPosTime = ZERO_TIME;
			m_tCurrent->fLight_Pos = m_tNext->fLight_Pos;
		}

		XMStoreFloat4(&temp_light.fLight_Pos, temp_vector_final);
		Light_Value_Changed = true;
	}

	if (m_fDirTime > ZERO_TIME)
	{
		m_fDirTick += delta_time;
		lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fDirTime, m_fDirTick));

		XMVECTOR pos = XMLoadFloat4(&m_tCurrent->fLight_Pos), up = XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);
		XMMATRIX curr_mat = XMMatrixLookToLH(pos, XMLoadFloat3(&m_tCurrent->fLight_Dir), up);
		XMMATRIX next_mat = XMMatrixLookToLH(pos, XMLoadFloat3(&m_tNext->fLight_Dir), up);
		XMVECTOR curr_scale, curr_rot, curr_trans;
		XMVECTOR next_scale, next_rot, next_trans;
		XMMatrixDecompose(&curr_scale, &curr_rot, &curr_trans, curr_mat);
		XMMatrixDecompose(&next_scale, &next_rot, &next_trans, next_mat);
		curr_scale = XMVectorLerp(curr_scale, next_scale, lerp_ratio);
		curr_trans = XMVectorLerp(curr_trans, next_trans, lerp_ratio);
		curr_rot = XMQuaternionSlerp(curr_rot, next_rot, lerp_ratio);
		curr_mat = XMMatrixAffineTransformation(curr_scale, pos, curr_rot, curr_trans);

		if (m_fDirTick > m_fDirTime)
		{
			m_fDirTime = ZERO_TIME;
			m_tCurrent->fLight_Dir = m_tNext->fLight_Dir;
		}

		curr_mat = XMMatrixInverse(NULL, curr_mat);
		XMStoreFloat3(&temp_light.fLight_Dir, curr_mat.r[2]);
		Light_Value_Changed = true;

	}

	//Got to let the LightManager know things have changed.
	if (Light_Value_Changed)
	{
		_pLightMM->SetDataInBuffer(this->m_pRenderLightAddress, this->GetCompType(), &temp_light);
	}

}

tDirectional_Light CDirectional_Light::GetLightData() const
{
	tDirectional_Light light_data_return;
	_pLightMM->GetDataFromBuffer(m_pRenderLightAddress, this->GetCompType(), &light_data_return);

	return light_data_return;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CDirectional_Light::ChangeColors(XMFLOAT3 _color, float _time)
{
	if (_time <= 0.0F)
	{
		m_tCurrent->fLight_Col = _color;
		_pLightMM->SetDataInBuffer(m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Col = _color;
	m_fColTime = _time;
	m_fColTick = 0.0F;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CDirectional_Light::ChangeColors(XMFLOAT4 _color, float _time)
{
	ChangeColors({ _color.x, _color.y, _color.z }, _time);

}

//Change the position of the light
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CDirectional_Light::MoveLight(XMFLOAT4 _pos, float _time)
{
	if (_time <= 0.0F)
	{
		m_tCurrent->fLight_Pos = _pos;
		_pLightMM->SetDataInBuffer(m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Pos = _pos;
	m_fPosTime = _time;
	m_fPosTick = 0.0F;
}

//Change the direction of the light
//IF time is <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CDirectional_Light::ChangeDirection(XMFLOAT3 _dir, float _time)
{
	XMStoreFloat3(&_dir, XMVector3Normalize(XMVectorSet(_dir.x, _dir.y, _dir.z, 0.0F)));

	if (_time <= 0.0F)
	{
		m_tCurrent->fLight_Dir = _dir;
		_pLightMM->SetDataInBuffer(m_pRenderLightAddress, this->GetCompType(), &m_tCurrent);
		return;
	}

	m_tNext->fLight_Dir = _dir;
	m_fDirTime = _time;
	m_fDirTick = 0.0F;
}
#pragma endregion

#pragma region AMBIENT LIGHT
CAmbient_Light::CAmbient_Light() : m_fColTime(ZERO_TIME), m_fColTick(ZERO_TIME), m_tCurrent(nullptr), m_tNext(nullptr)
{

}

CAmbient_Light::~CAmbient_Light()
{
	Shutdown();
}

CAmbient_Light::CAmbient_Light(CAmbient_Light& cpy)
{
	*this = cpy;
}

CAmbient_Light& CAmbient_Light::operator=(CAmbient_Light& assign)
{
	if (m_tNext || m_tCurrent)
		Shutdown();

	m_tCurrent = new tAmbient_Light;
	m_tCurrent->fLight_Col = assign.m_tCurrent->fLight_Col;

	return *this;
}

bool CAmbient_Light::Initialize(XMFLOAT3 _color, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	if (!_LightMemoryManager)
		return false;

	_pLightMM = _LightMemoryManager;
	m_tCurrent = new tAmbient_Light;
	m_tNext = new tAmbient_Light;
	ZeroMemory(m_tCurrent, sizeof(tAmbient_Light));
	ZeroMemory(m_tNext, sizeof(tAmbient_Light));

	m_tCurrent->fLight_Col = _color;

	return true;
}

bool CAmbient_Light::Initialize(XMFLOAT4 _color, CLightMemoryManager* _LightMemoryManager, UINT _active)
{
	return Initialize({ _color.x, _color.y, _color.z }, _LightMemoryManager, _active);
}

void CAmbient_Light::Shutdown() 
{
	if (m_tCurrent)
		delete m_tCurrent;
	if (m_tNext)
		delete m_tNext;
}

void CAmbient_Light::UpdateComponent()
{
	if (m_fColTime > ZERO_TIME)
	{
		tAmbient_Light temp_light = *m_tCurrent;
		XMVECTOR temp_vector_final;
		float lerp_ratio = CLAMP(LERP_TIME_RATIO(m_fColTime, m_fColTick));
		m_fColTick += CTimeManager::GetTimeDelta();

		XMVECTOR temp_vector_curr = XMLoadFloat3(&m_tCurrent->fLight_Col);
		XMVECTOR temp_vector_next = XMLoadFloat3(&m_tNext->fLight_Col);

		temp_vector_final = XMVectorLerp(temp_vector_curr, temp_vector_next, lerp_ratio);

		if (m_fColTick > m_fColTime)
		{
			m_fColTime = ZERO_TIME;
			m_tCurrent->fLight_Col = m_tNext->fLight_Col;
		}

		XMStoreFloat3(&temp_light.fLight_Col, temp_vector_final);
	}
}

tAmbient_Light CAmbient_Light::GetLightData() const
{
	tAmbient_Light light_data_return;
	_pLightMM->GetDataFromBuffer(m_pRenderLightAddress, this->GetCompType(), &light_data_return);

	return light_data_return;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CAmbient_Light::ChangeColors(XMFLOAT3 _color, float _time)
{
	if (_time <= 0.0F)
	{
		m_tCurrent->fLight_Col = _color;
		return;
	}

	m_tNext->fLight_Col = _color;
	m_fColTime = _time;
	m_fColTick = 0.0F;
}

//Change the color of the light
//If time is  <= 0 the change will happen immediately
//If time is > 0 the change will happen over time.
void CAmbient_Light::ChangeColors(XMFLOAT4 _color, float _time)
{
	ChangeColors({ _color.x, _color.y, _color.z }, _time);

}
#pragma endregion