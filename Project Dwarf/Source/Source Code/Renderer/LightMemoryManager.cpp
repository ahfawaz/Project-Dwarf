#include "../Application/stdafx.h"

#include "LightMemoryManager.h"

//#incldue STL

//#include Headers
#include "../Components/Component.h"

CLightMemoryManager::CLightMemoryManager() : m_pLightBufferData(nullptr), m_pPointLightStartData(nullptr), m_pConeLightStartData(nullptr), m_pDirLightStartData(nullptr),
m_unLightBufferTotalSize(NULL), m_unNumPointLights(NULL), m_unNumConeLights(NULL), m_unNumDirLights(NULL)
{
	m_unSizePointLight = sizeof(tPoint_Light);
	m_unSizeConeLight = sizeof(tCone_Light);
	m_unSizeDirLight = sizeof(tDirectional_Light);
}
CLightMemoryManager::~CLightMemoryManager()
{
	ClearLightMemory();
}

CLightMemoryManager::CLightMemoryManager(CLightMemoryManager & cpy)
{
	*this = cpy;
}
CLightMemoryManager& CLightMemoryManager::operator = (CLightMemoryManager & assign)
{
	this->ClearLightMemory();
	
	if (assign.m_pLightBufferData)
	{
		this->m_pLightBufferData = (uint8_t*)malloc(assign.m_unLightBufferTotalSize);
		memcpy_s(this->m_pLightBufferData, this->m_unLightBufferTotalSize, assign.m_pLightBufferData, 1);

		this->m_pPointLightStartData = this->m_pLightBufferData;
		this->m_pConeLightStartData = this->m_pPointLightStartData + (assign.m_unNumPointLights * assign.m_unSizePointLight);
		this->m_pDirLightStartData = this->m_pConeLightStartData + (assign.m_unNumConeLights * assign.m_unSizeConeLight);

		this->m_unLightBufferTotalSize = assign.m_unLightBufferTotalSize;
		this->m_unNumPointLights = assign.m_unNumPointLights;
		this->m_unNumConeLights = assign.m_unNumConeLights;
		this->m_unNumDirLights = assign.m_unNumDirLights;
	}

	return *this;
}

bool CLightMemoryManager::Initialize()
{
	return true;
}

void CLightMemoryManager::Shutdown()
{
	ClearLightMemory();
}

void CLightMemoryManager::Update()
{

}

