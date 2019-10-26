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


	// VertexBuffer
	LPDIRECT3DVERTEXBUFFER9		m_pMyVB;
	LPDIRECT3DVERTEXBUFFER9		m_pMyBack;

	// 頂点１つのデータ型
	struct MY_VERTEX {
		D3DXVECTOR3 p;		// 位置
		DWORD		color;	// 色
		D3DXVECTOR2 t;      // テクスチャーの画像の位置
	};

	// ＦＶＦの設定
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)

// Texture
	LPDIRECT3DTEXTURE9			m_pTexture;
	LPDIRECT3DTEXTURE9			m_pTextureBack;



};

