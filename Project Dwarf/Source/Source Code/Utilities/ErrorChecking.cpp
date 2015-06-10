#include "../Application/stdafx.h"

#include "ErrorChecking.h"

#define CHECK

#if _DEBUG
#ifdef CHECK
bool CheckFloats(XMVECTOR _data)
{
	for (size_t i = 0; i < 4u; i++)
	{
		if (_data.m128_f32[i] != _data.m128_f32[i])
			return false;
	}

	return true;
}
bool CheckFloats(XMMATRIX _data)
{
	for (size_t i = 0; i < 4u; i++)
	{
		for (size_t j = 0; j < 4u; j++)
		{
			if (_data.r[i].m128_f32[j] != _data.r[i].m128_f32[j])
				return false;
		}
	}

	return true;
}
bool CheckFloats(XMFLOAT4X4 _data)
{
	for (size_t i = 0; i < 4u; i++)
	{
		for (size_t j = 0; j < 4u; j++)
		{
			if (_data.m[i][j] != _data.m[i][j])
				return false;
		}
	}

	return true;
}
bool CheckFloats(XMFLOAT3X3 _data)
{
	for (size_t i = 0; i < 3u; i++)
	{
		for (size_t j = 0; j < 3u; j++)
		{
			if (_data.m[i][j] != _data.m[i][j])
				return false;
		}
	}

	return true;
}
bool CheckFloats(XMFLOAT4 _data)
{
	if (_data.x != _data.x)
		return false;
	if (_data.y != _data.y)
		return false;
	if (_data.z != _data.z)
		return false;
	if (_data.w != _data.w)
		return false;

	return true;
}
bool CheckFloats(XMFLOAT3 _data)
{
	if (_data.x != _data.x)
		return false;
	if (_data.y != _data.y)
		return false;
	if (_data.z != _data.z)
		return false;

	return true;
}
bool CheckFloats(float* _data, UINT _size)
{
	for (size_t i = 0; i < _size; i++)
	{
		if (_data[i] != _data[i])
			return false;
	}

	return true;
}
bool CheckFloats(float _data)
{
	return _data == _data;
}

#else
bool CheckFloats(XMVECTOR _data) {return true;}
bool CheckFloats(XMMATRIX _data) {return true;}
bool CheckFloats(XMFLOAT4X4 _data) {return true;}
bool CheckFloats(XMFLOAT3X3 _data) {return true;}
bool CheckFloats(XMFLOAT4 _data) {return true;}
bool CheckFloats(XMFLOAT3 _data) {return true;}
bool CheckFloats(float* _data, UINT _size) {return true;}
bool CheckFloats(float _data) { return true; }
#endif
#endif