bool CLightMemoryManager::LoadLightData(vector<CPoint_Light*> _vPointLightComp, vector<CCone_Light*> _vConeLightComp, vector<CDirectional_Light*> _vDirLightComp,
vector<tPoint_Light> _vPointLightData, vector<tCone_Light> _vConeLightData, vector<tDirectional_Light> _vDirLightData)
{
	m_unNumPointLights = _vPointLightComp.size();
	m_unNumConeLights = _vConeLightData.size();
	m_unNumDirLights = _vDirLightData.size();

	m_unLightBufferTotalSize += m_unNumPointLights * m_unSizePointLight;
	m_unLightBufferTotalSize += m_unNumConeLights * m_unSizeConeLight;
	m_unLightBufferTotalSize += m_unNumDirLights * m_unSizeDirLight;

	//Make sure we have not surpassed the data threshhold
	if (m_unLightBufferTotalSize > MAX_DATA_SIZE_LIGHTS || m_unNumPointLights > MAX_NUM_POINT_LIGHTS ||
		m_unNumConeLights > MAX_NUM_CONE_LIGHTS || m_unNumDirLights > MAX_NUM_DIR_LIGHTS)
		return false;

	//Allocate the new memory in a contigous chunk.
	m_pLightBufferData = (uint8_t*)malloc((size_t)m_unLightBufferTotalSize);

	//The Start of the memory we're copying to.
	uint8_t * start_memory_ptr = m_pLightBufferData;

	//Copy over the first set of data and save its starting address.
	memcpy_s(start_memory_ptr, m_unLightBufferTotalSize, &_vPointLightData[0], (m_unSizePointLight * m_unNumPointLights));
	m_pPointLightStartData = start_memory_ptr;

	//Move the start pointer to the end of point light data
	start_memory_ptr += (m_unNumPointLights * m_unSizePointLight);

	//Copy over the second set of data and save its starting address.
	if (m_unNumConeLights > 0)
	{
		memcpy_s(start_memory_ptr, m_unLightBufferTotalSize, &_vPointLightData[0], (m_unSizeConeLight * m_unNumConeLights));
		m_pConeLightStartData = start_memory_ptr;
	}
	//Move the start pointer to the end of cone light data
	start_memory_ptr += (m_unNumConeLights * m_unSizeConeLight);

	//Copy over the second set of data and save its starting address.
	if (m_unNumDirLights > 0)
	{
		memcpy_s(start_memory_ptr, m_unLightBufferTotalSize, &_vPointLightData[0], (m_unSizeDirLight * m_unNumDirLights));
		m_pDirLightStartData = start_memory_ptr;
	}

	//Now that all the memory is set, let's tell the components where their lights are.

	//Start with the Point lights
	start_memory_ptr = m_pPointLightStartData;
	uint8_t * memory_position_to_store = nullptr;
	for (uint i = 0; i < m_unNumPointLights; i++)
	{
		memory_position_to_store = start_memory_ptr + (m_unSizePointLight * i);
		_vPointLightComp[i]->SetAddress(&memory_position_to_store);
	}

	//Next with Cone Lights
	start_memory_ptr = m_pConeLightStartData;
	memory_position_to_store = nullptr;
	for (uint i = 0; i < m_unNumConeLights; i++)
	{
		memory_position_to_store = start_memory_ptr + (m_unSizeConeLight * i);
		_vConeLightComp[i]->SetAddress(&memory_position_to_store);
	}

	//Last is the Directional Lights
	start_memory_ptr = m_pDirLightStartData;
	memory_position_to_store = nullptr;
	for (uint i = 0; i < m_unNumDirLights; i++)
	{
		memory_position_to_store = start_memory_ptr + (m_unSizeDirLight * i);
		_vDirLightComp[i]->SetAddress(&memory_position_to_store);
	}

	return true;
}


void CLightMemoryManager::GetDataFromBuffer(uint8_t const * const _lightAddress, const uint _lightType, void* _data) const
{
	switch (_lightType)
	{
	case eCompTypes::ePOINT_LIGHT:
		memcpy_s(_data, m_unSizePointLight, _lightAddress, 1);
		break;
	case eCompTypes::eCONE_LIGHT:
		memcpy_s(_data, m_unSizeConeLight, _lightAddress, 1);
		break;
	case eCompTypes::eDIR_LIGHT:
		memcpy_s(_data, m_unSizeDirLight, _lightAddress, 1);
		break;
	default:
		_data = nullptr;
		break;
	}
}

void CLightMemoryManager::SetDataInBuffer(uint8_t * const _lightAddress, const uint _lightType, void const * const _data)
{
	switch (_lightType)
	{
	case eCompTypes::ePOINT_LIGHT:
		memcpy_s(_lightAddress, m_unSizePointLight, _data, 1);
		break;
	case eCompTypes::eCONE_LIGHT:
		memcpy_s(_lightAddress, m_unSizeConeLight, _data, 1);
		break;
	case eCompTypes::eDIR_LIGHT:
		memcpy_s(_lightAddress, m_unSizeDirLight, _data, 1);
		break;
	case eCompTypes::eAMB_LIGHT:
		
	default:
		break;
	}
}


void CLightMemoryManager::ClearLightMemory()
{
	if (m_pLightBufferData)
	{
		free(m_pLightBufferData);
		m_pPointLightStartData = nullptr;
		m_pConeLightStartData = nullptr;
		m_pDirLightStartData = nullptr;
	}
}