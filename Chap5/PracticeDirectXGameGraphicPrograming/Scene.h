#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class AnimationLinear;

class Scene
{
public:
	Scene();
	~Scene();

	HRESULT Create(LPDIRECT3DDEVICE9 pD3DDevice);

	void Transform(LPDIRECT3DDEVICE9 pD3DDevice);

	void Draw(LPDIRECT3DDEVICE9 pD3DDevice);

	void Destroy();

private:
	D3DXMATRIX m_proj; // �ˉe�s��
	D3DXMATRIX m_view;//�r���[�C���O�s��
	D3DXMATRIX m_world;



	// ���b�V��
	LPD3DXMESH		m_pMesh;			
	DWORD			m_numMesh;			

	//  �}�e���A��
	D3DMATERIAL9	m_material;

	// ���C�g
	D3DLIGHT9		m_light;

	// �A�j���[�V����
	AnimationLinear* m_pAnim;

};

