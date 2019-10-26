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
	D3DXMATRIX m_proj; // 射影行列
	D3DXMATRIX m_view;//ビューイング行列
	D3DXMATRIX m_world;



	// メッシュ
	LPD3DXMESH		m_pMesh;			
	DWORD			m_numMesh;			

	//  マテリアル
	D3DMATERIAL9	m_material;

	// ライト
	D3DLIGHT9		m_light;

	// アニメーション
	AnimationLinear* m_pAnim;

};

