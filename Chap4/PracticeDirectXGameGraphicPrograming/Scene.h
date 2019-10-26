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
	D3DXMATRIX m_proj; // �ˉe�s��
	D3DXMATRIX m_view;//�r���[�C���O�s��
	D3DXMATRIX m_world;


	// VertexBuffer
	LPDIRECT3DVERTEXBUFFER9		m_pMyVB;
	LPDIRECT3DVERTEXBUFFER9		m_pMyBack;

	// ���_�P�̃f�[�^�^
	struct MY_VERTEX {
		D3DXVECTOR3 p;		// �ʒu
		DWORD		color;	// �F
		D3DXVECTOR2 t;      // �e�N�X�`���[�̉摜�̈ʒu
	};

	// �e�u�e�̐ݒ�
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)

// Texture
	LPDIRECT3DTEXTURE9			m_pTexture;
	LPDIRECT3DTEXTURE9			m_pTextureBack;



};

