#pragma once 

//STL Includes 

//Header Includes


//Namespaces used

//Forward Declares
class CGameObject;
class CRenderComponent;

class CConstantBufferManager
{
	//Class Members
	ID3D11Buffer* m_pWorldView;
	ID3D11Buffer* m_pWorldPos;
	ID3D11Buffer* m_pBatchPos;
	ID3D11DeviceContext* _pContext;

public:
	CConstantBufferManager();
	~CConstantBufferManager();

	bool Initialize(ID3D11Device* _device, ID3D11DeviceContext* _context);
	void Shutdown();
	void UpdateConstBuffers(CGameObject* _obj, CRenderComponent* _comp);
	void UpdateViewAndProjBuffer(XMFLOAT4X4 _view, XMFLOAT4X4 _proj);
	void BindConstBuffer(CRenderComponent* _comp) const;

private:

	template <class TYPE>
	void BufferUpdate(ID3D11Buffer ** _buffer, TYPE & _data);

	template <class TYPE>
	void BufferUpdate(ID3D11Buffer ** _buffer, TYPE & _data, UINT _data_size);


};
