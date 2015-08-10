#pragma once

//STL includes
#include <d3d11.h>
#pragma comment(lib, "d3d11")

//Header includes

//Namespaces used

//Foward Declares
class Core;
class CRenderComponent;

class CShaderManager
{
	//m_vs[name] are vertex shader pointers.
	//m_ps[name] are pixel shader pointers.
	//m_il[name] are input layout pointers.

	//Vertex Shaders
	ID3D11VertexShader* m_vsEmptyVertex;
	ID3D11VertexShader* m_vsStaticNoTangents;
	ID3D11VertexShader* m_vsStaticWithTangents;

	//Geometry Shaders

	//Pixel Shaders
	ID3D11PixelShader* m_psBasic;
	ID3D11PixelShader* m_psTexture;

	//Input Layouts
	ID3D11InputLayout* m_ilStaticNoTangents;
	ID3D11InputLayout* m_ilStaticWithTangents;

	UINT m_ShaderFlag;
	UINT m_LayoutFlag;

public:
	CShaderManager();
	~CShaderManager();

	bool Initialize(ID3D11Device* _device);
	void Shutdown();

	void BindShaders(ID3D11DeviceContext* _context, CRenderComponent* _comp);
	void SetInputLayout(ID3D11DeviceContext* _context, CRenderComponent* _comp);

private:
	HRESULT LoadVertexShaders(ID3D11Device* _device);
	HRESULT LoadGeometryShaders(ID3D11Device* _device);
	HRESULT LoadPixelShaders(ID3D11Device* _device);
	HRESULT LoadInputLayouts(ID3D11Device* _device);

};

