#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"




Renderer::Renderer() : m_pD3D(0),m_pD3DDevice(0)
{
}


Renderer::~Renderer()
{
	Finalize();
}

/*
�����_���[�̏�����
*/
HRESULT Renderer::Init(HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight)
{
	// �f�B�X�v���C���[�h
	D3DDISPLAYMODE d3ddm;

	//Direct3d9�̍쐬
	if ((m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0){
		return	E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h��d3ddm�֎擾���A�G���[�`�F�b�N
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// D3DPRESENT_PARAMETERS�T�C�Y�𓮓I�Ɋm�ہB�����l��0
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));

	//�o�b�N�o�b�t�@�̐�
	m_D3DPP.BackBufferCount = 1;
	if (isFullScreen)
	{
		m_D3DPP.Windowed = false;
		m_D3DPP.BackBufferWidth = clientWidth;
		m_D3DPP.BackBufferHeight = clientHeight;
	}
	else {
		m_D3DPP.Windowed = true;
	}

	m_D3DPP.BackBufferFormat = d3ddm.Format;
	//�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̐؂�ւ����@ ���f�B�X�v���C�h���C�o���������f���[�h��
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.EnableAutoDepthStencil = TRUE;
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	/*
	�f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬

	*/
	// �`��ƒ��_�������n�[�h�E�F�A��
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&m_D3DPP, &m_pD3DDevice))) {
		//�@�`��̓n�[�h�E�F�A�B���_��CPU
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&m_D3DPP, &m_pD3DDevice))) {
			// �`��ƒ��_��CPU
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&m_D3DPP, &m_pD3DDevice))) {
				return E_FAIL;
			}
		}
	}


	return S_OK;
}

// �f�o�C�X�̎擾
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return m_pD3DDevice;
}

void Renderer::RenderScene(Scene * pScene)
{
	if (pScene == 0)
		return;

	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	if (FAILED(m_pD3DDevice->Clear(0, NULL,
		D3DCLEAR_TARGET |
		D3DCLEAR_ZBUFFER,// �u�o�b�N�o�b�t�@�v�{�u�[�x�o�b�t�@�v
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f,
		0))) {
		return;
	}

	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {

		pScene->Draw(m_pD3DDevice);

		m_pD3DDevice->EndScene();
	}

	// �`�挋�ʂ��f�o�C�X���֓]��
	if (FAILED(m_pD3DDevice->Present(0, 0, 0, 0))) {
		// ���A
		m_pD3DDevice->Reset(&m_D3DPP);
	}
}

void Renderer::Finalize()
{
	if (m_pD3DDevice) {
		m_pD3DDevice->Release(); 
		m_pD3DDevice = NULL;
	}

	if (m_pD3D) {
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}
