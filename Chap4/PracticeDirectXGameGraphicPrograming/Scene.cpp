#include "stdafx.h"
#include "Scene.h"

#include <mmsystem.h>
#include "Scene.h"

Scene::Scene()
	:m_pMyVB(0), m_pTexture(0), m_pMyBack(0), m_pTextureBack(0)
{
}


Scene::~Scene()
{
	Destroy();
}

HRESULT Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Destroy();

	if (pD3DDevice == 0) {
		return E_FAIL;
	}


	
	if (FAILED(pD3DDevice->CreateVertexBuffer(4 * sizeof(MY_VERTEX),
		D3DUSAGE_WRITEONLY, //���\�[�X�̐���B���_�o�b�t�@�̂�
		MY_VERTEX_FVF, // ���_�t�H�[�}�b�g�BFVF
		D3DPOOL_MANAGED, //�ǂ̃��������g�p���邩
		&m_pMyVB, NULL))) {
		return E_FAIL;
	}

	MY_VERTEX* v;


	//
	// �\��������̂̍쐬
	//

	m_pMyVB->Lock(0, 0, (void**)&v, 0);

	// ���_���W�̐ݒ�
	v[0].p = D3DXVECTOR3(-1.5f, 1.5f, 0.0f);
	v[1].p = D3DXVECTOR3(1.5f, 1.5f, 0.0f);
	v[2].p = D3DXVECTOR3(-1.5f, -1.5f, 0.0f);
	v[3].p = D3DXVECTOR3(1.5f, -1.5f, 0.0f);

	// �e�N�X�`�����W�̐ݒ�
	v[0].t = D3DXVECTOR2(0.0f, 0.0f);
	v[1].t = D3DXVECTOR2(1.0f, 0.0f);
	v[2].t = D3DXVECTOR2(0.0f, 1.0f);
	v[3].t = D3DXVECTOR2(1.0f, 1.0f);
	// ���_�J���[�̐ݒ�
	v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_pMyVB->Unlock();
	

	// 4�̒��_����Ȃ钸�_�o�b�t�@�����
	if (FAILED(pD3DDevice->CreateVertexBuffer(4 * sizeof(MY_VERTEX),
		D3DUSAGE_WRITEONLY,
		MY_VERTEX_FVF,
		D3DPOOL_MANAGED, &m_pMyBack, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	m_pMyBack->Lock(0, 0, (void**)&v, 0);
	v[0].p = D3DXVECTOR3(-10.0f, 10.0f, 12.0f);
	v[1].p = D3DXVECTOR3(10.0f, 10.0f, 12.0f);
	v[2].p = D3DXVECTOR3(-10.0f, -10.0f, 12.0f);
	v[3].p = D3DXVECTOR3(10.0f, -10.0f, 12.0f);
	v[0].t = D3DXVECTOR2(0.0f, 0.0f);
	v[1].t = D3DXVECTOR2(10.0f, 0.0f);
	v[2].t = D3DXVECTOR2(0.0f, 10.0f);
	v[3].t = D3DXVECTOR2(10.0f, 10.0f);
	v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMyBack->Unlock();


	//
	//�@�e�N�X�`���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�摜�ǂݍ���
	//
	D3DXCreateTextureFromFileEx(pD3DDevice,
		TEXT("tex.bmp"),
		0, 0, 0, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
		D3DCOLOR_XRGB(0, 0, 0), 		// �J���[�L�[
		NULL,
		NULL,
		&m_pTexture);

	D3DXCreateTextureFromFile(pD3DDevice, TEXT("bg.bmp"), &m_pTextureBack);


	//
	// �e�N�X�`���X�e�[�W
	//
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ���ߏ���
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//
	// �����_�����O�̃p�����[�^�[�ݒ�
	//

	// ���ʕ`�惂�[�h�̎w��
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// �m�[���C�e�B���O���[�h 	
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// ���ʔ��ː����̗L���� 	
//	pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);


	//
	// �r���[�C���O�̍s����������B�J����
	//
	// �P�ʍs����쐬
	// D3DXMATRIX��4*4�̍s��
	D3DXMatrixIdentity(&m_view);
	// �������ݒ�
	// �p�����[�^�[�F(�o�́A���_�A�����_�A�����)
	D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0.0f, 0.0f, -6.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	
	return S_OK;
}


// ���W�ϊ����s��
void Scene::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == 0)
		return;

	// �r���[�|�[�g�̐ݒ�
	// �r���[�|�[�g�ŏI�I�Ɍ����ʂ͈̔�
	/*
	DirecX�̏ꍇ �����l
		D3DVIEWPORT9 vp;
		vp.X = 0;
		vp.Y = 0;
		vp.Width = RenderTarget.Width;
		vp.Height = RenderTarget.Height;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		// vp.X, vp.Y ���`�ʂ��鍶��̍��W
		// vp.Width, vp.Height ���`�ʕ�
		// vp.MinZ, vp.MaxZ ���[�x�l�̕ϊ��ł�
	*/
	D3DVIEWPORT9 vp;
	if (FAILED(pD3DDevice->GetViewport(&vp)))
		return;

	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	//
	// �ˉe�s��
	// 
	// �P�ʍs��
	D3DXMatrixIdentity(&m_proj);
	// �쐬
	//�p�����[�^�[: (�o�́AFOV�i�J�����̉�p�j�A�A�X�y�N�g�A�ǂ��������ʂ����iNear�v���[���j�A�ǂ��܂ł��ʂ����iFar�v���[���j
	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 1.0f, 1000.0f);

	//
	//  �f�o�C�X�ɂ��ꂼ��̍s���ݒ�
	// 
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

}

// �`��
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{

	if (pD3DDevice == 0)
		return;

	if (m_pMyVB == 0)
		return;


	// ���W�ϊ��i�J�����̐ݒ�Ȃǁj
	Transform(pD3DDevice);

	// ���W�ϊ��i���f���̔z�u�j
	D3DXMatrixIdentity(&m_world);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);




	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	pD3DDevice->SetTexture(0, m_pTextureBack);

	// ���_�o�b�t�@�̕`��
	pD3DDevice->SetStreamSource(0, m_pMyBack, 0, sizeof(MY_VERTEX));
	pD3DDevice->SetFVF(MY_VERTEX_FVF);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);



	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	pD3DDevice->SetTexture(0, m_pTexture);

	// ���_�o�b�t�@�̕`��
	pD3DDevice->SetStreamSource(0, m_pMyVB, 0, sizeof(MY_VERTEX));
	pD3DDevice->SetFVF(MY_VERTEX_FVF);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void Scene::Destroy()
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}


	if (m_pTextureBack)
	{
		m_pTextureBack->Release();
		m_pTextureBack = NULL;
	}

	if (m_pMyVB)
	{
		m_pMyVB->Release();
		m_pMyVB = NULL;
	}

	if (m_pMyBack)
	{
		m_pMyBack->Release();
		m_pMyBack = NULL;
	}
}

