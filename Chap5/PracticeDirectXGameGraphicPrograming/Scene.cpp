#include "stdafx.h"
#include "Scene.h"

#include <windows.h>
#include <mmsystem.h>

#include "Scene.h"
#include "AnimationLinear.h"

Scene::Scene()
	: m_pMesh(0), m_numMesh(0L), m_pAnim(0)
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


	


	//
	// �\��������̂̍쐬
	//


	//--------------------------------------
	// �w�t�@�C���̓ǂݍ���
	//--------------------------------------
	if (FAILED(D3DXLoadMeshFromX(TEXT("sphere.x"),
		D3DXMESH_SYSTEMMEM,
		pD3DDevice,
		NULL,
		NULL,
		NULL,
		&m_numMesh,
		&m_pMesh))) {
		return E_FAIL;
	}

	//
	// �����_�����O�̃p�����[�^�[�ݒ�
	//

	// ���ʕ`�惂�[�h�̎w��
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);

	///
	/// ���C�g
	///
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̐F�ݒ�
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;

	// ���C�g�̕����̐ݒ�
	D3DXVECTOR3		mVecDir;
	mVecDir = D3DXVECTOR3(-0.5f, -0.5f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &mVecDir);

	// �ݒ�
	pD3DDevice->SetLight(0, &m_light);
	pD3DDevice->LightEnable(0, TRUE);


	//
	// �}�e���A��
	//

	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));

	// ���f���̐F��ݒ�
	m_material.Diffuse.r = 1.0f;
	m_material.Diffuse.g = 0.0f;
	m_material.Diffuse.b = 0.0f;
	m_material.Diffuse.a = 1.0f;


	// 
	// �A�j���[�V����
	//
	m_pAnim = new AnimationLinear;

	// �L�[�t���[�����쐬
	m_pAnim->CreateKey(3);

	// �L�[�t���[���̎��ԁi�����Ŏw��j�ƒl�̐ݒ�	
	m_pAnim->SetKeyValue(0, 0.0f, 0.0f);
	m_pAnim->SetKeyValue(1, 0.5f, 1.0f);
	m_pAnim->SetKeyValue(2, 1.0f, 0.0f);

	// �A�j���[�V�����̌p������	
	m_pAnim->SetDuration(5000);

	// �A�j���[�V���������[�v�ݒ�	
	m_pAnim->SetLoopFlag(TRUE); // TRUE�����[�v������

	// �A�j���[�V�����̃X�^�[�g���Ԃ̐ݒ�	
	DWORD time = timeGetTime(); // ���ݎ������~���P�ʂŎ擾
	m_pAnim->SetStarTime(time);




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

	if (m_pMesh == 0)
		return;


	// ���W�ϊ��i�J�����̐ݒ�Ȃǁj
	Transform(pD3DDevice);

	// ���W�ϊ��i���f���̔z�u�j
	D3DXMatrixIdentity(&m_world);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);


	//
	// �}�e���A���̃A�j��
	//
	float value;
	float red = 0.0f;
	bool isEnd;

	DWORD time = timeGetTime();

	if (m_pAnim->GetValue(time, &value, &isEnd)) {
		red = value;
	}


	// �}�e���A��
	m_material.Diffuse.r = red;
	m_material.Diffuse.g = 0.0f;
	m_material.Diffuse.b = 0.0f;
	pD3DDevice->SetMaterial(&m_material);

	// ���f��

	for (DWORD i = 0; i < m_numMesh; i++) {
		m_pMesh->DrawSubset(i);
	}
}

void Scene::Destroy()
{

	if (m_pAnim != 0) {
		delete m_pAnim;
	}

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

}

