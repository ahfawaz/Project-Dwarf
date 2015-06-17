#if __cplusplus

#include <DirectXMath.h>
using namespace DirectX;

typedef XMFLOAT4 float4;
typedef XMFLOAT3 float3;
typedef XMFLOAT2 float2;

typedef XMUINT4  uint4;
typedef XMUINT3  uint3;
typedef XMUINT2  uint2;

typedef XMFLOAT4X4 float4x4;
typedef XMFLOAT3X3 float3x3;

///The following is not my code. This code is from Full Sail's Game Engine used for their classes.
// lets us ensure constant buffers and their variables are 16byte aligned to HLSL 4-float registers
#define _regAlign /*__declspec(align(16))*/
// allows us to attach semantics to HLSL variables without bugging C++
#define SEMANTIC(s_name) /* : s_name */

#define CONSTANT_BUFFER_BEGIN(cb_name, reg) struct _regAlign cb_name {
#define CONSTANT_BUFFER_END };

#else
#pragma pack_matrix(row_major)

// lets us ensure constant buffers and variables are 16byte aligned (HLSL will do this for us anyway)
#define _regAlign /**/
// allows us to attach semantics to HLSL variables without bugging C++
#define SEMANTIC(s_name) : s_name
// In HLSL constant buffers will be identified by their name and sequential ordering
#define CONSTANT_BUFFER_BEGIN(cb_name, reg) cbuffer cb_name : register(reg){
//#define CONSTANT_BUFFER_BEGIN(cb_name) cbuffer cb_name {
#define CONSTANT_BUFFER_END }
///End of Full Sail's Game Engine code.
#endif
