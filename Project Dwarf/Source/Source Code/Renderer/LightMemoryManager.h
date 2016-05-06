#pragma once

//STL Includes
#include <vector>

//Header Includes
//#include "Shaders/LightBuffers.h"
#include "Shaders\BufferDefines.h"
#include "../Components/LightComponent.h"

//Namespaces Used
using namespace std;

//Forward Declares
class CPoint_Light;
class CCone_Light;
class CDirectional_Light;

//Defines used
#define MAX_DATA_SIZE_LIGHTS	192816u ///Maximum size(in bytes) for storing lights. Cannot exceed this limit.
#define MAX_NUM_POINT_LIGHTS	2048u
#define MAX_NUM_CONE_LIGHTS		2048u
#define MAX_NUM_DIR_LIGHTS		100u
#define MAX_NUM_AMB_LIGHTS		5u

class CLightMemoryManager
{
private:

	//Memory Buffer for the light data. Contains all the information for the lights.
	uint8_t * m_pLightBufferData;

	//Total size of the light memory stored.
	uint m_unLightBufferTotalSize;

	//Memory Start points for light data. Contains the information where the first of each light type starts.
	uint8_t * m_pPointLightStartData;
	uint8_t * m_pConeLightStartData;
	uint8_t * m_pDirLightStartData;

	//Number of lights within the contained data.
	uint m_unNumPointLights;
	uint m_unNumConeLights;
	uint m_unNumDirLights;

	//Memory size of each light type. 
	//Light Data is static and can only be changed in Lightbuffers.h
	//This makes it easier for looking up the struct size.
	uint m_unSizePointLight;
	uint m_unSizeConeLight;
	uint m_unSizeDirLight;

	///Lets hold the memory for the Ambient Lights locally on the stack and figure out the details later.
	tAmbient_Light m_vAmbLights[5];

public:

	CLightMemoryManager();
	~CLightMemoryManager();
	
	CLightMemoryManager(CLightMemoryManager & cpy);
	CLightMemoryManager& operator=(CLightMemoryManager & assign);

	bool Initialize();

	void Shutdown();

	void Update();

	bool LoadLightData(vector<CPoint_Light*> _vPointLightComp, vector<CCone_Light*> _vConeLightComp, vector<CDirectional_Light*> _vDirLightComp,
		vector<tPoint_Light> _vPointLightData, vector<tCone_Light> _vConeLights, vector<tDirectional_Light> _vDirLights);
	void GetDataFromBuffer(uint8_t const * const _lightAddress, const uint _lightType, void* _data) const;
	void SetDataInBuffer(uint8_t * const _lightAddress, const uint _lightType, void const * const _data);

	void ClearLightMemory();
};