/*
サンプルを参考に写経
*/

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

// 依存するクラス
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
	// IDirect3D9へのポインタ
	LPDIRECT3D9 m_pD3D;

	// IDirect3DDevice9へのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	// デバイスのプレゼンテーションパラメーター
	D3DPRESENT_PARAMETERS m_D3DPP;
};

