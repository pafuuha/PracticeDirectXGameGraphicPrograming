#include "stdafx.h"
#include "Scene.h"

#include <mmsystem.h>
#include "Scene.h"

Scene::Scene() : m_pMesh(0), m_numMtrl(0L), m_pD3DXMtrlBuffer(0)
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

	// X�t�@�C��
	if (FAILED(D3DXLoadMeshFromX(TEXT("robo0.x"),// �w�t�@�C���̃t�@�C����
		D3DXMESH_SYSTEMMEM,	// ���b�V�����V�X�e���������ɓW�J����
		pD3DDevice,			// D3D�f�o�C�X�I�u�W�F�N�g
		NULL,
		&m_pD3DXMtrlBuffer,	// �����o�b�t�@�̐ݒ�
		NULL,
		&m_numMtrl,			// �����̐�
		&m_pMesh))) {		// �i�[��̃��b�V��
		return E_FAIL;
	}

	/*
	// ����
	if (FAILED(FAILED(D3DXCreateSphere(pD3DDevice,
		1.0f,
		36,
		36,
		&m_pMesh,
		NULL)))) {
		return E_FAIL;
	}
	*/

	/*
	���_
	if (FAILED(pD3DDevice->CreateVertexBuffer(3 * sizeof(MY_VERTEX), // MyVerTex3���m��
		D3DUSAGE_WRITEONLY, //���\�[�X�̐���B���_�o�b�t�@�̂�
		MY_VERTEXT_FVF, // ���_�t�H�[�}�b�g�BFVF
		D3DPOOL_MANAGED, //�ǂ̃��������g�p���邩
		&m_pMyVB, NULL))) {
		return E_FAIL;
	}

	MY_VERTEX* v;


	//
	// �\��������̂̍쐬
	//

	m_pMyVB->Lock(0, 0, (void**)&v, 0);

	//���_�f�[�^�F���_�̍��W�l�̐ݒ�
	v[0].p = D3DXVECTOR3(-1.5f, 1.5f, 0.0f);	// ���_�@
	v[1].p = D3DXVECTOR3(1.5f, 1.5f, 0.0f);	// ���_�A
	v[2].p = D3DXVECTOR3(-1.5f, -1.5f, 0.0f);	// ���_�B
	v[3].p = D3DXVECTOR3(1.5f, -1.5f, 0.0f);	// ���_�C

	//���_�f�[�^�F���_�̊�{�I�ȐF�̐ݒ�i�ԐF�ɐݒ�j
	v[0].diffuse = v[1].diffuse = v[2].diffuse = v[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//���_�f�[�^�F���_�̋��ʔ��ː����̐ݒ�i1�ڂ̒��_�ɂ������̃X�y�L����������ݒ�j
	v[0].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	v[1].specular = v[2].specular = v[3].specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_pMyVB->Unlock();
	*/

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
//	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// ���ʔ��ː����̗L���� 	
//	pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	D3DLIGHT9 mLight;
	ZeroMemory(&mLight, sizeof(D3DLIGHT9));

	//���C�g�̃^�C�v
	mLight.Type = D3DLIGHT_DIRECTIONAL;

	//�g�U��
	mLight.Diffuse.r = 1.0f;
	mLight.Diffuse.g = 1.0f;
	mLight.Diffuse.b = 1.0f;
	/*
	// ����
	m_light.Ambient.r = 0.5f;
	m_light.Ambient.g = 0.5f;
	m_light.Ambient.b = 0.5f;

	// ���ʔ��ˌ�
	m_light.Specular.r = 1.0f;
	m_light.Specular.g = 1.0f;
	m_light.Specular.b = 1.0f;
	*/


	//���C�g�̕���
	D3DXVECTOR3 mVecDir;
	mVecDir = D3DXVECTOR3(-5.0f, -5.0f, 10.0f);
	// ���K��
	D3DXVec3Normalize((D3DXVECTOR3*)&mLight.Direction, &mVecDir);

	// �J������ݒ�
	pD3DDevice->SetLight(0, &mLight);

	//�L����
	pD3DDevice->LightEnable(0, TRUE);

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// D3DMATERIAL9�\���̂�0�ŃN���A
//	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));

	// ���f���̐F��ݒ�
	/*
	// �g�U��
	m_material.Diffuse.r = 0.5f;
	m_material.Diffuse.g = 1.0f;
	m_material.Diffuse.b = 0.5f;
	m_material.Diffuse.a = 1.0f;
	// ����
	m_material.Ambient.r = 0.2f;
	m_material.Ambient.g = 0.5f;
	m_material.Ambient.b = 0.2f;
	m_material.Ambient.a = 1.0f;

	// ���ʔ��ˌ�
	m_material.Specular.r = 0.5f;
	m_material.Specular.g = 1.0f;
	m_material.Specular.b = 0.5f;
	m_material.Specular.a = 1.0f;
	m_material.Power = 4000.0f;
	// �g�U��
	m_material.Emissive.r = 0.5f;
	m_material.Emissive.g = 1.0f;
	m_material.Emissive.b = 0.5f;
	m_material.Emissive.a = 1.0f;
	*/
	//
	// �r���[�C���O�̍s����������B�J����
	//
	
	// �P�ʍs����쐬
	// D3DXMATRIX��4*4�̍s��
	D3DXMatrixIdentity(&m_view);
	// �������ݒ�
	// �p�����[�^�[�F(�o�́A���_�A�����_�A�����)
	D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	
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

	if (m_pMesh == 0)
		return;

	Transform(pD3DDevice);
	// ���W�ϊ��i���f���̔z�u�j
	D3DXMATRIX	mScale;
	D3DXMATRIX	mRot;
	D3DXMATRIX	mMove;
	D3DXMatrixIdentity(&m_world);

	// �g��k��
	D3DXMatrixScaling(&mScale, 2.0f, 0.5f, 1.0f);

	// ��]
	D3DXMatrixRotationY(&mRot, D3DXToRadian(45.0f));

	// �ړ�
	D3DXMatrixTranslation(&mMove, 4.0f, 3.0f, 0.0f);

	// �����P�@�g��k���Ɖ�]
	D3DXMATRIX	mMult;
	D3DXMatrixMultiply(&mMult, &mScale, &mRot);

	// �����Q�@�����P�ƈړ��@
	D3DXMatrixMultiply(&m_world, &mMult, &mMove);

	// �}�g���b�N�X�������_�����O�p�C�v���C���ɐݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);


	D3DMATERIAL9	mMaterial;
	// �}�e���A���������o��
	D3DXMATERIAL*	mD3DXMaterials = (D3DXMATERIAL*)m_pD3DXMtrlBuffer->GetBufferPointer();
	for (DWORD i = 0; i < m_numMtrl; i++) {
		// �}�e���A���̃R�s�[
		mMaterial = mD3DXMaterials[i].MatD3D;
		// �}�e���A���̃Z�b�g
		pD3DDevice->SetMaterial(&mMaterial);
		// �������ꂽ���b�V���̕`��
		m_pMesh->DrawSubset(i);
	}

	// �}�e���A���������_�����O�p�C�v���C���ɐݒ�
//	pD3DDevice->SetMaterial(&m_material);

	// ���f���̕`��
//	m_pMesh->DrawSubset(0);

	/*
	pD3DDevice->SetStreamSource(0, m_pMyVB, 0, sizeof(MY_VERTEX));

	pD3DDevice->SetFVF(MY_VERTEXT_FVF);

	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	*/
}

void Scene::Destroy()
{
	if (m_pD3DXMtrlBuffer)
	{
		m_pD3DXMtrlBuffer->Release();
		m_pD3DXMtrlBuffer = NULL;
	}


	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

