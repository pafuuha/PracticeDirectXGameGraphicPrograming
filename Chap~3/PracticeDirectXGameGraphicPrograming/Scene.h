#pragma once

#include <d3d9.h>
#include <d3dx9.h>

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

	LPD3DXMESH  m_pMesh;
	LPD3DXBUFFER	m_pD3DXMtrlBuffer;	
	DWORD			m_numMtrl;			

	/*
	D3DLIGHT9 m_light;
	D3DMATERIAL9 m_material;
	*/

	/*

	// 頂点バッファー
	LPDIRECT3DVERTEXBUFFER9		m_pMyVB;

	//頂点バッファ用
	struct MY_VERTEX {
		D3DXVECTOR3 p;
		DWORD diffuse;
		DWORD specular;
	};

	#define MY_VERTEXT_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR)
	*/



};

