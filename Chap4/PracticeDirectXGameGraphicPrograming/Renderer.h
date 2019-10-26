/*
�T���v�����Q�l�Ɏʌo
*/

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

// �ˑ�����N���X
class Scene;

class Renderer
{
public:
	Renderer();
	~Renderer();

	HRESULT Init(HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight);

	LPDIRECT3DDEVICE9 GetDevice();

	void RenderScene(Scene* pScene);

	void Finalize();

private:
	// IDirect3D9�ւ̃|�C���^
	LPDIRECT3D9 m_pD3D;

	// IDirect3DDevice9�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�[
	D3DPRESENT_PARAMETERS m_D3DPP;
};

