#pragma once

//STL includes
#include <DirectXMath.h>

//Header includes

//Forward Declares

bool CheckFloats(XMVECTOR _data);
bool CheckFloats(XMMATRIX _data);
bool CheckFloats(XMFLOAT4X4 _data);
bool CheckFloats(XMFLOAT3X3 _data);
bool CheckFloats(XMFLOAT4 _data);
bool CheckFloats(XMFLOAT3 _data);
bool CheckFloats(float* _data, UINT _size);
bool CheckFloats(float _data);


